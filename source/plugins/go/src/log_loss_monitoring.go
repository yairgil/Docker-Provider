package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"sync"
	"sync/atomic"
	"time"
)

const log_loss_telemetry_interval = 5 * time.Minute
const log_loss_track_rotations_interval = 10 * time.Second

// increase for lower memory usage and higher cpu usage, decrese for the opposite. Do not set lower than 2.
const garbage_collection_agressiveness = 5

/*
Garbage collection:
This code contains two main threads: FW (File Watcher) and Main (fbit's main thread). Both threads have maps of containers.

The FW thread can tell if a container has been deleted because it can see the container's file on disk disappear.

The Main thread doesn't have a good way of telling a container has disappeared, because it only has access to the incoming log stream (And the container might have
just stopped logging for a long time)

Therefore, how will the main thread delete entries for old containers? Otherwise it's memory usage will grow unboundedly as containers come and go.
Also, the Main thread needs to do as little work as possible because it's on the logging critical path (any delay from it will directly the log volume)

The implemented solution:
1. The FW thread will keep a public count of the number of containers on disk.

2. Every time the Main thread sees a new container and it has significantly more containers stored in its internal data
structures than are in the public count, it will pick five containers from its internal data structures and write their
IDs to a shared chanel (deleted_containers_query) (in a non-blocking way, if the channel is full then retry later)

3. The FW thread will read the containerIds form deleted_containers_query, then if they are not on disk then write the
containerIds back to a second shared channel (deleted_containers_response)

4. The Main thread will read from deleted_containers_response (also non-blocking) and delete any recieved
containerIDs from it's own tables. (the delay is so that any logs from a freshly deleted container will have time to go through fbit)

Motivation: with this method, the Main thread should store about 1.2x as many containers as are on disk (assuming it queries about deleted containers
in a evenly distributed way). If more than 1/5 of the containers in the Main thread's data structures are deleted, then on average query more
than one deleted containerId. That will cause the number of containers the Main thread is storing to shrink.
*/

// Main thread managed data structures:
// var m_container_to_arr_index = make(map[string]int64)
var m_bytes_logged_storage AddressableMap

var m_deletion_query_index = 0

// File Watcher managed data structures
// (FW = File Watcher)
type FwRecord struct {
	unrotated_bytes    int64
	rotated_bytes      int64
	existing_log_files []string
}

var FW_records map[string]FwRecord // identifier -> file names
// var FW_bytes_logged_rotated = make(map[string]int64)   // identifier -> count
// var FW_bytes_logged_unrotated = make(map[string]int64) // identifier -> count
// var current_iteration_count int64 = 0
var num_containers_on_disk int32

// Shared objects
var deleted_containers_query chan string
var deleted_containers_response chan string

var process_logs_mut sync.Mutex
var read_disk_mut sync.Mutex
var enabled bool

var disabled_namespaces map[string]bool

var log_loss_logger *log.Logger

func init() {
	init_log_loss_monitoring_globals()
}

// This function exists for unit tests, it lets each test reset all global variables at the start of each test
func init_log_loss_monitoring_globals() {
	m_bytes_logged_storage = Make_AddressableMap()

	m_deletion_query_index = 0

	FW_records = make(map[string]FwRecord) // identifier -> file names
	// var FW_bytes_logged_rotated = make(map[string]int64)   // identifier -> count
	// var FW_bytes_logged_unrotated = make(map[string]int64) // identifier -> count
	// var current_iteration_count int64 = 0
	num_containers_on_disk = 0

	// Shared objects
	deleted_containers_query = make(chan string, garbage_collection_agressiveness)
	deleted_containers_response = make(chan string, garbage_collection_agressiveness)

	process_logs_mut = sync.Mutex{}
	read_disk_mut = sync.Mutex{}

	disabled_namespaces = make(map[string]bool)
}

// //go:generate mockgen -destination=log_loss_monitoring_mock.go -self_package=main Docker-Provider/source/plugins/go/src IGetEnvVar
//go:generate mockgen -source=log_loss_monitoring.go -destination=log_loss_monitoring_mock.go -packag=main
type IGetEnvVar interface {
	Getenv(key string) string
}
type GetEnvVarImpl struct{}

var env IGetEnvVar = GetEnvVarImpl{}

// This is here for unit tests (mocking)
func (GetEnvVarImpl) Getenv(name string) string {
	return os.Getenv(name)
}

func setupLogLossTracker() {
	enabled = env.Getenv("CONTROLLER_TYPE") == "DaemonSet"

	enabled = enabled && !(strings.ToLower(env.Getenv("IN_UNIT_TEST")) == "true")

	// toggle env var is meant to be set by Microsoft, customer can set AZMON_ENABLE_LOG_LOSS_TRACKING through a configmap
	if env.Getenv("AZMON_ENABLE_LOG_LOSS_TRACKING_SET") == "true" {
		enabled = enabled && (env.Getenv("AZMON_ENABLE_LOG_LOSS_TRACKING") == "true")
	} else {
		enabled = enabled && (env.Getenv("AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE") == "true")
	}

	// don't count logs if stdout or stderr log collection is globally disabled
	enabled = enabled && strings.ToLower(env.Getenv("AZMON_COLLECT_STDOUT_LOGS")) == "true" && strings.ToLower(env.Getenv("AZMON_COLLECT_STDERR_LOGS")) == "true"

	if enabled {
		for _, excluded_namespace := range strings.Split(env.Getenv("AZMON_STDERR_EXCLUDED_NAMESPACES"), ",") {
			disabled_namespaces[excluded_namespace] = true
		}

		for _, excluded_namespace := range strings.Split(env.Getenv("AZMON_STDOUT_EXCLUDED_NAMESPACES"), ",") {
			disabled_namespaces[excluded_namespace] = true
		}
	}
}

func StartLogLossTracker() {

	// This is broken out into a separate function for unit-testability
	setupLogLossTracker()

	if enabled {
		log_loss_logger = createLogger("", "container-log-counts.log")

		write_counts_ticker := time.NewTicker(log_loss_telemetry_interval)
		go write_telemetry(write_counts_ticker.C)

		//TODO: turn this frequency down
		track_rotations_ticker := time.NewTicker(log_loss_track_rotations_interval)
		go track_log_rotations(track_rotations_ticker.C, "/var/log/pods")
	}
}

// TODO: when scale testing also measure disk usage (should measure cpu, mem, disk, and lost logs when scale testing)
// 			does running a separate process impact cpu/mem/disk/lost logs
func Process_log(containerID *string, k8sNamespace *string, k8sPodName *string, containerName *string, logEntry *string, logTime *string) {
	if enabled {
		identifier := *k8sNamespace + "_" + *k8sPodName + "_" + *containerName
		log_count_index, container_new := m_bytes_logged_storage.get(identifier)

		if container_new {
			// this branch only executes when a new container is seen, so it doesn't have to be as optimized
			process_logs_mut.Lock() // only grab this lock when a new container is seen. Slices are thread safe so we don't need to lock it
			defer process_logs_mut.Unlock()

			// do garbage collection
			if float32(atomic.LoadInt32(&num_containers_on_disk))*1.2 < float32(m_bytes_logged_storage.len()) {
				// send some containerIDs to the FW thread to check if they are deleted from disk
			write_to_chan:
				for i := 0; i < garbage_collection_agressiveness; i++ {
					m_deletion_query_index = (m_deletion_query_index + 1) % len(m_bytes_logged_storage.container_identifiers)

					if m_bytes_logged_storage.container_identifiers[m_deletion_query_index] != "" {
						// use select to write to the channel in a non-blocking way
						select {
						case deleted_containers_query <- m_bytes_logged_storage.container_identifiers[m_deletion_query_index]:
							continue
						default:
							break write_to_chan
						}
					}
				}
				// now delete any returned containers (this will probably run the next time a container is added)
			read_from_chan:
				for {
					select {
					case deleted_identifier := <-deleted_containers_response:
						m_bytes_logged_storage.delete(deleted_identifier)
					default:
						break read_from_chan
					}
				}
			}
		}
		log_bytes := len(*logTime) + len(" stdout f ") + len(*logEntry) + 1 // (an extra byte for the trailing \n in the source log file)

		// double check that the atomic read/write fixes any concurency issues? (it really should)
		atomic.AddInt64(log_count_index, int64(log_bytes))
	}
}

func write_telemetry(ticker <-chan time.Time) {
	// putting this code in a sub-function so that it can use defer
	inner_func := func() {
		identifiers, values := m_bytes_logged_storage.export_values()

		read_disk_mut.Lock()
		defer read_disk_mut.Unlock()

		var total_bytes_logged int64 = 0
		var total_bytes_on_disk int64 = 0

		for index, container_identifier := range identifiers {
			if container_identifier == "" {
				continue
			}
			logs_counted := values[index]

			record, container_seen_on_disk := FW_records[container_identifier]
			rotated_bytes := record.rotated_bytes
			unrotated_bytes := record.unrotated_bytes

			if !container_seen_on_disk {
				// this can happen for perfectly normal reasons (like the container is waiting to be garbage collected)
				continue
			}

			total_bytes_logged += logs_counted
			total_bytes_on_disk += rotated_bytes + unrotated_bytes
			log_loss_logger.Printf(`{"namespace_pod_container": "%s", "bytes_at_log_file": %d, "bytes_at_output_plugin": %d}`, container_identifier, rotated_bytes+unrotated_bytes, logs_counted)
		}
		// these variables live in telemetry.go
		LogsLostInFluentBit = total_bytes_on_disk - total_bytes_logged
		LogsWrittenToDisk = total_bytes_on_disk
	}

	for range ticker {
		inner_func()
	}
}

func track_log_rotations(ticker <-chan time.Time, watch_dir string) {
	inner_func := func() {
		read_disk_mut.Lock()
		defer read_disk_mut.Unlock()

		// this needs a way to return an error
		all_files_and_sizes := GetSizeOfAllFilesInDir(watch_dir)
		containers_seen_this_iter := make(map[string]bool)

		for filepath, file_size := range all_files_and_sizes {
			// we don't care about compressed log files
			if strings.HasSuffix(filepath, ".gz") {
				continue
			}

			filepath_parts := strings.Split(filepath, "/") // TODO: this will be different for windows

			//TODO: need some safety here
			if len(filepath_parts) != 3 {
				Log("illegal file found in pod log dir: %v", filepath)
				continue
			}
			pod_folder_parts := strings.Split(filepath_parts[0], "_") // this is safe because kubernetes objects can't have _ in their name
			namespace_folder := pod_folder_parts[0]
			pod_folder := pod_folder_parts[1]
			container_folder := filepath_parts[1]
			log_file_name := filepath_parts[2]
			container_identifier := namespace_folder + "_" + pod_folder + "_" + container_folder // should be of the format namespace_podname-garbage_containername

			containers_seen_this_iter[container_identifier] = true

			container_record, container_already_tracked := FW_records[container_identifier]
			if !container_already_tracked {
				container_record = FwRecord{existing_log_files: make([]string, 0), rotated_bytes: 0, unrotated_bytes: 0}
			}

			if log_file_name == "0.log" {
				container_record.unrotated_bytes = file_size
			} else if !slice_contains_str(container_record.existing_log_files, log_file_name) {
				Log(fmt.Sprintf("log file %s/%s, %s is new", pod_folder, container_folder, log_file_name))
				container_record.existing_log_files = append(container_record.existing_log_files, log_file_name)

				container_record.rotated_bytes += file_size

				if len(container_record.existing_log_files) > 4 { // I've never seen 4 rotated uncompressed log files kept around at once
					container_record.existing_log_files = container_record.existing_log_files[1:]
				}
			}

			FW_records[container_identifier] = container_record
		}

		// garbage collection: delete any containers which didn't have log files.
		for container_identifier := range FW_records {
			if _, container_seen := containers_seen_this_iter[container_identifier]; !container_seen {
				delete(FW_records, container_identifier)
			}
		}

	garbage_detection:
		for {
			// use select to write to the channel in a non-blocking way
			select {
			case container_identifier := <-deleted_containers_query:
				if _, container_exists := FW_records[container_identifier]; !container_exists {
					select {
					case deleted_containers_response <- container_identifier:
						continue
					default:
						continue
					}
				}
			default:
				break garbage_detection
			}
		}

		atomic.StoreInt32(&num_containers_on_disk, int32(len(FW_records)))
	}

	for range ticker {
		inner_func()
	}
}

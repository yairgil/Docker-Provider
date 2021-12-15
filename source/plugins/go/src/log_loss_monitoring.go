package main

//TODO: replace all panics with actuall error handling

import (
	"fmt"
	"log"
	"os"
	"strings"
	"sync"
	"sync/atomic"
	"time"
)

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
var m_container_to_arr_index = make(map[string]int)
var m_bytes_logged_storage = Make_QuickDeleteSlice()

// type container_pod_name struct {
// 	namespace_pod string
// 	name          string
// }
// var m_container_pod_name_to_id = make(map[container_pod_name]string)

var m_deletion_query_index = 0

// File Watcher managed data structures
// (FW = File Watcher)
var FW_existing_log_files = make(map[string][]string)  // identifier -> file names
var FW_bytes_logged_rotated = make(map[string]int64)   // identifier -> count
var FW_bytes_logged_unrotated = make(map[string]int64) // identifier -> count
// var current_iteration_count int64 = 0
var num_containers_on_disk int32 = 0

// Shared objects
var deleted_containers_query = make(chan string, 5)    // There might be a lot more than 5 containers  of 10 chosen arbitrairly
var deleted_containers_response = make(chan string, 5) // capacity of 10 chosen arbitrairly

var process_logs_mut = &sync.Mutex{}
var read_disk_mut = &sync.Mutex{}
var enabled bool

var disabled_namespaces = make(map[string]bool)

var log_loss_logger *log.Logger

func init() {
	enabled = os.Getenv("CONTROLLER_TYPE") == "DaemonSet"

	enabled = enabled && !(strings.ToLower(os.Getenv("IN_UNIT_TEST")) == "true")

	// toggle env var is meant to be set by Microsoft, customer can set AZMON_DISABLE_LOG_LOSS_TRACKING through a configmap
	enabled = enabled && (os.Getenv("AZMON_DISABLE_LOG_LOSS_TRACKING") != "true") && (os.Getenv("AZMON_DISABLE_LOG_LOSS_TRACKING_TOGGLE") != "true")

	// don't count logs if stdout or stderr log collection is globally disabled
	enabled = enabled && strings.ToLower(os.Getenv("AZMON_COLLECT_STDOUT_LOGS")) == "true" && strings.ToLower(os.Getenv("AZMON_COLLECT_STDERR_LOGS")) == "true"

	if enabled {
		log_loss_logger = createLogger("container-log-counts.log")

		for _, excluded_namespace := range strings.Split(os.Getenv("AZMON_STDERR_EXCLUDED_NAMESPACES"), ",") {
			disabled_namespaces[excluded_namespace] = true
		}

		for _, excluded_namespace := range strings.Split(os.Getenv("AZMON_STDOUT_EXCLUDED_NAMESPACES"), ",") {
			disabled_namespaces[excluded_namespace] = true
		}

		write_counts_ticker := time.NewTicker(5 * time.Minute)
		go write_telemetry(write_counts_ticker.C)

		//TODO: turn this frequency down
		track_rotations_ticker := time.NewTicker(10 * time.Second)
		go track_log_rotations(track_rotations_ticker.C, "/var/log/pods")
	}
}

// TODO: when scale testing also measure disk usage (should measure cpu, mem, disk, and lost logs when scale testing)
// 			does running a separate process impact cpu/mem/disk/lost logs
func Process_log(containerID *string, k8sNamespace *string, k8sPodName *string, containerName *string, logEntry *string, logTime *string) {
	if enabled {
		identifier := *k8sNamespace + "_" + *k8sPodName + "_" + *containerName
		log_count_index, container_old := m_container_to_arr_index[identifier]

		if !container_old {
			// this branch only executes when a new container is seen, so it doesn't have to be as optimized
			process_logs_mut.Lock() // only grab this lock when a new container is seen. Slices are thread safe so we don't need to lock it
			defer process_logs_mut.Unlock()

			log_count_index := m_bytes_logged_storage.insert_new_container(identifier)
			m_container_to_arr_index[identifier] = log_count_index

			// do garbage collection
			if float32(atomic.LoadInt32(&num_containers_on_disk)) > float32(len(m_container_to_arr_index))*1.2 {
				// send some containerIDs to the FW thread to check if they are deleted from disk
			write_to_chan:
				for i := 0; i < 5; i++ {
					deletion_query_index := (m_deletion_query_index + 1) % len(m_bytes_logged_storage.container_identifiers)

					// use select to write to the channel in a non-blocking way
					select {
					case deleted_containers_query <- m_bytes_logged_storage.container_identifiers[deletion_query_index]:
						continue
					default:
						break write_to_chan
					}
				}
				// now delete any returned containers (this will probably run the next time a container is added)
			read_from_chan:
				for {
					select {
					case deleted_id := <-deleted_containers_response:
						deleted_container_index := m_container_to_arr_index[deleted_id]
						m_bytes_logged_storage.remove_index(deleted_container_index)
						delete(m_container_to_arr_index, deleted_id)
					default:
						break read_from_chan
					}
				}
			}
		}
		log_bytes := len(*logTime) + len(" stdout f ") + len(*logEntry) + 1 // (an extra byte for the trailing \n in the source log file)

		// double check that the atomic read/write fixes any concurency issues? (it really should)
		atomic.AddInt64(&m_bytes_logged_storage.log_counts[log_count_index], int64(log_bytes))
	}
}

func write_telemetry(ticker <-chan time.Time) {
	// putting this code in a sub-function so that it can use defer
	inner_func := func() {
		var main_log_counts_copy []int64
		var main_container_identifiers_copy []string

		func() {
			m_bytes_logged_storage.management_mut.Lock()
			defer m_bytes_logged_storage.management_mut.Unlock()

			// log counts need to be copied atomically
			main_log_counts_copy = make([]int64, len(m_bytes_logged_storage.log_counts))
			for i := 0; i < len(m_bytes_logged_storage.log_counts); i++ {
				main_log_counts_copy[i] = atomic.LoadInt64(&m_bytes_logged_storage.log_counts[i])
			}

			// container identifiers do not need to be copied atomicaly. They don't change as long as m_bytes_logged_storage.management_mut is held
			main_container_identifiers_copy = m_bytes_logged_storage.container_identifiers
		}()

		read_disk_mut.Lock()
		defer read_disk_mut.Unlock()

		var total_bytes_logged int64 = 0
		var total_bytes_on_disk int64 = 0

		for log_count_index, container_identifier := range main_container_identifiers_copy {
			if container_identifier == "" {
				continue
			}
			logs_counted := main_log_counts_copy[log_count_index]

			unrotated_bytes, container_seen_on_disk := FW_bytes_logged_unrotated[container_identifier]
			rotated_bytes := FW_bytes_logged_rotated[container_identifier]

			if !container_seen_on_disk {
				// this can happen for perfectly normal reasons (like the container is waiting to be garbage collected)
				continue
			}

			total_bytes_logged += logs_counted
			total_bytes_on_disk += rotated_bytes + unrotated_bytes
			log_loss_logger.Printf(`{"namespace_pod_container": "%s", "bytes_at_log_file": %d, "bytes_at_output_plugin": %d}`, container_identifier, rotated_bytes+unrotated_bytes, logs_counted)

			// these variables live in telemetry.go
			LogsLostInFluentBit = total_bytes_on_disk - total_bytes_logged
			LogsWrittenToDisk = total_bytes_on_disk
		}
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
			pod_folder_parts := strings.Split(filepath_parts[0], "_")
			namespace_folder := pod_folder_parts[0]
			pod_folder := pod_folder_parts[1]
			container_folder := filepath_parts[1]
			log_file_name := filepath_parts[2]
			container_identifier := namespace_folder + "_" + pod_folder + "_" + container_folder // should be of the format namespace_podname-garbage_containername

			containers_seen_this_iter[container_identifier] = true

			_, container_already_tracked := FW_existing_log_files[container_identifier]
			if !container_already_tracked {
				FW_existing_log_files[container_identifier] = make([]string, 0)
				FW_bytes_logged_rotated[container_identifier] = 0
				FW_bytes_logged_unrotated[container_identifier] = 0
			}

			if log_file_name == "0.log" {
				FW_bytes_logged_unrotated[container_identifier] = file_size
			} else if !slice_contains(FW_existing_log_files[container_identifier], log_file_name) {
				Log(fmt.Sprintf("log file %s/%s, %s is new", pod_folder, container_folder, log_file_name))
				FW_existing_log_files[container_identifier] = append(FW_existing_log_files[container_identifier], log_file_name)

				FW_bytes_logged_rotated[container_identifier] += file_size

				if len(FW_existing_log_files[container_identifier]) > 4 { // I've never seen 4 rotated uncompressed log files kept around at once
					FW_existing_log_files[container_identifier] = FW_existing_log_files[container_identifier][1:]
				}
			}
		}

		// garbage collection: delete any containers which didn't have log files.
		for container_identifier, _ := range FW_bytes_logged_rotated {
			if _, container_seen := containers_seen_this_iter[container_identifier]; !container_seen {

				delete(FW_existing_log_files, container_identifier)
				delete(FW_bytes_logged_rotated, container_identifier)
				delete(FW_bytes_logged_unrotated, container_identifier)
			}
		}

	garbage_detection:
		for {
			// use select to write to the channel in a non-blocking way
			select {
			case container_identifier := <-deleted_containers_query:
				if _, container_exists := FW_bytes_logged_unrotated[container_identifier]; !container_exists {
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

		atomic.StoreInt32(&num_containers_on_disk, int32(len(FW_bytes_logged_unrotated)))
	}

	for range ticker {
		inner_func()
	}
}

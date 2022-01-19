package main

import (
	"io/ioutil"
	"log"
	"os"
	"path/filepath"
	"strings"
	"sync"
	"sync/atomic"
	"time"
)

const log_loss_telemetry_interval = 5 * time.Minute
const log_loss_track_rotations_interval = 5 * time.Second

// used exclusive by the main thread
var container_logs_current = make(map[string]int64)

// shared between the main thread and telemetry thread
var request_snapshot int32 = 0 // 0 = false, 1 = true. Must be int instead of float so we can do atomic operations on this
var snapshot_chan = make(chan map[string]int64, 1)

// used exclusively by the telemetry thread
var container_logs_cumulative = make(map[string]int64)

// shared between the telemetry thread and file watcher thread
// File Watcher managed data structures
// (FW = File Watcher)
type FwRecord struct {
	deleted_bytes        int64            // number of bytes in deleted log files (not rotated)
	existing_log_files   map[string]int64 // number of bytes in all existing log files (regardless of if they were rotated)
	last_generation_seen int64
}

var FW_records map[string]FwRecord
var disk_bytes_from_deleted_containers int64 = 0
var read_disk_mut sync.Mutex

var disabled_namespaces map[string]bool

// other global vars
var enable_log_loss_detection bool
var log_loss_logger *log.Logger

func init() {
	init_log_loss_monitoring_globals()
}

// This function exists for unit tests, it lets each test reset all global variables at the start of each test
func init_log_loss_monitoring_globals() {
	enable_log_loss_detection = false

	container_logs_current = make(map[string]int64)
	request_snapshot = 0
	container_logs_cumulative = make(map[string]int64)

	FW_records = make(map[string]FwRecord)
	read_disk_mut = sync.Mutex{}

	disabled_namespaces = make(map[string]bool)
}

// The next few type definitions are for unit testing (to mock os.Genenv())
//go:generate mockgen -source=log_loss_monitoring.go -destination=log_loss_monitoring_mock.go -package=main  IGetEnvVar
type IGetEnvVar interface {
	Getenv(key string) string
}
type GetEnvVarImpl struct{}

var env_mock IGetEnvVar = GetEnvVarImpl{}

// This is here for unit tests (mocking)
func (GetEnvVarImpl) Getenv(name string) string {
	return os.Getenv(name)
}

func setupLogLossTracker() {
	enabled := env_mock.Getenv("CONTROLLER_TYPE") == "DaemonSet"
	enabled = enabled && !(strings.ToLower(env_mock.Getenv("IN_UNIT_TEST")) == "true")
	enabled = enabled && !(strings.ToLower(env_mock.Getenv("CONTAINER_RUNTIME")) == "docker")

	// toggle env var is meant to be set by Microsoft, customer can set AZMON_ENABLE_LOG_LOSS_TRACKING through a configmap
	if env_mock.Getenv("AZMON_ENABLE_LOG_LOSS_TRACKING_SET") == "true" {
		enabled = enabled && (env_mock.Getenv("AZMON_ENABLE_LOG_LOSS_TRACKING") == "true")
	} else {
		enabled = enabled && (env_mock.Getenv("AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE") == "true")
	}

	// don't count logs if stdout or stderr log collection is globally disabled
	enabled = enabled && strings.ToLower(env_mock.Getenv("AZMON_COLLECT_STDOUT_LOGS")) == "true" && strings.ToLower(env_mock.Getenv("AZMON_COLLECT_STDERR_LOGS")) == "true"

	// remove this after adding windows support (which should very simple)
	if !is_linux() {
		enabled = false
	}

	if enabled {
		for _, excluded_namespace := range strings.Split(env_mock.Getenv("AZMON_STDERR_EXCLUDED_NAMESPACES"), ",") {
			disabled_namespaces[excluded_namespace] = true
		}

		for _, excluded_namespace := range strings.Split(env_mock.Getenv("AZMON_STDOUT_EXCLUDED_NAMESPACES"), ",") {
			disabled_namespaces[excluded_namespace] = true
		}
	}

	enable_log_loss_detection = enabled
}

func StartLogLossTracker() {

	// This is broken out into a separate function for unit-testability
	setupLogLossTracker()

	if enable_log_loss_detection {
		Log("log_loss_monitoring.go: starting")
		log_loss_logger = createLogger("", "container-log-counts-cumulative.log")

		write_counts_ticker := time.NewTicker(log_loss_telemetry_interval)
		go write_telemetry(write_counts_ticker.C)

		track_rotations_ticker := time.NewTicker(log_loss_track_rotations_interval)
		go track_log_rotations(track_rotations_ticker.C, "/var/log/pods")
	}
}

func Process_log(containerID *string, k8sNamespace *string, k8sPodName *string, containerName *string, logEntry *string, logTime *string) {
	if enable_log_loss_detection {
		identifier := *k8sNamespace + "_" + *k8sPodName + "_" + *containerName
		// log_count_index, container_new := m_bytes_logged_storage.get(identifier)
		log_count, container_already_exists := container_logs_current[identifier]
		if !container_already_exists {
			log_count = 0
		}
		log_bytes := len(*logTime) + len(" stdout f ") + len(*logEntry) + 1 // (an extra byte for the trailing \n in the source log file)

		container_logs_current[identifier] = log_count + int64(log_bytes)

		// Check if the telemetry thread is requesting the latest snapshot. If it is, give it this threads currently
		//  stored counts and create a new map to store the counts in. By handing off the old map to the telemetry
		// thread, we avoid any concurrency issues.
		// (The telemetry thread keeps track of all log counts since the beginning of time, this thread only keeps
		// 	track of the log counts since the telemetry thread last requested them)
		if atomic.LoadInt32(&request_snapshot) != 0 {
			select {
			case snapshot_chan <- container_logs_current:
				container_logs_current = make(map[string]int64)
				atomic.StoreInt32(&request_snapshot, 0)
			default:
			}
		}
	}
}

func write_telemetry(ticker <-chan time.Time) {
	// putting this code in a sub-function so that it can use defer
	inner_func := func() {
		// identifiers, values := m_bytes_logged_storage.export_values()

		update_state_from_snapshot := func(snapshot map[string]int64) {
			for container, new_count := range snapshot {
				old_count, exists := container_logs_cumulative[container]
				if !exists {
					old_count = 0
				}
				container_logs_cumulative[container] = new_count + old_count
			}
		}

		// Make sure there was no previous snapshot already in the snapshot channel. If we don't do this before
		// requesting a new one and an extra snapshot is in the channel then this thread would always be 5
		// minutes behind the real data.
		select {
		case incoming_snapshot := <-snapshot_chan:
			update_state_from_snapshot(incoming_snapshot)
		default:
		}

		// request a new snapshot. This flag signals the main thread that it should send its current snapshot
		atomic.StoreInt32(&request_snapshot, 1)

		// don't wait forever for a new snapshot because they are only sent when data is being logged.
		select {
		case incoming_snapshot := <-snapshot_chan:
			update_state_from_snapshot(incoming_snapshot)
		case <-get_timeout_chan(time.Second * 20):
		}

		read_disk_mut.Lock()
		defer read_disk_mut.Unlock()

		var total_bytes_logged int64 = 0
		var total_bytes_on_disk int64 = disk_bytes_from_deleted_containers // account for containers which were deleted on disk

		deleted_containers := make([]string, 0)

		for container_identifier, container_log_count := range container_logs_cumulative {
			record, container_seen_on_disk := FW_records[container_identifier]
			if !container_seen_on_disk {
				deleted_containers = append(deleted_containers, container_identifier)
			}

			disk_deleted_bytes := record.deleted_bytes
			var disk_undeleted_bytes int64 = sum_undeleted_bytes(record.existing_log_files)

			total_bytes_logged += container_log_count
			total_bytes_on_disk += disk_undeleted_bytes + disk_deleted_bytes
			log_loss_logger.Printf(`{"namespace_pod_container": "%s", "bytes_at_log_file": %d, "bytes_at_output_plugin": %d}`, container_identifier, disk_undeleted_bytes+disk_deleted_bytes, container_log_count)
		}
		// these variables live in telemetry.go
		LogsLostInFluentBit = total_bytes_on_disk - total_bytes_logged
		LogsWrittenToDisk = total_bytes_on_disk

		for _, container_identifier := range deleted_containers {
			delete(container_logs_cumulative, container_identifier)
		}
	}

	for range ticker {
		if !enable_log_loss_detection {
			return
		}
		inner_func()
	}
}

func sum_undeleted_bytes(log_files map[string]int64) int64 {
	var unrotated_bytes int64 = 0
	for _, val := range log_files {
		unrotated_bytes += val
	}
	return unrotated_bytes
}

func track_log_rotations(ticker <-chan time.Time, watch_dir string) {
	track_log_rotations_impl(ticker, watch_dir, 0)
}

// This function is broken out for unit testing
func track_log_rotations_impl(ticker <-chan time.Time, watch_dir string, current_generation int64) {
	inner_func := func() {
		read_disk_mut.Lock()
		defer read_disk_mut.Unlock()

		// this needs a way to return an error
		all_files_and_sizes, err := GetAllContainerLogFilesAndSizes(watch_dir)
		if err != nil {
			// turn off log loss detection if there's an error, don't attempt to recover (maybe change this in the future)
			enable_log_loss_detection = false
			return
		}
		// containers_seen_this_iter := make(map[string]bool)

		for filepath, current_container_log_files := range all_files_and_sizes {
			// we don't care about compressed log files

			pod_folder_parts := strings.Split(filepath.pod_folder, "_") // this is safe because kubernetes objects can't have _ in their name
			namespace_folder := pod_folder_parts[0]
			pod_folder := pod_folder_parts[1]
			container_folder := filepath.container_folder
			container_identifier := namespace_folder + "_" + pod_folder + "_" + container_folder // should be of the format namespace_podname-garbage_containername

			// containers_seen_this_iter[container_identifier] = true

			container_record, container_already_tracked := FW_records[container_identifier]
			if !container_already_tracked {
				container_record = FwRecord{existing_log_files: make(map[string]int64), deleted_bytes: 0, last_generation_seen: current_generation}
			}

			// if the previous list of container log files (container_record.existing_log_files) has any log files which are not on disk, then count them as deleted.
			// Then set the previous list of log files to the current list.
			for old_log_file, old_byte_count := range container_record.existing_log_files {
				if _, file_still_exists := current_container_log_files[old_log_file]; !file_still_exists {
					container_record.deleted_bytes += old_byte_count
				}
			}
			container_record.existing_log_files = current_container_log_files

			container_record.last_generation_seen = current_generation
			FW_records[container_identifier] = container_record
		}

		// garbage collection: delete any containers which didn't have log files.
		for container_identifier, container_record := range FW_records {
			if container_record.last_generation_seen != current_generation {
				// keep a count of bytes from deleted containers
				disk_bytes_from_deleted_containers += sum_undeleted_bytes(container_record.existing_log_files)
				disk_bytes_from_deleted_containers += container_record.deleted_bytes
				delete(FW_records, container_identifier)
			}
		}
	}

	for range ticker {
		inner_func()
		current_generation += 1
		if !enable_log_loss_detection {
			return
		}
	}
}

type pod_and_container_folders struct {
	pod_folder       string
	container_folder string
}

func GetAllContainerLogFilesAndSizes(root_dir string) (map[pod_and_container_folders]map[string]int64, error) {
	output_map := make(map[pod_and_container_folders]map[string]int64)

	pod_folders, err := ioutil.ReadDir(root_dir)
	if err != nil {
		Log("ERROR: reading pod dir " + err.Error())
		return nil, err
	}
	for _, pod_folder := range pod_folders {
		if pod_folder.IsDir() {
			//single pod
			container_folders, err := ioutil.ReadDir(filepath.Join(root_dir, pod_folder.Name()))
			if err != nil {
				Log("ERROR: reading container dir " + err.Error())
				return nil, err
			}
			for _, container_folder := range container_folders {
				if container_folder.IsDir() {
					// single container

					container_logs := make(map[string]int64)
					log_files, err := ioutil.ReadDir(filepath.Join(root_dir, pod_folder.Name(), container_folder.Name()))
					if err != nil {
						Log("ERROR: reading container dir " + err.Error())
						return nil, err
					}
					for _, log_file := range log_files {
						if log_file.IsDir() {
							continue
						}
						if strings.HasSuffix(log_file.Name(), ".gz") {
							continue // we don't care about compressed log files
						}
						container_logs[log_file.Name()] = log_file.Size()
					}

					output_map[pod_and_container_folders{pod_folder: pod_folder.Name(), container_folder: container_folder.Name()}] = container_logs
				}
			}
		}
	}
	return output_map, nil
}

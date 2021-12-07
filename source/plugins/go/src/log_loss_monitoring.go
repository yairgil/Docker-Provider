package main

//TODO: replace all panics with actuall error handling

import (
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
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
var container_to_arr_index = make(map[string]int)
var bytes_logged_storage = Make_QuickDeleteSlice()
var container_id_to_pod_folder = make(map[string]string)
var container_id_to_name = make(map[string]string)

type container_pod_name struct {
	namespace_pod string
	name          string
}
var container_pod_name_to_id = make(map[container_pod_name]string)

var deletion_query_index = 0



// File Watcher managed data structures
// (FW = File Watcher)
var FW_existing_log_files = make(map[string][]string)  // containerid -> file names
var FW_bytes_logged_rotated = make(map[string]int64)   // containerid -> count
var FW_bytes_logged_unrotated = make(map[string]int64) // containerid -> count
// var FW_container_last_seen = make(map[string]int64)    // containerid -> sequence_number
// var current_iteration_count int64 = 0
var num_containers_on_disk = 0


// Shared objects
var deleted_containers_query = make(chan string, 5)  // There might be a lot more than 5 containers  of 10 chosen arbitrairly
var deleted_containers_response = make(chan string, 5)  // capacity of 10 chosen arbitrairly


var process_logs_mut = &sync.Mutex{}
var read_disk_mut = &sync.Mutex{}
var enabled bool
var last_telem_sent_time = time.Date(1970, time.January, 1, 0, 0, 0, 0, time.UTC)

func init() {
	enabled = os.Getenv("CONTROLLER_TYPE") == "DaemonSet"
	enabled = enabled && (os.Getenv("DISABLE_LOG_TRACKING") != "true")

	if enabled {
		write_counts_ticker := time.NewTicker(5 * time.Minute)
		go write_telemetry(write_counts_ticker)

		//TODO: turn this frequency down
		track_rotations_ticker := time.NewTicker(10 * time.Second)
		go track_log_rotations(track_rotations_ticker)
	}
}

// TODO: when scale testing also measure disk usage (should measure cpu, mem, disk, and lost logs when scale testing)
// 			does running a separate process impact cpu/mem/disk/lost logs
func Process_log_batch(containerID *string, k8sNamespace *string, k8sPodName *string, containerName *string, logEntry *string, logTime *string) {
	if enabled {
		log_count_index, container_old := container_to_arr_index[*containerID]

		if !container_old {
			// this branch only executes when a new container is seen, so it doesn't have to be very optimized
			process_logs_mut.Lock() // only grab this lock when a new container is seen. Slices are thread safe so we don't need to lock it
			defer process_logs_mut.Unlock()

			log_count_index := bytes_logged_storage.get_free_index()
			container_to_arr_index[*containerID] = log_count_index

			container_id_to_pod_folder[*containerID] = *k8sNamespace + "_" + *k8sPodName
			container_id_to_name[*containerID] = *containerName
			container_pod_name_to_id[container_pod_name{namespace_pod: *k8sNamespace + "_" + *k8sPodName, name: *containerName}] = *containerID

			// do garbage collection
			// TODO: explain why garbage collection is done when a new container is added
			if atomic.LoadInt32(&num_containers_on_disk) > len(container_to_arr_index) * 1.2 {
				// send some containerIDs to the FW thread to check if they are deleted from disk
				for i := 0; i < 5; i++ {
					deletion_query_index = (deletion_query_index + 1) % len(bytes_logged_storage.container_id)

					// use select to write to the channel in a non-blocking way
					select {
					case deleted_containers_query <- bytes_logged_storage.container_id[deletion_query_index]:
						continue
					default:
						break
					}
				}

				// now delete any returned containers (this will probably run the next time a container is added)
				for ;; {
					select {
					case deleted_id := <- deleted_containers_response:
						deleted_container_index := container_to_arr_index[*deleted_id]
						bytes_logged_storage.remove_index(deleted_container_index)

						delete(container_to_arr_index[*deleted_id])
						delete(container_id_to_pod_folder[*deleted_id])
						delete(container_id_to_name[*deleted_id])
					default:
						break
					}
				}
			}
		}
		log_bytes := len(*logTime) + len(" stdout f ") + len(*logEntry) + 1 // (an extra byte for the trailing \n in the source log file)

		// this should be fine, but maybe double check that the atomic read/write fixes any concurency issues?
		atomic.AddInt64(&bytes_logged_storage.log_counts[log_count_index], (int64) log_bytes)
	}
}

func write_telemetry(ticker *time.Ticker) {
	// Default:

	// every 10 minutes per node: single new metric (one value is metric "value", other value in CustomDimensions)
	// Total number of bytes tried to log (file size) and total number of bytes in plugin

	// Enableable by default every 5 minutes:
	// - Put by-container data in a log fine inside the container (not telemetry)
	// 	- Container foobar: 10% logs lost

	// - send values in raw bytes (don't report megabytes or anything like that)
	//
	//

	// putting this code in a sub-function so that it can use defer
	inner_func := func() {
		file, err := os.OpenFile("/dev/write-to-traces", os.O_APPEND|os.O_WRONLY, 0644)
		if err != nil {
			Log("Error opening /dev/write-to-traces", err.Error())
		}
		defer file.Close()

		process_logs_mut.Lock() // TODO: this lock might be held for too long. Maybe to the file operations, then lock and compare to saved data?
		defer process_logs_mut.Unlock()
		read_disk_mut.Lock()
		defer read_disk_mut.Unlock()

		for containerID, log_count_index := range container_to_arr_index {
			logs_counted := atomic.LoadInt64(&bytes_logged_storage[log_count_index])

			filename_header := container_id_to_pod_folder[containerID]
			container_name := container_id_to_name[containerID]
			filesystem_bytes := FW_bytes_logged_rotated[containerID] + FW_bytes_logged_unrotated[containerID]
			// TODO: move file IO out of this loop? (because the mutex is held during this loop, so make it as fast as possible)
			_, err := file.WriteString(get_CRI_header() + fmt.Sprintf(`{"filename_header": "%s", "container_name": "%s", "bytes_at_output_plugin": %d, "bytes_at_log_file": %d}`, filename_header, container_name, logs_counted, filesystem_bytes) + "\n")
			if err != nil {
				Log("error writing to traces:", err.Error())
			}
		}
	}

	for range ticker.C {
		inner_func()
	}
}

func get_CRI_header() string {
	return time.Now().Format(time.RFC3339Nano) + " stdout F "
}

func track_log_rotations(ticker *time.Ticker) {
	inner_func := func() {
		read_disk_mut.Lock()
		defer read_disk_mut.Unlock()

		// this needs a way to return an error
		all_files_and_sizes := GetSizeOfAllFilesInDir("/var/log/pods")
		containers_seen_this_iter := make(map[string]bool)

		for filepath := range all_files_and_sizes {
			// we don't care about compressed log files
			if ! strings.Contains(filepath, ".gz") {
				continue
			}

			filepath_parts := filepath.Split(filepath, "/")  // TODO: this will be different for windows

			//TODO: need some safety here
			pod_folder := filepath_parts[4]
			container_folder := folderpath_parts[5]
			log_file_name := folderpath_parts[6]

			container_id, container_seen := log_folder_to_container_id(pod_folder, container_folder)
			containers_seen_this_iter[container_id] = true


			_, container_already_tracked := FW_most_recent_log_file[container_id]
			if !container_already_tracked {
				FW_most_recent_log_file[container_id] = "0.log.19700101-000001"  // Highly unlikely this code will be run before the unix epoch
				FW_bytes_logged_rotated[container_id] = 0
				FW_bytes_logged_unrotated[container_id] = 0
			}

			if log_file_name == "0.log" {
				current_log_file, err := os.Stat(filepath.Join(container_full_path, logfilename.Name()))
				if err != nil {
					Log("ERROR: getting file statistics" + err.Error())
				}
				FW_bytes_logged_unrotated[container_id] = current_log_file.Size()
			} else if !contains(FW_existing_log_files[container_id], log_file_name) {
				Log(fmt.Sprintf("log file %s, %s is new", container_full_path, rotated_file_name))
				FW_existing_log_files[container_id] = append(FW_existing_log_files[container_id], rotated_file_name)

				fi, err := os.Stat(filepath.Join(container_full_path, rotated_file_name))
				if err != nil {
					Log("ERROR: getting file statistics" + err.Error())
				}
				FW_bytes_logged_rotated[container_id] += fi.Size()

				if len(FW_existing_log_files[container_id]) > 6 { // I've never seen 6 unrotated log files kept around at once
					FW_existing_log_files[container_id] = FW_existing_log_files[container_id][1:]
				}
			}
		}

		// garbage collection: delete any containers which didn't have log files.
		deletion_list := make([]string, 0)
		for _, containerId := range container_to_arr_index {
			if _, container_seen = containers_seen_this_iter[containerId]; !container_seen {
				deletion_list = append(deletion_list, containerId)

				delete(FW_existing_log_files, containerId)
				delete(FW_bytes_logged_rotated, containerId)
				delete(FW_bytes_logged_unrotated, containerId)
			}
		}

		for ;; {
			// use select to write to the channel in a non-blocking way
			select {
			case test_container_id := <- deleted_containers_query
				if _, container_exists := FW_bytes_logged_rotated[test_container_id]; !container_exists {
					select {
					case deleted_containers_response <- test_contest_container_id:
						continue
					default:
						continue
					}
				}
			default:
				break
			}
		}

		atomic.StoreInt32(&num_containers_on_disk, len(FW_existing_log_files))
	}

	for range ticker.C {
		inner_func()
	}
}



func log_folder_to_container_id(folder_name string, container_name string) (string, bool) {
	parts := strings.Split(folder_name, "_")
	namespace_and_pod_name := strings.Join(parts[:2], "_")
	key := container_pod_name{namespace_pod: namespace_and_pod_name, name: container_name}
	id, exists := container_pod_name_to_id[key]
	return id, exists
}

func contains(str_slice []string, target_str string) bool {
	for _, val := range str_slice {
		if val == target_str {
			return true
		}
	}
	return false
}

package main

//TODO: replace all panics with actuall error handling

import (
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"strings"
	"sync"
	"time"
)

//TODO: garbage collection (if pods are being frequently recycled then these two maps will grow without bound)
var logged_bytes_by_container = make(map[string]int64)
var container_id_to_pod_folder = make(map[string]string)
var container_id_to_name = make(map[string]string)

type container_pod_name struct {
	namespace_pod string
	name          string
}

var container_pod_name_to_id = make(map[container_pod_name]string)

//TODO: replace with thread safe maps
// (FW = File Watcher)
var FW_existing_log_files = make(map[string][]string)  // containerid -> file names
var FW_bytes_logged_rotated = make(map[string]int64)   // containerid -> count
var FW_bytes_logged_unrotated = make(map[string]int64) // containerid -> count
var FW_container_last_seen = make(map[string]int64)    // containerid -> sequence_number
var current_iteration_count int64 = 0

var process_logs_mut = &sync.Mutex{}

var read_disk_mut = &sync.Mutex{}

var enabled bool

func init() {
	// if os.Getenv("ENABLE_PROFILING") == "true" {
	// 	// TODO: remove this when done profiling
	// 	prof := profile.Start()
	// 	go func() {
	// 		time.Sleep(time.Second * 60 * 10)
	// 		prof.Stop()
	// 	}()
	// }

	enabled = os.Getenv("CONTROLLER_TYPE") == "DaemonSet"
	enabled = enabled && (os.Getenv("DISABLE_LOG_TRACKING") != "true")

	if enabled {
		enabled = true

		write_counts_ticker := time.NewTicker(10 * time.Second)
		go write_counts_to_traces(write_counts_ticker)

		track_rotations_ticker := time.NewTicker(10 * time.Second)
		go track_log_rotations(track_rotations_ticker)
	}
}

// TODO: can these args be passed by reference?
// TODO: is strlen O(1) op?
// TODO: when scale testing also measure disk usage (should measure cpu, mem, disk, and lost logs when scale testing)
// 			does running a separate process impact cpu/mem/disk/lost logs
func Process_log_batch(containerID *string, k8sNamespace *string, k8sPodName *string, containerName *string, logEntry *string, logTime *string) {
	if enabled {
		process_logs_mut.Lock()
		defer process_logs_mut.Unlock()

		_, container_old := logged_bytes_by_container[*containerID]
		if !container_old {
			logged_bytes_by_container[*containerID] = 0
		}

		logged_bytes_by_container[*containerID] += (int64)(len(*logTime)+len(" stdout f ")+len(*logEntry)) + 1 // (an extra byte for the trailing \n in the source log file)
		if _, container_seen_before := container_id_to_pod_folder[*containerID]; !container_seen_before {
			container_id_to_pod_folder[*containerID] = *k8sNamespace + "_" + *k8sPodName
			container_id_to_name[*containerID] = *containerName

			container_pod_name_to_id[container_pod_name{namespace_pod: *k8sNamespace + "_" + *k8sPodName, name: *containerName}] = *containerID
		}
	}
}

func write_counts_to_traces(ticker *time.Ticker) {
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

		for containerID, logs_counted := range logged_bytes_by_container {

			// skip containers which have been stopped
			// TODO: actually delete stale containers from all the different maps
			if iter_count, seen := FW_container_last_seen[containerID]; seen && iter_count >= current_iteration_count-2 {
				filename_header := container_id_to_pod_folder[containerID]
				container_name := container_id_to_name[containerID]
				filesystem_bytes := FW_bytes_logged_rotated[containerID] + FW_bytes_logged_unrotated[containerID]
				_, err := file.WriteString(get_CRI_header() + fmt.Sprintf(`{"filename_header": "%s", "container_name": "%s", "log_bytes_counted": %d, "log_bytes_fs": %d}`, filename_header, container_name, logs_counted, filesystem_bytes) + "\n")
				if err != nil {
					Log("error writing to traces:", err.Error())
				}
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

// // iotuil's single-line function for writing text to a file doesn't append
// func create_or_append_to_file(folder string, filename string, text string) error {
// 	_, err := os.Stat(folder)
// 	if os.IsNotExist(err) {
// 		err = os.Mkdir(folder, 0755)
// 		if err != nil {
// 			Log(fmt.Sprintf("log-loss-counter: Error creating folder: %s", err.Error()))
// 			return err
// 		}
// 	}

// 	f, err := os.OpenFile(filepath.Join(folder, filename), os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
// 	if err != nil {
// 		Log(fmt.Sprintf("log-loss-counter: Error opening file for tracked container: %s", err.Error()))
// 		return err
// 	}
// 	defer f.Close()
// 	if _, err := f.WriteString(text); err != nil {
// 		Log(fmt.Sprintf("log-loss-counter: Error writing to file for tracked container: %s", err.Error()))
// 		return err
// 	}
// 	return nil
// }

func track_log_rotations(ticker *time.Ticker) {
	inner_func := func() {
		read_disk_mut.Lock()
		defer read_disk_mut.Unlock()

		pod_folders, err := ioutil.ReadDir("/var/log/pods")
		if err != nil {
			Log("ERROR: reading dir /var/log/pods: " + err.Error())
		}
		for _, pod_folder := range pod_folders {
			container_folders, err := ioutil.ReadDir(filepath.Join("/var/log/pods", pod_folder.Name()))
			if err != nil {
				Log("ERROR: reading dir" + err.Error())
				continue
			}
			for _, container_folder := range container_folders {
				container_id, container_seen := log_folder_to_container_id(pod_folder.Name(), container_folder.Name())
				FW_container_last_seen[container_id] = current_iteration_count
				if !container_seen {
					continue
				}
				container_full_path := filepath.Join("/var/log/pods", pod_folder.Name(), container_folder.Name())
				var rotated_file_names []string
				log_files, err := ioutil.ReadDir(container_full_path)
				if err != nil {
					Log("ERROR: reading dir " + err.Error())
				}
				for _, logfilename := range log_files {
					if !strings.Contains(logfilename.Name(), ".gz") && logfilename.Name() != "0.log" {
						rotated_file_names = append(rotated_file_names, logfilename.Name())
					}
					if logfilename.Name() == "0.log" {
						current_log_file, err := os.Stat(filepath.Join(container_full_path, logfilename.Name()))
						if err != nil {
							Log("ERROR: getting file statistics" + err.Error())
						}
						FW_bytes_logged_unrotated[container_id] = current_log_file.Size()
					}
				}

				_, container_already_tracked := FW_existing_log_files[container_id]
				if !container_already_tracked {
					FW_existing_log_files[container_id] = make([]string, 0)
					FW_bytes_logged_rotated[container_id] = 0
					FW_bytes_logged_unrotated[container_id] = 0
				}

				for _, rotated_file_name := range rotated_file_names {
					if !contains(FW_existing_log_files[container_id], rotated_file_name) {
						Log(fmt.Sprintf("log file %s, %s is new", container_full_path, rotated_file_name))
						FW_existing_log_files[container_id] = append(FW_existing_log_files[container_id], rotated_file_name)

						fi, err := os.Stat(filepath.Join(container_full_path, rotated_file_name))
						if err != nil {
							Log("ERROR: getting file statistics" + err.Error())
						}
						FW_bytes_logged_rotated[container_id] += fi.Size()
					}
				}
				if len(FW_existing_log_files[container_id]) > 8 { // I've never seen 8 unrotated log files kept around at once
					FW_existing_log_files[container_id] = FW_existing_log_files[container_id][1:]
				}
			}
		}
	}

	for range ticker.C {
		current_iteration_count += 1
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

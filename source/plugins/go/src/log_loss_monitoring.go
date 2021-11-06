package main

//TODO: replace all panics with actuall error handling

import (
	"fmt"
	"math/rand"
	"os"
	"path/filepath"
	"time"
)

//TODO: thread safe
var logs_by_container map[string]int
var log_file_names map[string]string
var stop_monitoring_time map[string]time.Time
var start_monitoring_time map[string]time.Time

//TODO: need to clean old containers out of this map. otherwise it will grow forever
var all_containers map[string]bool // this map is being used as a set. bool is the value type because it's the smallest type

const target_ratio = 1.0
const monitoring_time_seconds time.Duration = time.Second * 30
const extra_wait time.Duration = time.Second * 10

var followed_containers int64 = 0

//TODO: thread safe
var latest_seen_time time.Time = time.Date(1970, 0, 0, 0, 0, 0, 0, time.UTC)

func init() {
	logs_by_container = make(map[string]int)
	log_file_names = make(map[string]string)
	stop_monitoring_time = make(map[string]time.Time)
	start_monitoring_time = make(map[string]time.Time)
	all_containers = make(map[string]bool)
}

// TODO: can these args be passed by reference?
func Process_log_batch(containerID string, k8sNamespace string, k8sPodName string, containerName string, logEntry string, logTime string) {
	container_tracked, container_seen := all_containers[containerID]
	if !container_seen {
		Log("seeing new container")
		//TODO: decide if we should track this container
		if rand.Float32() <= target_ratio {
			all_containers[containerID] = true
			container_tracked = true
			// log_path_by_container[*containerID] = struct {
			// 	path_a string
			// 	path_b string
			// }{*k8sNamespace + "_" + *k8sPodName, *containerName}
			// kube-system_omsagent-qz74z_f5f369a7-4094-4f58-bdb9-23c35121e491

			log_time_parsed, err := time.Parse(time.RFC3339Nano, logTime)
			if err != nil {
				// send the error to telemetry
				fmt.Printf("Invalid time format seen in log loss monitoring: %s", logTime)
			} else {
				// last_log_time[*containerID] = parsed_time
			}

			log_file_name := k8sPodName + "_" + fmt.Sprint(followed_containers)

			logs_by_container[containerID] = 0
			start_monitoring_time[containerID] = log_time_parsed
			stop_monitoring_time[containerID] = start_monitoring_time[containerID].Add(monitoring_time_seconds)
			log_file_names[containerID] = log_file_name

			message := fmt.Sprintf("choosing to monitor new container %s, will be monitored from %s to %s", containerID, start_monitoring_time[containerID].Format(time.RFC3339Nano), stop_monitoring_time[containerID].Format(time.RFC3339Nano))
			Log(message)

			followed_containers += 1
			go monitor_container(containerID, start_monitoring_time[containerID], stop_monitoring_time[containerID], log_file_name)

		} else {
			all_containers[containerID] = false
			container_tracked = false
		}
	}

	parsed_time, err := time.Parse(time.RFC3339, logTime)
	if err != nil {
		// send the error to telemetry
		fmt.Printf("Invalid time format seen in log loss monitoring: %s", logTime)
	}

	//TODO: not sure if we need to check that this is after the log start time, does fbit deliver messages in-order?
	var not_too_early = !parsed_time.Before(start_monitoring_time[containerID])
	var not_too_late = parsed_time.Before(stop_monitoring_time[containerID])
	if container_tracked && not_too_early && not_too_late {
		logs_by_container[containerID] += 1
		Log(fmt.Sprintf(" ******** counting log line from container %s, log line is: %s  (diagnostics: container_tracked: %v, parsed_time: %s, !parsed_time.Before(start_monitoring_time[containerID]): %v, parsed_time.Before(stop_monitoring_time[containerID]): %v, log_file_names[containerID]: %s, time.Now(): %s", containerID, logEntry, container_tracked, logTime, not_too_early, not_too_late, log_file_names[containerID], time.Now().Format(time.RFC3339Nano)))
	} else {
		Log(fmt.Sprintf(" ******** NOT counting log line from container %s, log line is: %s  (diagnostics: container_tracked: %v, parsed_time: %s, !parsed_time.Before(start_monitoring_time[containerID]): %v, parsed_time.Before(stop_monitoring_time[containerID]): %v, log_file_names[containerID]: %s, time.Now(): %s", containerID, logEntry, container_tracked, logTime, not_too_early, not_too_late, log_file_names[containerID], time.Now().Format(time.RFC3339Nano)))
	}

	if latest_seen_time.Before(parsed_time) {
		latest_seen_time = parsed_time
	}
}

func monitor_container(containerID string, start_time time.Time, end_time time.Time, message_file_name string) {

	Log(fmt.Sprintf("in monitor_container(%s, %s, %s, %s) at time %s", containerID, start_time.Format(time.RFC3339Nano), end_time.Format(time.RFC3339Nano), message_file_name, time.Now().Format(time.RFC3339Nano)))

	start_monitoring_time_text, err := start_time.MarshalText()
	if err != nil {
		Log(fmt.Sprintf("log-loss-counter: Error converting time to text? (how is this possible?): %s", err.Error()))
		return
	}

	stop_monitoring_time_text, err := end_time.MarshalText()
	if err != nil {
		Log(fmt.Sprintf("log-loss-counter: Error converting time to text? (how is this possible?): %s", err.Error()))
		return
	}

	output_str := fmt.Sprintf("{\"ContainerID\": \"%s\", \"StartTime\": \"%s\", \"EndTime\": \"%s\"}\n", containerID, start_monitoring_time_text, stop_monitoring_time_text)
	err = create_or_append_to_file("/var/log_counts", message_file_name, output_str)
	if err != nil {
		Log(fmt.Sprintf("log-loss-counter: Error creating file for %s", err.Error()))
		return
	}

	// wait for the specified duration to be over
	wait_end_time := end_time.Add(extra_wait) // wait a few extra seconds for any logs logged right at the time cutoff to be read
	Log(fmt.Sprintf("about to wait for logs past time %s (time.Now(): %s)", wait_end_time.Format(time.RFC3339Nano), time.Now().Format(time.RFC3339Nano)))
	for latest_seen_time.Before(wait_end_time) {
		time.Sleep(time.Second * 2) // 1 secondd chosen arbitrairly
	}
	Log("done sleeping, last seen time is %s (time.Now(): %s)", latest_seen_time.Format(time.RFC3339Nano), time.Now().Format(time.RFC3339Nano))

	// write to file to signify that monitoring is done and close the file. It will be the log_line_counter's responsibility to delete the file
	end_log_count := logs_by_container[containerID]
	output_str = fmt.Sprintf("{\"Final_log_count\": %d}\n", end_log_count)
	err = create_or_append_to_file("/var/log_counts", message_file_name, output_str)
	if err != nil {
		Log(fmt.Sprintf("log-loss-counter: Error creating file for container %s, error is %s", containerID, err.Error()))
	}

	Log(fmt.Sprintf("done monitoring container %s, final log count: %d", containerID, end_log_count))

	// allow monitoring this container again
	//TODO: go maps are not thread safe (how is that possible? threading is golang's main feature)
	delete(logs_by_container, containerID)
	delete(stop_monitoring_time, containerID)
	delete(start_monitoring_time, containerID)
	delete(all_containers, containerID)
}

// iotuil's single-line function for writing text to a file doesn't append
func create_or_append_to_file(folder string, filename string, text string) error {
	_, err := os.Stat(folder)
	if os.IsNotExist(err) {
		err = os.Mkdir(folder, 0755)
		if err != nil {
			Log(fmt.Sprintf("log-loss-counter: Error creating folder: %s", err.Error()))
			return err
		}
	}

	f, err := os.OpenFile(filepath.Join(folder, filename), os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		Log(fmt.Sprintf("log-loss-counter: Error opening file for tracked container: %s", err.Error()))
		return err
	}
	defer f.Close()
	if _, err := f.WriteString(text); err != nil {
		Log(fmt.Sprintf("log-loss-counter: Error writing to file for tracked container: %s", err.Error()))
		return err
	}
	return nil
}

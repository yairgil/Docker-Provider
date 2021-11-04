package main

//TODO: replace all panics with actuall error handling

import (
	"fmt"
	"math/rand"
	"os"
	"path/filepath"
	"time"
)

var logs_by_container map[string]int

var stop_monitoring_time map[string]time.Time
var start_monitoring_time map[string]time.Time

//TODO: need to clean old containers out of this map. otherwise it will grow forever
var all_containers map[string]bool // this map is being used as a set. bool is the value type because it's the smallest type

const target_ratio = 1.0

var monitoring_time_seconds time.Duration

var extra_wait time.Duration

func init() {
	extra_wait, _ = time.ParseDuration("10s")
	monitoring_time_seconds, _ = time.ParseDuration("30s")

	logs_by_container = make(map[string]int)
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
			message := fmt.Sprintf("choosing to monitor new container %s", containerID)
			Log(message)
			all_containers[containerID] = true
			container_tracked = true
			// log_path_by_container[*containerID] = struct {
			// 	path_a string
			// 	path_b string
			// }{*k8sNamespace + "_" + *k8sPodName, *containerName}
			// kube-system_omsagent-qz74z_f5f369a7-4094-4f58-bdb9-23c35121e491

			log_time_parsed, err := time.Parse(time.RFC3339, logTime)
			if err != nil {
				// send the error to telemetry
				fmt.Printf("Invalid time format seen in log loss monitoring: %s", logTime)
			} else {
				// last_log_time[*containerID] = parsed_time
			}

			logs_by_container[containerID] = 0
			start_monitoring_time[containerID] = log_time_parsed
			stop_monitoring_time[containerID] = start_monitoring_time[containerID].Add(monitoring_time_seconds)

			go monitor_container(containerID, start_monitoring_time[containerID], stop_monitoring_time[containerID])

		} else {
			all_containers[containerID] = false
			container_tracked = false
		}
	}

	if container_tracked {

		parsed_time, err := time.Parse(time.RFC3339, logTime)
		if err != nil {
			// send the error to telemetry
			fmt.Printf("Invalid time format seen in log loss monitoring: %s", logTime)
		}

		//TODO: not sure if we need to check that this is after the log start time, does fbit deliver messages in-order?
		if (!parsed_time.Before(start_monitoring_time[containerID])) && parsed_time.Before(stop_monitoring_time[containerID]) {
			logs_by_container[containerID] += 1
		}
	}
}

func monitor_container(containerID string, start_time time.Time, end_time time.Time) {
	message := fmt.Sprintf("in write_results_at_end_of_run(%s)", containerID)
	Log(message)

	start_monitoring_time_text, err := start_time.MarshalText()
	if err != nil {
		message := fmt.Sprintf("log-loss-counter: Error converting time to text? (how is this possible?): %s", err.Error())
		Log(message)
		return
	}

	stop_monitoring_time_text, err := end_time.MarshalText()
	if err != nil {
		message := fmt.Sprintf("log-loss-counter: Error converting time to text? (how is this possible?): %s", err.Error())
		Log(message)
		return
	}

	output_str := fmt.Sprintf("{\"ContainerID\": \"%s\", \"StartTime\": \"%s\", \"EndTime\": \"%s\"}\n", containerID, start_monitoring_time_text, stop_monitoring_time_text)
	err = create_or_append_to_file("/var/log_counts", containerID, output_str)
	if err != nil {
		message := fmt.Sprintf("log-loss-counter: Error creating file for %s", err.Error())
		Log(message)
		return
	}

	Log("about to sleep")

	time_to_sleep := time.Until(end_time) + extra_wait // wait a few seconds for any logs logged right at the time cutoff to be read
	time.Sleep(time_to_sleep)

	Log("done sleeping")

	// write to file to signify that monitoring is done and close the file. It will be the log_line_counter's responsibility to delete the file
	end_log_count := logs_by_container[containerID]
	output_str = fmt.Sprintf("{\"Final_log_count\": %d}\n", end_log_count)
	err = create_or_append_to_file("/var/log_counts", containerID, output_str)
	if err != nil {
		message := fmt.Sprintf("log-loss-counter: Error creating file for container %s, error is %s", containerID, err.Error())
		Log(message)
	}

	message = fmt.Sprintf("done monitoring container %s, final log count: %d", containerID, end_log_count)
	Log(message)

	// allow monitoring this container again
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
			message := fmt.Sprintf("log-loss-counter: Error creating folder: %s", err.Error())
			Log(message)
			return err
		}
	}

	f, err := os.OpenFile(filepath.Join(folder, filename), os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		message := fmt.Sprintf("log-loss-counter: Error opening file for tracked container: %s", err.Error())
		Log(message)
		return err
	}
	defer f.Close()
	if _, err := f.WriteString(text); err != nil {
		message := fmt.Sprintf("log-loss-counter: Error writing to file for tracked container: %s", err.Error())
		Log(message)
		return err
	}
	return nil
}

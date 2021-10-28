package main

import (
	"fmt"
	"os"
	"path/filepath"
	"time"
)

var logs_by_container map[string]int
var last_log_time map[string]time.Time
var stop_monitoring_time map[string]time.Time // TODO: store this as a list or sorted map instead of a map. We don't need random access into it
var log_path_by_container map[string]struct {
	path_a string
	path_b string
}
var all_containers map[string]bool // this map is being used as a set. bool is the value type because it's the smallest type
var tracked_containers int

var ticker *time.Ticker = nil

const target_ratio = 1.0
const monitoring_time_seconds = 30

var extra_wait time.Duration

func init() {
	extra_wait, _ := time.ParseDuration("10s")
}

// TODO: can these args be passed by reference?
func Process_log_batch(containerID *string, k8sNamespace *string, k8sPodName *string, containerName *string, logEntry *string, logTime *string) {
	container_tracked, container_seen := all_containers[*containerID]
	if !container_seen {
		//TODO: decide if we should track this container
		if tracked_containers/len(all_containers) <= target_ratio {
			all_containers[*containerID] = true
			container_tracked = true
			tracked_containers += 1
			log_path_by_container[*containerID] = struct {
				path_a string
				path_b string
			}{*k8sNamespace + "_" + *k8sPodName, *containerName}
			// kube-system_omsagent-qz74z_f5f369a7-4094-4f58-bdb9-23c35121e491
			logs_by_container[*containerID] = 0
			stop_monitoring_time[*containerID] = time.Now()

			go write_results_at_end_of_run(*containerID)

		} else {
			all_containers[*containerID] = false
			container_tracked = false
		}
	}

	if container_tracked {

		parsed_time, err := time.Parse(time.RFC3339, *logTime)
		if err != nil {
			// send the error to telemetry
			fmt.Printf("Invalid time format seen in log loss monitoring: %s", *logTime)
		} else {
			last_log_time[*containerID] = parsed_time
		}

		if stop_monitoring_time[*containerID].After(parsed_time) {
			// TODO: stop monitoring this container and send the number of logs observed to the independent log line counter
		} else {
			logs_by_container[*containerID] += 1
		}
	}
}

func write_results_at_end_of_run(containerID string) {
	// write file to signify monitoring is starting
	stop_monitoring_time, exists := stop_monitoring_time[containerID]
	if !exists {
		message := fmt.Sprintf("log-loss-counter: Error creating file for ")
		Log(message)
		return
	}

	stop_monitoring_time_text, err := stop_monitoring_time.MarshalText()
	if err != nil {
		message := fmt.Sprintf("log-loss-counter: Error converting time to text? (how is this possible?): %s", err.Error())
		Log(message)
		return
	}

	output_str := fmt.Sprintf(`{"path_a": "%s", "path_b": "%s", "endtime": "%s"}`, stop_monitoring_time_text)
	err = create_or_append_to_file("/var/log_counts", containerID, output_str)
	if err != nil {
		message := fmt.Sprintf("log-loss-counter: Error creating file for ", err.Error())
		Log(message)
		return
	}

	time_to_sleep := time.Until(stop_monitoring_time) + extra_wait // wait a few seconds for any logs logged right at the time cutoff to be read
	time.Sleep(time_to_sleep)

	// write to file to signify that monitoring is done and close the file. It will be the log_line_counter's responsibility to delete the file
	output_str = fmt.Sprintf(`{"final_log_count": "%s"}`, logs_by_container[containerID])
	err = create_or_append_to_file("/var/log_counts", containerID, output_str)
	if err != nil {
		message := fmt.Sprintf("log-loss-counter: Error creating file for ", err.Error())
		Log(message)
	}
}

// iotuil's single-line function for writing text to a file doesn't append
func create_or_append_to_file(folder string, filename string, text string) error {
	_, err := os.Stat(folder)
	if os.IsNotExist(err) {
		err = os.Mkdir(folder, 0755)
		if err != nil {
			message := fmt.Sprintf("log-loss-counter: Error creating folder: ", err.Error())
			Log(message)
			return err
		}
	}

	f, err := os.OpenFile(filepath.Join(folder, filename), os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		message := fmt.Sprintf("log-loss-counter: Error opening file for tracked container: ", err.Error())
		Log(message)
		return err
	}
	defer f.Close()
	if _, err := f.WriteString(text); err != nil {
		message := fmt.Sprintf("log-loss-counter: Error writing to file for tracked container: ", err.Error())
		Log(message)
		return err
	}
	return nil
}

func ensure_folder_exists(folder string) {

}

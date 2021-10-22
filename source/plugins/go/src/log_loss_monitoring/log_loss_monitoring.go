package log_loss_monitoring

import (
	"fmt"
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

			if ticker == nil {
				ticker = time.NewTicker(1 * time.Second)
				go check_for_expired_test_runs()
			}

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

func check_for_expired_test_runs() {
	for {
		select {
		case <-ticker.C:
			// do stuff
		}
	}
}

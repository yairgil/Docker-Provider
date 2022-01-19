package main

import (
	"fmt"
	"os"
	"path/filepath"
	"sync/atomic"
	"testing"
	"time"

	"github.com/golang/mock/gomock"
)

func Test_setupLogLossTracker(t *testing.T) {
	init_log_loss_monitoring_globals() // This turns on consistency checks in m_bytes_logged_storage

	env_mock_old := env_mock

	type test_struct struct {
		name                                  string
		CONTROLLER_TYPE                       string
		CONTAINER_RUNTIME                     string
		IN_UNIT_TEST                          string
		AZMON_ENABLE_LOG_LOSS_TRACKING        string
		AZMON_ENABLE_LOG_LOSS_TRACKING_SET    string
		AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE string
		AZMON_COLLECT_STDOUT_LOGS             string
		AZMON_COLLECT_STDERR_LOGS             string
		AZMON_STDERR_EXCLUDED_NAMESPACES      string
		AZMON_STDOUT_EXCLUDED_NAMESPACES      string
		enabled                               bool
		disabled_namespaces                   map[string]bool
	}

	// This is a pretty useless unit test, but it demonstrates the concept (putting together a real test
	// would require some large json structs). If getDataTypeToStreamIdMapping() is ever updated, that
	// would be a good opertunity to add some real test cases.
	tests := []test_struct{
		{
			"disable_by_replicaset", // name
			"ReplicaSet",            // CONTROLLER_TYPE
			"containerd",            // CONTAINER_RUNTIME
			"false",                 // IN_UNIT_TEST
			"true",                  // AZMON_ENABLE_LOG_LOSS_TRACKING
			"true",                  // AZMON_ENABLE_LOG_LOSS_TRACKING_SET
			"true",                  // AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE
			"true",                  // AZMON_COLLECT_STDOUT_LOGS
			"true",                  // AZMON_COLLECT_STDERR_LOGS
			"",                      // AZMON_STDERR_EXCLUDED_NAMESPACES
			"",                      // AZMON_STDOUT_EXCLUDED_NAMESPACES
			false,                   // enabled
			make(map[string]bool),   // disabled_namespaces
		},
		{
			"disable_by_container_runtime", // name
			"DaemonSet",                    // CONTROLLER_TYPE
			"docker",                       // CONTAINER_RUNTIME
			"false",                        // IN_UNIT_TEST
			"true",                         // AZMON_ENABLE_LOG_LOSS_TRACKING
			"true",                         // AZMON_ENABLE_LOG_LOSS_TRACKING_SET
			"true",                         // AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE
			"true",                         // AZMON_COLLECT_STDOUT_LOGS
			"true",                         // AZMON_COLLECT_STDERR_LOGS
			"",                             // AZMON_STDERR_EXCLUDED_NAMESPACES
			"",                             // AZMON_STDOUT_EXCLUDED_NAMESPACES
			false,                          // enabled
			make(map[string]bool),          // disabled_namespaces
		},
		{
			"enable_by_configmap", // name
			"DaemonSet",           // CONTROLLER_TYPE
			"containerd",          // CONTAINER_RUNTIME
			"false",               // IN_UNIT_TEST
			"true",                // AZMON_ENABLE_LOG_LOSS_TRACKING
			"true",                // AZMON_ENABLE_LOG_LOSS_TRACKING_SET
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE
			"true",                // AZMON_COLLECT_STDOUT_LOGS
			"true",                // AZMON_COLLECT_STDERR_LOGS
			"",                    // AZMON_STDERR_EXCLUDED_NAMESPACES
			"",                    // AZMON_STDOUT_EXCLUDED_NAMESPACES
			true,                  // enabled
			make(map[string]bool), // disabled_namespaces
		},
		{
			"disable_by_configmap", // name
			"DaemonSet",            // CONTROLLER_TYPE
			"containerd",           // CONTAINER_RUNTIME
			"false",                // IN_UNIT_TEST
			"false",                // AZMON_ENABLE_LOG_LOSS_TRACKING
			"true",                 // AZMON_ENABLE_LOG_LOSS_TRACKING_SET
			"true",                 // AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE
			"true",                 // AZMON_COLLECT_STDOUT_LOGS
			"true",                 // AZMON_COLLECT_STDERR_LOGS
			"",                     // AZMON_STDERR_EXCLUDED_NAMESPACES
			"",                     // AZMON_STDOUT_EXCLUDED_NAMESPACES
			false,                  // enabled
			make(map[string]bool),  // disabled_namespaces
		},
		{
			"enabled_by_toggle",   // name
			"DaemonSet",           // CONTROLLER_TYPE
			"containerd",          // CONTAINER_RUNTIME
			"false",               // IN_UNIT_TEST
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING_SET
			"true",                // AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE
			"true",                // AZMON_COLLECT_STDOUT_LOGS
			"true",                // AZMON_COLLECT_STDERR_LOGS
			"",                    // AZMON_STDERR_EXCLUDED_NAMESPACES
			"",                    // AZMON_STDOUT_EXCLUDED_NAMESPACES
			true,                  // enabled
			make(map[string]bool), // disabled_namespaces
		},
		{
			"disabled_by_toggle",  // name
			"DaemonSet",           // CONTROLLER_TYPE
			"containerd",          // CONTAINER_RUNTIME
			"false",               // IN_UNIT_TEST
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING_SET
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE
			"true",                // AZMON_COLLECT_STDOUT_LOGS
			"true",                // AZMON_COLLECT_STDERR_LOGS
			"",                    // AZMON_STDERR_EXCLUDED_NAMESPACES
			"",                    // AZMON_STDOUT_EXCLUDED_NAMESPACES
			false,                 // enabled
			make(map[string]bool), // disabled_namespaces
		},
		{
			"disabled_by_stdout",  // name
			"DaemonSet",           // CONTROLLER_TYPE
			"containerd",          // CONTAINER_RUNTIME
			"false",               // IN_UNIT_TEST
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING_SET
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE
			"false",               // AZMON_COLLECT_STDOUT_LOGS
			"true",                // AZMON_COLLECT_STDERR_LOGS
			"",                    // AZMON_STDERR_EXCLUDED_NAMESPACES
			"",                    // AZMON_STDOUT_EXCLUDED_NAMESPACES
			false,                 // enabled
			make(map[string]bool), // disabled_namespaces
		},
		{
			"disabled_by_stderr",  // name
			"DaemonSet",           // CONTROLLER_TYPE
			"containerd",          // CONTAINER_RUNTIME
			"false",               // IN_UNIT_TEST
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING_SET
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE
			"true",                // AZMON_COLLECT_STDOUT_LOGS
			"false",               // AZMON_COLLECT_STDERR_LOGS
			"",                    // AZMON_STDERR_EXCLUDED_NAMESPACES
			"",                    // AZMON_STDOUT_EXCLUDED_NAMESPACES
			false,                 // enabled
			make(map[string]bool), // disabled_namespaces
		},
		{
			"disabled_namespaces", // name
			"DaemonSet",           // CONTROLLER_TYPE
			"containerd",          // CONTAINER_RUNTIME
			"false",               // IN_UNIT_TEST
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING_SET
			"false",               // AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE
			"true",                // AZMON_COLLECT_STDOUT_LOGS
			"false",               // AZMON_COLLECT_STDERR_LOGS
			`["default", "ns2"]`,  // AZMON_STDERR_EXCLUDED_NAMESPACES
			"",                    // AZMON_STDOUT_EXCLUDED_NAMESPACES
			false,                 // enabled
			map[string]bool{"default": true, "ns2": true}, // disabled_namespaces
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			mockCtrl := gomock.NewController(t)
			mock := NewMockIGetEnvVar(mockCtrl)
			mock.EXPECT().Getenv("CONTROLLER_TYPE").Return(tt.CONTROLLER_TYPE).AnyTimes()
			mock.EXPECT().Getenv("CONTAINER_RUNTIME").Return(tt.CONTAINER_RUNTIME).AnyTimes()
			mock.EXPECT().Getenv("IN_UNIT_TEST").Return(tt.IN_UNIT_TEST).AnyTimes()
			mock.EXPECT().Getenv("AZMON_ENABLE_LOG_LOSS_TRACKING_SET").Return(tt.AZMON_ENABLE_LOG_LOSS_TRACKING_SET).AnyTimes()
			mock.EXPECT().Getenv("AZMON_ENABLE_LOG_LOSS_TRACKING").Return(tt.AZMON_ENABLE_LOG_LOSS_TRACKING).AnyTimes()
			mock.EXPECT().Getenv("AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE").Return(tt.AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE).AnyTimes()
			mock.EXPECT().Getenv("AZMON_COLLECT_STDOUT_LOGS").Return(tt.AZMON_COLLECT_STDOUT_LOGS).AnyTimes()
			mock.EXPECT().Getenv("AZMON_COLLECT_STDERR_LOGS").Return(tt.AZMON_COLLECT_STDERR_LOGS).AnyTimes()
			mock.EXPECT().Getenv("AZMON_STDERR_EXCLUDED_NAMESPACES").Return(tt.AZMON_STDERR_EXCLUDED_NAMESPACES).AnyTimes()
			mock.EXPECT().Getenv("AZMON_STDOUT_EXCLUDED_NAMESPACES").Return(tt.AZMON_STDOUT_EXCLUDED_NAMESPACES).AnyTimes()
			env_mock = mock

			setupLogLossTracker()

			if enable_log_loss_detection != tt.enabled {
				t.Errorf("Expected log loss tracking to be %v, was actually %v", tt.enabled, enable_log_loss_detection)
			}

			mockCtrl.Finish()
		})
	}

	env_mock = env_mock_old
}

func Test_Process_log(t *testing.T) {
	init_log_loss_monitoring_globals() // This turns on consistency checks in m_bytes_logged_storage
	// m_bytes_logged_storage.debug_mode = true

	containerID := "containerID"
	k8sNamespace := "k8sNamespace"
	k8sPodName := "k8sPodName"
	containerName := "containerName"
	logEntry := "1234567890"
	logTime := "1970/01/01T01:01:01.00001Z"

	enable_log_loss_detection = true

	identifier := k8sNamespace + "_" + k8sPodName + "_" + containerName
	if len(container_logs_current) != 0 {
		t.Error("container_logs_current didn't start empty")
	}

	Process_log(&containerID, &k8sNamespace, &k8sPodName, &containerName, &logEntry, &logTime)

	val, exists := container_logs_current[identifier]

	if !exists {
		t.Error("Process_log() didn't create record for container")
	}

	if val != int64(len("1234567890")+len(logTime)+len(" stdout f ")+1) {
		t.Error("Process_log() had incorrect log count")
	}

	Process_log(&containerID, &k8sNamespace, &k8sPodName, &containerName, &logEntry, &logTime)

	val, exists = container_logs_current[identifier]

	if !exists {
		t.Error("Process_log() didn't create record for container")
	}

	if val != int64((len("1234567890")+len(logTime)+len(" stdout f ")+1)*2) {
		t.Error("Process_log() had incorrect log count")
	}
}

func Test_Process_log_send_snapshot(t *testing.T) {
	init_log_loss_monitoring_globals()
	enable_log_loss_detection = true

	containerID := "containerID"
	k8sNamespace := "k8sNamespace"
	k8sPodName := "k8sPodName"
	logEntry := "1234567890"
	logTime := "1970/01/01T01:01:01.00001Z"

	for i := 0; i < 10; i++ {
		containerNameTemp := fmt.Sprintf("containerName_%d", i)
		Process_log(&containerID, &k8sNamespace, &k8sPodName, &containerNameTemp, &logEntry, &logTime)
	}

	atomic.StoreInt32(&request_snapshot, 1)
	containerNameTemp := fmt.Sprintf("containerName_%d", -1)
	Process_log(&containerID, &k8sNamespace, &k8sPodName, &containerNameTemp, &logEntry, &logTime)

	if len(container_logs_current) != 0 {
		t.Error("m_bytes_logged_storage didn't have the right number of entries, garbage collection failed?")
	}

	select {
	case snapshot := <-snapshot_chan:
		if len(snapshot) != 11 {
			t.Errorf("Process_log() sent a wrong snapshot, should have had 11 containers in it: %v", snapshot)
		}
	default:
		t.Error("Process_log() didn't send a snapshot after one was requested")
	}
}

func Test_track_log_rotations(t *testing.T) {
	init_log_loss_monitoring_globals()
	// m_bytes_logged_storage.debug_mode = true

	test_dir := filepath.Join(get_repo_root_dir(), "test", "unit-tests", "other-test-directories", "log-loss-detection", "pods_1")

	if _, err := os.Stat(filepath.Join(test_dir, "marker.txt")); os.IsNotExist(err) {
		message := "unit test Test_track_log_rotations() not in the right directory. The test setup is wrong"
		t.Error(message)
		panic(message)
	}

	if len(FW_records) != 0 {
		t.Error("FW_records did not start empty in unit test (this is probably a test problem, not a code problem)")
	}

	ch := make(chan time.Time)
	go func() {
		ch <- time.Now()
		close(ch)
	}()
	track_log_rotations(ch, test_dir)

	if len(FW_records) != 8 {
		t.Errorf("FW_records did not have the correct number of records, actually had actually %d", len(FW_records))
	}

	if sum_undeleted_bytes(FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].existing_log_files) != 30 {
		t.Errorf("incorrect number of undeleted bytes, was actually %d", sum_undeleted_bytes(FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].existing_log_files))
	}

	if FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].deleted_bytes != 0 {
		t.Errorf("incorrect number of deleted bytes, was actually %d", FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].deleted_bytes)
	}

	if len(FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].existing_log_files) != 2 {
		t.Error("incorrect set of existing log files")
	}

	// now test garbage collection. The second test directory has one fewer pod folders

	test_dir_2 := filepath.Join(get_repo_root_dir(), "test", "unit-tests", "other-test-directories", "log-loss-detection", "pods_1_removed")
	ch2 := make(chan time.Time)
	go func() {
		ch2 <- time.Now()
		close(ch2)
	}()
	track_log_rotations_impl(ch2, test_dir_2, 1)

	if len(FW_records) != 7 {
		t.Errorf("FW_records did not have the correct number of records, actually had actually %d", len(FW_records))
	}

	if FW_records["default_highscale-deployment-10-kb-minute-569b8b9988-rc7l8_highscale"].deleted_bytes != 30 {
		t.Errorf(`FW_records["default_highscale-deployment-10-kb-minute-569b8b9988-rc7l8_highscale"].deleted_bytes != 30`+", actual value was %d", FW_records["default_highscale-deployment-10-kb-minute-569b8b9988-rc7l8_highscale"].deleted_bytes)
	}
	if len(FW_records["default_highscale-deployment-10-kb-minute-569b8b9988-rc7l8_highscale"].existing_log_files) != 2 {
		t.Errorf(`len(FW_records["default_highscale-deployment-10-kb-minute-569b8b9988-rc7l8_highscale"].existing_log_files) != 2`+", actual value was %d", len(FW_records["default_highscale-deployment-10-kb-minute-569b8b9988-rc7l8_highscale"].existing_log_files))
	}
	if sum_undeleted_bytes(FW_records["default_highscale-deployment-10-kb-minute-569b8b9988-rc7l8_highscale"].existing_log_files) != 15 {
		t.Errorf(`sum_undeleted_bytes(FW_records["default_highscale-deployment-10-kb-minute-569b8b9988-rc7l8_highscale"].existing_log_files) != 15`+", actual value was %d", sum_undeleted_bytes(FW_records["default_highscale-deployment-10-kb-minute-569b8b9988-rc7l8_highscale"].existing_log_files))
	}

	if disk_bytes_from_deleted_containers != 300 {
		t.Errorf(`disk_bytes_from_deleted_containers != 300, actual value was %d`, disk_bytes_from_deleted_containers)
	}
}

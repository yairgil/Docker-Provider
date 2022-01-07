package main

import (
	"fmt"
	"os"
	"path"
	"path/filepath"
	"strings"
	"testing"
	"time"

	"github.com/golang/mock/gomock"
)

func Test_setupLogLossTracker(t *testing.T) {
	init_log_loss_monitoring_globals() // This turns on consistency checks in m_bytes_logged_storage
	m_bytes_logged_storage.debug_mode = true

	env_mock_old := env_mock

	type test_struct struct {
		name                                  string
		CONTROLLER_TYPE                       string
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
			"enable_by_configmap", // name
			"DaemonSet",           // CONTROLLER_TYPE
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
			defer mockCtrl.Finish()
			mock := NewMockIGetEnvVar(mockCtrl)
			mock.EXPECT().Getenv("CONTROLLER_TYPE").Return(tt.CONTROLLER_TYPE).Times(1)
			mock.EXPECT().Getenv("IN_UNIT_TEST").Return(tt.IN_UNIT_TEST).Times(1)
			mock.EXPECT().Getenv("AZMON_ENABLE_LOG_LOSS_TRACKING_SET").Return(tt.AZMON_ENABLE_LOG_LOSS_TRACKING_SET).Times(1)
			if strings.ToLower(tt.AZMON_ENABLE_LOG_LOSS_TRACKING_SET) == "true" {
				mock.EXPECT().Getenv("AZMON_ENABLE_LOG_LOSS_TRACKING").Return(tt.AZMON_ENABLE_LOG_LOSS_TRACKING).Times(1)
			} else {
				mock.EXPECT().Getenv("AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE").Return(tt.AZMON_ENABLE_LOG_LOSS_TRACKING_TOGGLE).Times(1)
			}
			if tt.enabled {
				mock.EXPECT().Getenv("AZMON_COLLECT_STDOUT_LOGS").Return(tt.AZMON_COLLECT_STDOUT_LOGS).Times(1)
				mock.EXPECT().Getenv("AZMON_COLLECT_STDERR_LOGS").Return(tt.AZMON_COLLECT_STDERR_LOGS).Times(1)
				mock.EXPECT().Getenv("AZMON_STDERR_EXCLUDED_NAMESPACES").Return(tt.AZMON_STDERR_EXCLUDED_NAMESPACES).Times(1)
				mock.EXPECT().Getenv("AZMON_STDOUT_EXCLUDED_NAMESPACES").Return(tt.AZMON_STDOUT_EXCLUDED_NAMESPACES).Times(1)
			}
			env_mock = mock

			setupLogLossTracker()

			if enabled != tt.enabled {
				t.Errorf("Expected log loss tracking to be %v, was actually %v", tt.enabled, enabled)
			}
		})
	}

	env_mock = env_mock_old
}

func Test_Process_log(t *testing.T) {
	init_log_loss_monitoring_globals() // This turns on consistency checks in m_bytes_logged_storage
	m_bytes_logged_storage.debug_mode = true

	containerID := "containerID"
	k8sNamespace := "k8sNamespace"
	k8sPodName := "k8sPodName"
	containerName := "containerName"
	logEntry := "1234567890"
	logTime := "1970/01/01T01:01:01.00001Z"

	enabled = true

	identifier := k8sNamespace + "_" + k8sPodName + "_" + containerName
	if len(m_bytes_logged_storage.string_to_arr_index) != 0 {
		t.Error("m_bytes_logged_storage.string_to_arr_index didn't start empty")
	}

	if len(m_bytes_logged_storage.container_identifiers) != 0 {
		t.Error("m_bytes_logged_storage.container_identifiers didn't start empty")
	}

	if len(m_bytes_logged_storage.log_counts) != 0 {
		t.Error("m_bytes_logged_storage.log_counts didn't start empty")
	}

	Process_log(&containerID, &k8sNamespace, &k8sPodName, &containerName, &logEntry, &logTime)

	val, _ := m_bytes_logged_storage.get(identifier)

	if *val != int64(len("1234567890")+len(logTime)+len(" stdout f ")+1) {
		t.Error("log count wrong")
	}

	// test garbage collection
	select {
	case deleted_identifier := <-deleted_containers_query:
		if deleted_identifier != k8sNamespace+"_"+k8sPodName+"_"+containerName {
			t.Error("got wrong container from Process_log for garbage collection")
		}
	default:
		t.Error("Process_log did not write a container for garbage collection")
	}

	deleted_containers_response <- k8sNamespace + "_" + k8sPodName + "_" + containerName

	containerName2 := "secondContainer"
	Process_log(&containerID, &k8sNamespace, &k8sPodName, &containerName2, &logEntry, &logTime)

	if m_bytes_logged_storage.len() != 1 {
		t.Error("m_bytes_logged_storage didn't have the right number of entries, did garbage collection fail?")
	}

	if _, new := m_bytes_logged_storage.get(k8sNamespace + "_" + k8sPodName + "_" + containerName2); new == true {
		t.Error("m_bytes_logged_storage didn't have the stored container identifier?")
	}
}

// This test adds 100 containers then makes sure all but one are gone within a few calls to Process_log()
func Test_track_log_rotations_garbage_collection(t *testing.T) {
	init_log_loss_monitoring_globals()
	m_bytes_logged_storage.debug_mode = true
	enabled = true

	containerID := "containerID"
	k8sNamespace := "k8sNamespace"
	k8sPodName := "k8sPodName"
	logEntry := "1234567890"
	logTime := "1970/01/01T01:01:01.00001Z"

	num_containers_on_disk = 1

	for i := 0; i < 100; i++ {
		containerNameTemp := fmt.Sprintf("containerName_%d", i)
		Process_log(&containerID, &k8sNamespace, &k8sPodName, &containerNameTemp, &logEntry, &logTime)
	}

	for i := 0; i < 50; i++ {
		containerNameTemp := fmt.Sprintf("containerName2_%d", i)
		Process_log(&containerID, &k8sNamespace, &k8sPodName, &containerNameTemp, &logEntry, &logTime)
	read_from_chan:
		for {
			select {
			case deleted_identifier := <-deleted_containers_query:
				if deleted_identifier != containerNameTemp {
					deleted_containers_response <- deleted_identifier
				}
			default:
				break read_from_chan
			}
		}
	}

	// It won't actually get down to 1 container since the previous loop added a container each iteration. But if
	// most of them are gone then it's good enough
	if m_bytes_logged_storage.len() >= 25 {
		t.Error("m_bytes_logged_storage didn't have the right number of entries, garbage collection failed?")
	}
}

func Test_track_log_rotations(t *testing.T) {
	init_log_loss_monitoring_globals()
	m_bytes_logged_storage.debug_mode = true

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

	if len(FW_records) != 15 {
		t.Errorf("FW_records did not have the correct number of records, actually had actually %d", len(FW_records))
	}

	if num_containers_on_disk != 15 {
		t.Errorf("num_containers_on_disk != 15, was actually %d", num_containers_on_disk)
	}

	if FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].unrotated_bytes != 10 {
		t.Errorf("incorrect number of bytes unrotated, was actually %d", FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].unrotated_bytes)
	}

	if FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].rotated_bytes != 20 {
		t.Errorf("incorrect number of bytes rotated, was actually %d", FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].rotated_bytes)
	}

	if len(FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].existing_log_files) != 1 &&
		FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].existing_log_files[0] != "0.log.20211214-201022" {
		t.Error("incorrect set of existing log files")
	}

	deleted_containers_query <- "default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"
	deleted_containers_query <- "doesn't exist"

	ch = make(chan time.Time)

	go func() {
		ch <- time.Now()
		close(ch)
	}()
	track_log_rotations(ch, test_dir)

	should_not_exist := ""

	select {
	case should_not_exist = <-deleted_containers_response:
	default:
		t.Error("track_log_rotations didn't return any containers for garbage collection")
	}

	if should_not_exist != "doesn't exist" {
		t.Errorf("track_log_rotations returned a wrong container for garbage collection (%s)\n", should_not_exist)
	}

	select {
	case <-deleted_containers_response:
		t.Error("track_log_rotations returned too many containers for garbage collection")
	default:
	}

}

func get_repo_root_dir() string {

	dir, err := os.Getwd()
	if err != nil {
		panic(err)
	}

	for {
		_, err := os.Stat(path.Join(dir, "ReleaseNotes.md"))
		if !os.IsNotExist(err) {
			return dir
		}

		dir = path.Join(dir, "..") // this actually removes the last directory in the path instead of appending /..

		if len(dir) <= 1 {
			panic("Not run in docker-provider repo")
		}
	}
}

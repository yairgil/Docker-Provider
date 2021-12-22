package main

import (
	"os"
	"path"
	"path/filepath"
	"testing"
	"time"
)

func Test_Process_log(t *testing.T) {
	init_log_loss_monitoring_globals()

	containerID := "containerID"
	k8sNamespace := "k8sNamespace"
	k8sPodName := "k8sPodName"
	containerName := "containerName"
	logEntry := "1234567890"
	logTime := "1970/01/01T01:01:01.00001Z"

	enabled = true

	identifier := k8sNamespace + "_" + k8sPodName + "_" + containerName
	if len(m_bytes_logged_storage.container_identifiers) != 0 {
		t.Error("m_bytes_logged_storage didn't start empty")
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
}

func Test_track_log_rotations(t *testing.T) {
	init_log_loss_monitoring_globals()

	test_dir := filepath.Join(get_repo_root_dir(), "test", "unit-tests", "other-test-directories", "log-loss-detection", "pods_1")

	if len(FW_records) != 0 {
		t.Error("FW_existing_log_files did not start empty in unit test (this is probably a test problem, not a code problem)")
	}

	if len(FW_records) != 0 {
		t.Error("FW_bytes_logged_rotated did not start empty in unit test (this is probably a test problem, not a code problem)")
	}

	if len(FW_records) != 0 {
		t.Error("FW_bytes_logged_unrotated did not start empty in unit test (this is probably a test problem, not a code problem)")
	}

	ch := make(chan time.Time)
	go func() {
		ch <- time.Now()
		close(ch)
	}()
	track_log_rotations(ch, test_dir)

	if len(FW_records) != 15 {
		t.Error("FW_existing_log_files did not start empty in unit test (this is probably a test problem, not a code problem)")
	}

	if num_containers_on_disk != 15 {
		t.Error("FW_existing_log_files did not start empty in unit test (this is probably a test problem, not a code problem)")
	}

	if FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].unrotated_bytes != 10 {
		t.Error("incorrect number of bytes unrotated")
	}

	if FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].rotated_bytes != 20 {
		t.Error("incorrect number of bytes rotated")
	}

	if len(FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].existing_log_files) != 1 &&
		FW_records["default_highscale-deployment-x-mb-minute-58f4b769-l894d_highscale"].existing_log_files[0] != "0.log.20211214-201022" {
		t.Error("incorrect number of bytes rotated")
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
	init_log_loss_monitoring_globals()

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

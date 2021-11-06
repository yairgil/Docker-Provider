package main

import (
	"errors"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"path"
	"strings"
	"time"

	// these two dependencies need an OSS review before being released.
	// hpcloud/tail is pretty important to have, we could do without fsnotify (but being cross platform might take more work)

	"gopkg.in/natefinch/lumberjack.v2"
)

func get_first_word(input string) string {
	for i := range input {
		if input[i] == ' ' {
			return input[0:i]
		}
	}
	return input
}

func get_container_log_file_name(dir string, containerID string) (string, error) {
	files, err := ioutil.ReadDir(dir)
	if err != nil {
		Log("ERROR: could not get log file name ", err.Error())
	}

	for _, f := range files {
		if strings.Contains(f.Name(), containerID) {
			return path.Join(dir, f.Name()), nil
		}
	}
	return "", errors.New("container log file not found")
}

// TODO: this code was copied from oms.go. Maybe extract code into a shared file/library?
func createLogger() *log.Logger {
	osType := os.Getenv("OS_TYPE")

	var logPath string
	var logfile *os.File

	if strings.Compare(strings.ToLower(osType), "windows") != 0 {
		logPath = "/var/opt/microsoft/docker-cimprov/log/log_line_counter.log"
	} else {
		logPath = "/etc/omsagentwindows/fluent-bit-out-oms-runtime.log"
	}

	if _, err := os.Stat(logPath); err == nil {
		fmt.Printf("File Exists. Opening file in append mode...\n")
		logfile, err = os.OpenFile(logPath, os.O_APPEND|os.O_WRONLY, 0600)
		if err != nil {
			fmt.Printf("Error: could not open file (error: %s)", err.Error())
		}
	}

	if _, err := os.Stat(logPath); os.IsNotExist(err) {
		fmt.Printf("File Doesnt Exist. Creating file...\n")
		logfile, err = os.Create(logPath)
		if err != nil {
			fmt.Printf("Error: could not create file (error: %s)", err.Error())
		}
	}

	logger := log.New(logfile, "", 0)

	logger.SetOutput(&lumberjack.Logger{
		Filename:   logPath,
		MaxSize:    10, //megabytes
		MaxBackups: 1,
		MaxAge:     28,   //days
		Compress:   true, // false by default

	})

	logger.SetFlags(log.Ltime | log.Lshortfile | log.LstdFlags)

	// flush the log file at a reasonable rate (oms.go's logfile flushes really slowly, I don't want to deal with that here either)
	TimeTicker := time.NewTicker(5 * time.Second)
	go func() {
		for range TimeTicker.C {
			err := logfile.Sync()
			if err != nil {
				// also write this error directly to standard out (since the log file is having trouble)
				fmt.Printf("Error flushing log file: %s\n", err.Error())
				logger.Printf("Error flushing log file: %s", err.Error())
			}
		}
	}()

	return logger
}

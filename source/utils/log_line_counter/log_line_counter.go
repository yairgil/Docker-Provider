package main

import (
	"encoding/json"
	"errors"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"strings"
	"time"

	// these two dependencies need an OSS review before being released.
	// hpcloud/tail is pretty important to have, we could do without fsnotify (but being cross platform might take more work)
	"github.com/fsnotify/fsnotify"
	"github.com/hpcloud/tail"
	"gopkg.in/natefinch/lumberjack.v2"
)

var (
	// FLBLogger stream
	FLBLogger = createLogger()
	// Log wrapper function
	Log = FLBLogger.Printf
)

// This is where out_oms.go will write containers to monitor and it's log counts
const coms_dir = "/var/log_counts/"

func main() {
	Log("log line counter starting up")

	// check for any log count files which were created before this process starts up.

	files, err := ioutil.ReadDir(coms_dir)
	if err != nil {
		log.Fatal(err)
	}
	for _, f := range files {
		Log("found file \"", f.Name(), "\" at starup")
		go monitor_file(coms_dir + f.Name())
	}

	Log("done enumerating existing log count requests")

	// yes there is a race condition here (where this program finishes going through existing request files,
	// out_oms writes a new request file, then this program starts monitoring the request folder for new request files.)
	// We can fix it later.

	watcher, err := fsnotify.NewWatcher()
	if err != nil {
		log.Fatal(err)
	}
	defer watcher.Close()

	go func() {
		for {
			select {
			case event, ok := <-watcher.Events:
				if !ok {
					return
				}
				Log("event:", event)
				if event.Op&fsnotify.Create == fsnotify.Create {
					Log("modified file:", event.Name)
					go monitor_file(event.Name)
				}
			case err, ok := <-watcher.Errors:
				if !ok {
					return
				}
				Log("error:", err)
			}
		}
	}()

	err = watcher.Add(coms_dir)
	if err != nil {
		log.Fatal(err)
	}

	Log("watch set up, now waiting forever")

	// wait forever
	done := make(chan string)
	<-done

	FLBLogger.Fatalf("ERROR: somehow finished waiting forever")
}

// {"ContainerID": "%s", "EndTime": "%s"}
type firstMessageStruct struct {
	ContainerID string
	StartTime   string
	EndTime     string
}

// {"Final_log_count": "%i"}
type secondMessageStruct struct {
	Final_log_count int
}

func monitor_file(messageFileName string) {

	// first get the config (contains which container to track and for how long)

	log.Println("")

	configFile, err := tail.TailFile(messageFileName, tail.Config{Follow: true, MustExist: true, Poll: false})
	if err != nil {
		FLBLogger.Fatalf("error opening file: %s\n", err.Error())
	}
	defer configFile.Cleanup()

	configFileFirstLineStruct := (<-configFile.Lines)
	// var configFileFirstLineStruct *tail.Line
	// for a := range configFile.Lines {
	// 	configFileFirstLineStruct = a
	// 	// break
	// 	log.Printf("%s", a.Text)

	// }
	if configFileFirstLineStruct.Err != nil {
		//TODO: error handling
		FLBLogger.Fatalf("error reading config file: %s\n", configFileFirstLineStruct.Err.Error())
		return
	}
	Log("read config line: ", configFileFirstLineStruct.Text)
	var firstMessage firstMessageStruct
	json.Unmarshal([]byte(configFileFirstLineStruct.Text), &firstMessage)

	// when to start counting logs
	startTime, err := time.Parse(time.RFC3339, firstMessage.StartTime)
	if err != nil {
		FLBLogger.Fatalf("error parsing start time: %s, (error: %s)\n", firstMessage.StartTime, err.Error())
		return
	}

	// when to stop counting logs
	endTime, err := time.Parse(time.RFC3339, firstMessage.EndTime)
	if err != nil {
		FLBLogger.Fatalf("error parsing end time: %s, (error: %s)\n", firstMessage.EndTime, err.Error())
		return
	}

	// get the name of the container's log file
	filename, err := get_container_log_file_name("/var/log/containers/", firstMessage.ContainerID)
	if err != nil {
		FLBLogger.Fatalf("container log file not found\n")
		return
	}

	Log("opening container log file " + filename)

	container_tailer, err := tail.TailFile("/var/log/containers/"+filename, tail.Config{Follow: true, MustExist: true, Poll: false})
	if err != nil {
		FLBLogger.Fatalf("error opening file: %s\n", err.Error())
		return
	}
	defer container_tailer.Cleanup()

	go wait_then_stop(endTime, container_tailer)

	var linecount = 0
	for line := range container_tailer.Lines {
		if line.Err != nil {
			FLBLogger.Fatalf("error reading file: %s\n", err.Error())
			//TODO: was the error because stop was caled? check for the specific error
			break
		}

		logTime, err := time.Parse(time.RFC3339, get_first_word(line.Text))
		if err != nil {
			FLBLogger.Fatalf("error parsing time: %s\n", err.Error())
			FLBLogger.Fatalf("line was \"%s\"\n", line.Text)
		}

		if !logTime.Before(startTime) {
			if logTime.Before(endTime) {
				linecount += 1
			} else {
				break
			}
		}
	}
	Log("finished tailing container " + firstMessage.ContainerID + ", read " + string(linecount) + " log lines\n")

	// now wait for oms.go to write it's log line count
	configFileSecondLineStruct := <-configFile.Lines
	if configFileSecondLineStruct == nil {
		//TODO: error handling
		FLBLogger.Fatalf("configFileSecondLineStruct is nil")
		return
	}
	if configFileSecondLineStruct.Err != nil {
		//TODO: error handling
		FLBLogger.Fatalf("error reading config file: %s\n", configFileSecondLineStruct.Err.Error())
		return
	}

	Log("read config line: ", configFileSecondLineStruct.Text)
	var secondMessage secondMessageStruct
	json.Unmarshal([]byte(configFileSecondLineStruct.Text), &secondMessage)

	Log("finished everything for container " + firstMessage.ContainerID + ", counted " + fmt.Sprint(linecount) + " logs, recieved count of " + fmt.Sprint(secondMessage.Final_log_count) + " from oms.go\n")

	if secondMessage.Final_log_count != linecount {
		fmt.Printf("Log lines were missed! fbit counted %d lines, %d lines actually logged. (container %s)\n", secondMessage.Final_log_count, linecount, filename)
	}
}

func wait_then_stop(timeToStop time.Time, tailObj *tail.Tail) {
	time.Sleep(time.Until(timeToStop))
	time.Sleep(5000 * time.Millisecond)
	tailObj.Stop()
}

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
		log.Fatal(err)
	}

	for _, f := range files {
		if strings.Contains(f.Name(), containerID) {
			return f.Name(), nil
		}
	}
	return "", errors.New("container log file not found")
}

func createLogger() *log.Logger {
	var logfile *os.File

	osType := os.Getenv("OS_TYPE")

	var logPath string

	if strings.Compare(strings.ToLower(osType), "windows") != 0 {
		logPath = "/var/opt/microsoft/docker-cimprov/log/log_line_counter.log"
	} else {
		logPath = "/etc/omsagentwindows/fluent-bit-out-oms-runtime.log"
	}

	if _, err := os.Stat(logPath); err == nil {
		fmt.Printf("File Exists. Opening file in append mode...\n")
		logfile, err = os.OpenFile(logPath, os.O_APPEND|os.O_WRONLY, 0600)
		if err != nil {
			fmt.Printf(err.Error())
		}
	}

	if _, err := os.Stat(logPath); os.IsNotExist(err) {
		fmt.Printf("File Doesnt Exist. Creating file...\n")
		logfile, err = os.Create(logPath)
		if err != nil {
			fmt.Printf(err.Error())
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
	return logger
}

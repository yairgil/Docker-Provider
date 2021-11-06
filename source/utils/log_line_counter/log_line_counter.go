package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"time"

	// these two dependencies need an OSS review before being released.
	// hpcloud/tail is pretty important to have, we could do without fsnotify (but being cross platform might take more work)
	"github.com/fsnotify/fsnotify"
	"github.com/hpcloud/tail"
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
		log.Fatal("FATAL: Could not read coms directory", err.Error())
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
		log.Fatal("FATAL: could not create watcher", err.Error())
	}
	defer watcher.Close()

	go func() {
		for {
			select {
			case event, ok := <-watcher.Events:
				if !ok {
					return
				}
				if event.Op&fsnotify.Create == fsnotify.Create {
					Log("new file: " + event.Name)
					go monitor_file(event.Name)
				}
			case err, ok := <-watcher.Errors:
				if !ok {
					return
				}
				Log("ERROR:", err)
			}
		}
	}()

	err = watcher.Add(coms_dir)
	if err != nil {
		log.Fatal("FATAL: could not watch coms directory", err.Error())
	}

	Log("watch set up, now waiting forever")

	// wait forever
	done := make(chan string)
	<-done
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
		FLBLogger.Printf("ERROR: error opening file: %s\n", err.Error())
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
		FLBLogger.Printf("ERROR: error reading config file: %s\n", configFileFirstLineStruct.Err.Error())
		return
	}
	Log("recieved command: %s", configFileFirstLineStruct.Text)
	var firstMessage firstMessageStruct
	err = json.Unmarshal([]byte(configFileFirstLineStruct.Text), &firstMessage)
	if err != nil {
		FLBLogger.Printf("ERROR: error unmarshalling message file (error: %s)\n", err.Error())
		return
	}

	// when to start counting logs
	startTime, err := time.Parse(time.RFC3339, firstMessage.StartTime)
	if err != nil {
		FLBLogger.Printf("ERROR: error parsing start time: %s, (error: %s)\n", firstMessage.StartTime, err.Error())
		return
	}

	// when to stop counting logs
	endTime, err := time.Parse(time.RFC3339, firstMessage.EndTime)
	if err != nil {
		FLBLogger.Printf("ERROR: parsing end time: %s, (error: %s)\n", firstMessage.EndTime, err.Error())
		return
	}

	// get the name of the container's log file
	filename, err := get_container_log_file_name("/var/log/containers/", firstMessage.ContainerID)
	if err != nil {
		FLBLogger.Printf("ERROR: container log file not found (container ID: %s)\n", firstMessage.ContainerID)
		return
	}

	Log("opening container log file " + filename)

	container_tailer, err := tail.TailFile("/var/log/containers/"+filename, tail.Config{Follow: true, MustExist: true, Poll: false})
	if err != nil {
		FLBLogger.Printf("ERROR: error opening file: %s\n", err.Error())
		return
	}
	defer container_tailer.Cleanup()

	go wait_then_stop(endTime, container_tailer)

	var linecount = 0
	for line := range container_tailer.Lines {
		if line.Err != nil {
			FLBLogger.Printf("ERROR: error reading file: %s\n", err.Error())
			//TODO: was the error because stop was caled? check for the specific error
			break
		}

		logTime, err := time.Parse(time.RFC3339, get_first_word(line.Text))
		if err != nil {
			FLBLogger.Printf("ERROR: error parsing time: %s\n", err.Error())
			FLBLogger.Printf("ERROR: line was \"%s\"\n", line.Text)
		}

		if !logTime.Before(startTime) {
			if logTime.Before(endTime) {
				linecount += 1
			} else {
				break
			}
		}
	}
	Log("finished tailing container " + filename + ", read " + fmt.Sprint(linecount) + " log lines\n")

	// now wait for oms.go to write it's log line count
	configFileSecondLineStruct := <-configFile.Lines
	if configFileSecondLineStruct == nil {
		//TODO: error handling
		FLBLogger.Printf("ERROR: configFileSecondLineStruct is nil")
		return
	}
	if configFileSecondLineStruct.Err != nil {
		//TODO: error handling
		FLBLogger.Printf("ERROR: error reading config file: %s\n", configFileSecondLineStruct.Err.Error())
		return
	}

	var secondMessage secondMessageStruct
	json.Unmarshal([]byte(configFileSecondLineStruct.Text), &secondMessage)

	Log("finished everything for container " + filename + ", counted " + fmt.Sprint(linecount) + " logs, recieved count of " + fmt.Sprint(secondMessage.Final_log_count) + " from oms.go\n")

	if secondMessage.Final_log_count != linecount {
		fmt.Printf("Log lines were missed! fbit counted %d lines, %d lines actually logged. (container %s)\n", secondMessage.Final_log_count, linecount, filename)
	}

	// TODO: uncomment this when done debugging
	// // delete the message file so they don't pile up forever
	// err = os.Remove(messageFileName)
	// if err != nil {
	// 	Log("error deleting message file " + err.Error())
	// }
}

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
	"github.com/nxadm/tail"
)

var (
	// FLBLogger stream
	FLBLogger = createLogger()
	// Log wrapper function
	Log = FLBLogger.Printf
)

// simpler plan:
/*
1. the logic for tailing files will be taken out of monitor_file() and extracted into it's own goroutine (tailfile()). It will output a count on a channel when time is up
2. monitor_file() will watch its log file for fsnotify.Create events. It will then start a new tailfile() goroutine whenever that happens.
2.1 monitor_file() will also create a time-is-up goroutine which will triger a cancelation of the watch
3. when time is up monitor_file() will wait for a log count from each of the tailfile() goruotines sequentially.
*/

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
					Log("ERROR:", err)
				}
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

// simpler plan:
/*
1. the logic for tailing files will be taken out of monitor_file() and extracted into it's own goroutine (tailfile()). It will output a count on a channel when time is up
2. monitor_file() will watch its log file for fsnotify.Create events. It will then start a new tailfile() goroutine whenever that happens.
2.1 monitor_file() will also create a time-is-up goroutine which will triger a cancelation of the watch
3. when time is up monitor_file() will wait for a log count from each of the tailfile() goruotines sequentially.
*/

func monitor_file(messageFileName string) {

	// first get the config (contains which container to track and for how long)

	log.Println("")

	configFile, err := tail.TailFile(messageFileName, tail.Config{Follow: true, MustExist: true, Poll: false})
	if err != nil {
		FLBLogger.Printf("ERROR: error opening file: %s\n", err.Error())
	}
	defer configFile.Cleanup()

	configFileFirstLineStruct := (<-configFile.Lines)

	if configFileFirstLineStruct.Err != nil {
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
	start_time, err := time.Parse(time.RFC3339, firstMessage.StartTime)
	if err != nil {
		FLBLogger.Printf("ERROR: error parsing start time: %s, (error: %s)\n", firstMessage.StartTime, err.Error())
		return
	}

	// when to stop counting logs
	end_time, err := time.Parse(time.RFC3339, firstMessage.EndTime)
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

	count_channel_list := make([]chan int, 1)
	count_channel_list = append(count_channel_list, watch_specific_log_file(filename, start_time, end_time))
	var timeup_channel chan bool = make(chan bool)
	go wait_then_stop(end_time, timeup_channel)

	// yes there is a race condition here (between tailing the initial log file and setting up the watch. Fix if this ever is productionized)

	/////////

	Log("setting up watcher")
	watcher, err := fsnotify.NewWatcher()
	if err != nil {
		Log("ERROR: could not create watcher", err.Error())
	}
	defer watcher.Close()

	Log("adding directory to watcher")
	err = watcher.Add(filename)
	if err != nil {
		Log("ERROR: could not watch log file %s", err.Error())
	}

outerloop:
	for {
		select {
		case event, ok := <-watcher.Events:
			Log("got watcher event %v, %v", event, ok)
			if !ok {
				Log("ERROR:", err)
				return
			}
			if event.Op&fsnotify.Create == fsnotify.Create {
				Log("new log file (was rotated): " + event.Name)
				count_channel_list = append(count_channel_list, watch_specific_log_file(filename, start_time, end_time))
			}
		case err, ok := <-watcher.Errors:
			if !ok {
				Log("ERROR:", err)
			}
		case <-timeup_channel:
			Log("done watching")
			break outerloop
		}
	}

	var linecount int = 0

	for i, count_chan := range count_channel_list {
		Log("waiting for response from a chanel %d", i)
		for count := range count_chan {
			Log("got response from chanel %d: %d lines", i, count)
			linecount += count
		}
	}

	////////

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

// This tails a specific log file. It will not tail the new log file after rotation
func watch_specific_log_file(filename string, start_time time.Time, end_time time.Time) chan int {

	count_return_channel := make(chan int)

	go func() {
		var line_count int

		Log("opening container log file " + filename)

		container_tailer, err := tail.TailFile(filename, tail.Config{Follow: true, MustExist: true, Poll: false, Logger: FLBLogger})
		if err != nil {
			FLBLogger.Printf("ERROR: error opening file: %s\n", err.Error())
			return
		}
		defer container_tailer.Stop()
		defer container_tailer.Cleanup()

		for line := range container_tailer.Lines {
			if line.Err != nil {
				FLBLogger.Printf("ERROR: error reading file: %s\n", err.Error())
			}
			// Log("read line from log file %s", line.Text)

			logTime, err := time.Parse(time.RFC3339, get_first_word(line.Text))
			if err != nil {
				FLBLogger.Printf("ERROR: error parsing time: %s\n", err.Error())
				FLBLogger.Printf("ERROR: line was \"%s\"\n", line.Text)
			}

			if !logTime.Before(start_time) {
				if logTime.Before(end_time) {
					line_count += 1
				} else {
					break
				}
			}
		}

		Log("finished tailing container log file " + filename + ", read " + fmt.Sprint(line_count) + " log lines\n")
		count_return_channel <- line_count
		close(count_return_channel)
	}()

	return count_return_channel
}

func wait_then_stop(timeToStop time.Time, return_channel chan bool) {
	time.Sleep(time.Until(timeToStop))
	time.Sleep(5 * time.Second) // do an extra sleep for a few seconds
	return_channel <- true
}

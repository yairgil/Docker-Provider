package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"strings"
	"sync"
	"time"

	"github.com/fluent/fluent-bit-go/output"
	"github.com/mitchellh/mapstructure"
	lumberjack "gopkg.in/natefinch/lumberjack.v2"
	metav1 "k8s.io/apimachinery/pkg/apis/meta/v1"
	"k8s.io/client-go/kubernetes"
	"k8s.io/client-go/rest"
)

// DataType for Container Log
const DataType = "CONTAINER_LOG_BLOB"

// IPName for Container Log
const IPName = "Containers"

var (
	// PluginConfiguration the plugins configuration
	PluginConfiguration map[string]string
	// HTTPClient for making POST requests to OMSEndpoint
	HTTPClient http.Client
	// OMSEndpoint ingestion endpoint
	OMSEndpoint string
	// Computer (Hostname) when ingesting into ContainerLog table
	Computer string
)

var (
	// ImageIDMap caches the container id to image mapping
	ImageIDMap map[string]string
	// NameIDMap caches the container it to Name mapping
	NameIDMap map[string]string
	// IgnoreIDSet set of  container Ids of kube-system pods
	IgnoreIDSet map[string]bool
)

var (
	// FLBLogger stream
	FLBLogger = createLogger()

	// Log wrapper function
	Log = FLBLogger.Printf
)

// Log wrapper to Log with timestamp
// func Log(format string, v ...interface{}) {
// 	FLBLogger.SetPrefix(time.Now().Format("2006-01-02 15:04:05") + " : ")
// 	FLBLogger.Printf(format, v)
// }

const containerInventoryPath = "/var/opt/microsoft/docker-cimprov/state/ContainerInventory"

// ContainerInventory represents the container info
type ContainerInventory struct {
	ElementName       string `json:"ElementName"`
	CreatedTime       string `json:"CreatedTime"`
	State             string `json:"State"`
	ExitCode          int    `json:"ExitCode"`
	StartedTime       string `json:"StartedTime"`
	FinishedTime      string `json:"FinishedTime"`
	ImageID           string `json:"ImageId"`
	Image             string `json:"Image"`
	Repository        string `json:"Repository"`
	ImageTag          string `json:"ImageTag"`
	ComposeGroup      string `json:"ComposeGroup"`
	ContainerHostname string `json:"ContainerHostname"`
	Computer          string `json:"Computer"`
	Command           string `json:"Command"`
	EnvironmentVar    string `json:"EnvironmentVar"`
	Ports             string `json:"Ports"`
	Links             string `json:"Links"`
}

// DataItem represents the object corresponding to the json that is sent by fluentbit tail plugin
type DataItem struct {
	LogEntry          string `json:"LogEntry"`
	LogEntrySource    string `json:"LogEntrySource"`
	LogEntryTimeStamp string `json:"LogEntryTimeStamp"`
	ContainerID       string `json:"ContainerId"`
	Image             string `json:"Image"`
	Name              string `json:"Name"`
	SourceSystem      string `json:"SourceSystem"`
	Computer          string `json:"Computer"`
	Filepath          string `json:"Filepath"`
}

// ContainerLogBlob represents the object corresponding to the payload that is sent to the ODS end point
type ContainerLogBlob struct {
	DataType  string     `json:"DataType"`
	IPName    string     `json:"IPName"`
	DataItems []DataItem `json:"DataItems"`
}

func populateMaps() {

	Log("Updating ImageIDMap and NameIDMap")
	var mutex = &sync.Mutex{}
	files, err := ioutil.ReadDir(containerInventoryPath)

	if err != nil {
		Log("error when reading container inventory %s\n", err.Error())
	}

	mutex.Lock()
	ImageIDMap = make(map[string]string)
	NameIDMap = make(map[string]string)

	for _, file := range files {
		fullPath := fmt.Sprintf("%s/%s", containerInventoryPath, file.Name())
		fileContent, err := ioutil.ReadFile(fullPath)
		if err != nil {
			Log("Error reading file content %s", fullPath)
			Log(err.Error())
		}
		var containerInventory ContainerInventory
		unmarshallErr := json.Unmarshal(fileContent, &containerInventory)

		if unmarshallErr != nil {
			Log("Unmarshall error when reading file %s %s \n", fullPath, unmarshallErr.Error())
		}

		ImageIDMap[file.Name()] = containerInventory.Image
		NameIDMap[file.Name()] = containerInventory.ElementName
	}
	mutex.Unlock()
}

func createLogger() *log.Logger {

	var logfile *os.File
	path := "/var/log/fluent-bit-out-oms-runtime.log"
	if _, err := os.Stat(path); err == nil {
		fmt.Printf("File Exists. Opening file in append mode...\n")
		logfile, err = os.OpenFile(path, os.O_APPEND|os.O_WRONLY, 0600)
		if err != nil {
			fmt.Printf(err.Error())
		}
	}

	if _, err := os.Stat(path); os.IsNotExist(err) {
		fmt.Printf("File Doesnt Exist. Creating file...\n")
		logfile, err = os.Create(path)
		if err != nil {
			fmt.Printf(err.Error())
		}
	}

	logger := log.New(logfile, "", 0)

	logger.SetOutput(&lumberjack.Logger{
		Filename:   path,
		MaxSize:    10, //megabytes
		MaxBackups: 3,
		MaxAge:     28,   //days
		Compress:   true, // false by default
	})

	logger.SetFlags(log.Ltime | log.Lshortfile | log.LstdFlags)
	return logger
}

func initMaps() {
	ImageIDMap = make(map[string]string)
	NameIDMap = make(map[string]string)

	populateMaps()

	for range time.Tick(time.Second * 60) {
		populateMaps()
	}
}

func updateIgnoreContainerIds() {
	IgnoreIDSet = make(map[string]bool)

	updateKubeSystemContainerIDs()

	for range time.Tick(time.Second * 300) {
		updateKubeSystemContainerIDs()
	}
}

func updateKubeSystemContainerIDs() {

	var mutex = &sync.Mutex{}
	mutex.Lock()
	IgnoreIDSet = make(map[string]bool)

	if strings.Compare(os.Getenv("DISABLE_KUBE_SYSTEM_LOG_COLLECTION"), "true") != 0 {
		Log("Kube System Log Collection is ENABLED.")
		return
	}

	Log("Kube System Log Collection is DISABLED. Collecting containerIds to drop their records")
	config, err := rest.InClusterConfig()
	if err != nil {
		Log("Error getting config %s\n", err.Error())
	}

	clientset, err := kubernetes.NewForConfig(config)
	if err != nil {
		Log("Error getting clientset %s", err.Error())
	}

	pods, err := clientset.CoreV1().Pods("kube-system").List(metav1.ListOptions{})
	if err != nil {
		Log("Error getting pods %s\n", err.Error())
	}

	for _, pod := range pods.Items {
		for _, status := range pod.Status.ContainerStatuses {
			IgnoreIDSet[status.ContainerID[9:len(status.ContainerID)]] = true
		}
	}
	mutex.Unlock()
}

// PostDataHelper sends data to the OMS endpoint
func PostDataHelper(tailPluginRecords []map[interface{}]interface{}) int {

	start := time.Now()
	var dataItems []DataItem
	for _, record := range tailPluginRecords {

		//id := toString(record["Id"])
		containerID := getContainerIDFromFilePath(toString(record["Filepath"]))

		// if Id is in the list of Ids to drop  (Kube-system containers) continue
		if containsKey(IgnoreIDSet, containerID) {
			continue
		}

		var dataItem DataItem
		stringMap := make(map[string]string)

		// convert map[interface{}]interface{} to  map[string]string
		for key, value := range record {
			strKey := fmt.Sprintf("%v", key)
			strValue := toString(value)
			stringMap[strKey] = strValue
		}

		stringMap["Image"] = ImageIDMap[containerID]
		stringMap["Name"] = NameIDMap[containerID]
		stringMap["Computer"] = Computer
		mapstructure.Decode(stringMap, &dataItem)
		dataItems = append(dataItems, dataItem)
	}

	if len(dataItems) > 0 {
		logEntry := ContainerLogBlob{
			DataType:  DataType,
			IPName:    IPName,
			DataItems: dataItems}

		marshalled, err := json.Marshal(logEntry)
		req, _ := http.NewRequest("POST", OMSEndpoint, bytes.NewBuffer(marshalled))
		req.Header.Set("Content-Type", "application/json")

		resp, err := HTTPClient.Do(req)
		if err != nil {
			Log("Error when sending request %s \n", err.Error())
		}

		if resp == nil || resp.StatusCode != 200 {
			return output.FLB_RETRY
		}

		elapsed := time.Since(start)
		Log("Successfully flushed %d records in %s", len(dataItems), elapsed)
	}

	return output.FLB_OK
}

func containsKey(currentMap map[string]bool, key string) bool {
	_, c := currentMap[key]
	return c
}

func readConfig() {
	workspaceIDFile := "/shared/data/workspaceId"
	workspaceID, err := ioutil.ReadFile(workspaceIDFile)
	if err != nil {
		Log("Error when reading workspaceId file")
	}

	OMSEndpoint = fmt.Sprintf("https://%s.ods.opinsights.azure.com/OperationalData.svc/PostJsonDataItems", strings.TrimSpace(string(workspaceID)))
	Log("OMSEndpoint %s \n\n", OMSEndpoint)

	containerHostNameFile := "/var/opt/microsoft/docker-cimprov/state/containerhostname"
	containerHostName, err := ioutil.ReadFile(containerHostNameFile)
	if err != nil {
		Log("Error when reading containerHostName file")
	}

	Computer = toString(containerHostName)
	Log("Computer == %s \n\n", Computer)
}

func toString(s interface{}) string {
	value := s.([]uint8)
	return string([]byte(value[:]))
}

func getContainerIDFromFilePath(filepath string) string {
	start := strings.LastIndex(filepath, "-")
	end := strings.LastIndex(filepath, ".")
	return filepath[start+1 : end]
}

// ReadConfig reads and populates plugin configuration
func ReadConfig(pluginConfPath string) map[string]string {

	pluginConf, err := ReadConfiguration(pluginConfPath)
	omsadminConf, err := ReadConfiguration(pluginConf["omsadmin_conf_path"])

	if err != nil {
		Log(err.Error())
	}

	containerHostName, err := ioutil.ReadFile(pluginConf["container_host_file_path"])
	if err != nil {
		Log("Error when reading containerHostName file %s", err.Error())
	}

	Computer = toString(containerHostName)
	Log("Computer == %s \n", Computer)

	OMSEndpoint = omsadminConf["OMS_ENDPOINT"]
	Log("OMSEndpoint %s", OMSEndpoint)

	return pluginConf
}

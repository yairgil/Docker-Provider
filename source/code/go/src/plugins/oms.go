package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	//"net"
	"os"
	"strconv"
	"strings"
	"sync"
	"time"

	"github.com/fluent/fluent-bit-go/output"

	lumberjack "gopkg.in/natefinch/lumberjack.v2"

	metav1 "k8s.io/apimachinery/pkg/apis/meta/v1"
	"k8s.io/client-go/kubernetes"
	"k8s.io/client-go/rest"
	/*"github.com/vmihailenco/msgpack"
	"github.com/fluent/fluent-logger-golang/fluent"*/
	"github.com/tinylib/msgp/msgp"
)

// DataType for Container Log
const DataType = "CONTAINER_LOG_BLOB"

// ContainerLogPluginConfFilePath --> config file path for container log plugin
const ContainerLogPluginConfFilePath = "/etc/opt/microsoft/docker-cimprov/out_oms.conf"

// IPName for Container Log
const IPName = "Containers"
const defaultContainerInventoryRefreshInterval = 60
const defaultKubeSystemContainersRefreshInterval = 300

var (
	// PluginConfiguration the plugins configuration
	PluginConfiguration map[string]string
	// HTTPClient for making POST requests to OMSEndpoint
	HTTPClient http.Client
	// OMSEndpoint ingestion endpoint
	OMSEndpoint string
	// Computer (Hostname) when ingesting into ContainerLog table
	Computer string
	// WorkspaceID log analytics workspace id
	WorkspaceID string
	// Fluent Client
	/* FluentClient *fluent.Fluent */

	//TCP Client for MDSD
	//TCPClient net.Conn
)

var (
	// ImageIDMap caches the container id to image mapping
	ImageIDMap map[string]string
	// NameIDMap caches the container it to Name mapping
	NameIDMap map[string]string
	// IgnoreIDSet set of  container Ids of kube-system pods
	IgnoreIDSet map[string]bool
	// DataUpdateMutex read and write mutex access to the container id set
	DataUpdateMutex = &sync.Mutex{}
	// ClientSet for querying KubeAPIs
	ClientSet *kubernetes.Clientset
)

var (
	// KubeSystemContainersRefreshTicker updates the kube-system containers
	KubeSystemContainersRefreshTicker = time.NewTicker(time.Second * 300)
	// ContainerImageNameRefreshTicker updates the container image and names periodically
	ContainerImageNameRefreshTicker = time.NewTicker(time.Second * 60)
)

var (
	// FLBLogger stream
	FLBLogger = createLogger()
	// Log wrapper function
	Log = FLBLogger.Printf
)

// DataItem represents the object corresponding to the json that is sent by fluentbit tail plugin
type DataItem struct {
	LogEntry          string `json:"LogEntry"`
	LogEntrySource    string `json:"LogEntrySource"`
	LogEntryTimeStamp string `json:"LogEntryTimeStamp"`
	ID                string `json:"Id"`
	Image             string `json:"Image"`
	Name              string `json:"Name"`
	SourceSystem      string `json:"SourceSystem"`
	Computer          string `json:"Computer"`
}

// ContainerLogBlob represents the object corresponding to the payload that is sent to the ODS end point
type ContainerLogBlob struct {
	DataType  string     `json:"DataType"`
	IPName    string     `json:"IPName"`
	DataItems []DataItem `json:"DataItems"`
}

type Entry struct {
	Time   int64 `msg:"time"`
	Record map[string]string `msg:"record"`
}

//msgp:tuple Forward
type Forward struct {
	Tag     string `msg:"tag"`     
	Entries []Entry `msg:"entries"`    
	//Option  interface{}
}




func createLogger() *log.Logger {
	var logfile *os.File
	path := "/var/opt/microsoft/docker-cimprov/log/fluent-bit-out-oms-runtime.log"
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
		MaxBackups: 1,
		MaxAge:     28,   //days
		Compress:   true, // false by default
	})

	logger.SetFlags(log.Ltime | log.Lshortfile | log.LstdFlags)
	return logger
}

func updateContainerImageNameMaps() {
	for ; true; <-ContainerImageNameRefreshTicker.C {
		Log("Updating ImageIDMap and NameIDMap")

		_imageIDMap := make(map[string]string)
		_nameIDMap := make(map[string]string)

		pods, err := ClientSet.CoreV1().Pods("").List(metav1.ListOptions{})
		if err != nil {
			Log("Error getting pods %s\nIt is ok to log here and continue, because the logs will be missing image and Name, but the logs will still have the containerID", err.Error())
			continue
		}

		for _, pod := range pods.Items {
			for _, status := range pod.Status.ContainerStatuses {
				lastSlashIndex := strings.LastIndex(status.ContainerID, "/")
				containerID := status.ContainerID[lastSlashIndex+1 : len(status.ContainerID)]
				image := status.Image
				name := fmt.Sprintf("%s/%s", pod.UID, status.Name)
				if containerID != "" {
					_imageIDMap[containerID] = image
					_nameIDMap[containerID] = name
				}
			}
		}

		Log("Locking to update image and name maps")
		DataUpdateMutex.Lock()
		ImageIDMap = _imageIDMap
		NameIDMap = _nameIDMap
		DataUpdateMutex.Unlock()
		Log("Unlocking after updating image and name maps")
	}
}

func updateKubeSystemContainerIDs() {
	for ; true; <-KubeSystemContainersRefreshTicker.C {
		if strings.Compare(os.Getenv("DISABLE_KUBE_SYSTEM_LOG_COLLECTION"), "true") != 0 {
			Log("Kube System Log Collection is ENABLED.")
			return
		}

		Log("Kube System Log Collection is DISABLED. Collecting containerIds to drop their records")

		pods, err := ClientSet.CoreV1().Pods("kube-system").List(metav1.ListOptions{})
		if err != nil {
			Log("Error getting pods %s\nIt is ok to log here and continue. Kube-system logs will be collected", err.Error())
			continue
		}

		_ignoreIDSet := make(map[string]bool)
		for _, pod := range pods.Items {
			for _, status := range pod.Status.ContainerStatuses {
				lastSlashIndex := strings.LastIndex(status.ContainerID, "/")
				_ignoreIDSet[status.ContainerID[lastSlashIndex+1:len(status.ContainerID)]] = true
			}
		}

		Log("Locking to update kube-system container IDs")
		DataUpdateMutex.Lock()
		IgnoreIDSet = _ignoreIDSet
		DataUpdateMutex.Unlock()
		Log("Unlocking after updating kube-system container IDs")
	}
}

// PostDataHelper sends data to the OMS endpoint
func PostDataHelper(tailPluginRecords []map[interface{}]interface{}) int {

	defer DataUpdateMutex.Unlock()

	start := time.Now()
	var dataItems []DataItem
	var entries []Entry

	DataUpdateMutex.Lock()

	//payLoad = append(payLoad, ([]byte (fmt.Sprintf("[\"%s\",[", "vishwas.containerlog")))...)

	for _, record := range tailPluginRecords {

		containerID := GetContainerIDFromFilePath(toString(record["filepath"]))

		if containerID == "" || containsKey(IgnoreIDSet, containerID) {
			continue
		}

		stringMap := make(map[string]string)

		stringMap["LogEntry"] = toString(record["log"])
		stringMap["LogEntrySource"] = toString(record["stream"])
		stringMap["LogEntryTimeStamp"] = toString(record["time"])
		stringMap["SourceSystem"] = "Containers"
		stringMap["Id"] = containerID

		if val, ok := ImageIDMap[containerID]; ok {
			stringMap["Image"] = val
		} else {
			Log("ContainerId %s not present in Map ", containerID)
		}

		if val, ok := NameIDMap[containerID]; ok {
			stringMap["Name"] = val
		} else {
			Log("ContainerId %s not present in Map ", containerID)
		}

		dataItem := DataItem{
			ID:                stringMap["Id"],
			LogEntry:          stringMap["LogEntry"],
			LogEntrySource:    stringMap["LogEntrySource"],
			LogEntryTimeStamp: stringMap["LogEntryTimeStamp"],
			SourceSystem:      stringMap["SourceSystem"],
			Computer:          Computer,
			Image:             stringMap["Image"],
			Name:              stringMap["Name"],
		}

		dataItems = append(dataItems, dataItem)
		/*
		//FluentClient.Post("vishwas.containerlog",dataItem)
		//FluentClient.EncodeAndPostData("vishwas.containerlog", time.Now(), dataItem)
		error := FluentClient.PostWithTime("vishwas.containerlog", time.Now(), dataItems)
		if error != nil {
			Log(error.Error())
		  }
		  */
		//m, err := json.Marshal(dataItem)
		/*if err != nil {
			Log (" Error while marshaling dataItem %s", err.Error())
		} else {*/
			entry := Entry{
				Time: time.Now().Unix(),
				Record: stringMap}
			//if c == (len(tailPluginRecords) - 1) {
				entries = append (entries, entry)
			//}
		//}
	}

	if len(dataItems) > 0 {
		logEntry := ContainerLogBlob{
			DataType:  DataType,
			IPName:    IPName,
			DataItems: dataItems}

		marshalled, err := json.Marshal(logEntry)
		if err != nil {
			Log("Error while Marshalling log Entry: %s", err.Error())
			return output.FLB_OK
		}

		fluentForward := Forward{
			Tag:		"vishwas.containerlog",
			Entries:	entries}
		
		size := 1 + msgp.StringPrefixSize + len (fluentForward.Tag) + msgp.ArrayHeaderSize
		for i := range fluentForward.Entries {
			size += 1 + msgp.Int64Size + msgp.GuessSize(fluentForward.Entries[i].Record)
		}

		var b []byte

		b = msgp.Require(nil, size)
		b = append(b, 0x92)
		b = msgp.AppendString(b, fluentForward.Tag)
		b = msgp.AppendArrayHeader(b, uint32(len(fluentForward.Entries)))
		for i := range fluentForward.Entries {
			b = append(b, 0x92)
			b = msgp.AppendInt64(b, fluentForward.Entries[i].Time)
			//var e error
			b = msgp.AppendMapStrStr(b, fluentForward.Entries[i].Record)
			/*if e != nil {
				Log ("Error when marshaling to msgp %s", e.Error())
			}*/
		}
			
		//if TCPClient == nil {
			TCPClient := CreateMDSDClient()
		//}

		if (TCPClient != nil) {
			defer TCPClient.Close()
			TCPClient.SetDeadline(time.Now().Add(5 * time.Second))
			byts, err := TCPClient.Write(b)
			if err != nil {
				Log ("Error while writing to socket %s", err.Error())
			} else {
				Log ("Successfully wrote %d bytes to socket", byts)
			}
		}
			
		

		/*b, err := msgpack.Marshal(payLoad)
		if b == nil {
			Log ("Error b is nil")
		}
		if err!= nil {
			Log ("Error while Marshaling to messagePack: %s", err.Error)
		}*/

		/*var b bytes.Buffer
		enc := msgpack.NewEncoder(&b)
		e := enc.Encode(payLoad)
		if e != nil {
			Log("Error while encoding %s ", e.Error())
		}*/

		/*var f []byte
		e := msgpack.Unmarshal(b, &f)
		if e != nil {
			Log ("Error while unmarshaling from messagePack: %s")
		} else {
			Log ("Unmarshalled : %s", f )
		}*/

		/*error := FluentClient.write(b) */
		/*if TCPClient == nil {
			CreateMDSDClient()
		}

		if (TCPClient != nil) {
			byts, err := TCPClient.Write(b)
		//	byts, err := TCPClient.Write(b.Bytes())
			if err != nil {
				Log ("Error while writing to socket %s", err.Error())
			} else {
				Log ("Successfully wrote %d bytes to socket", byts)
			}
		}*/

		


		req, _ := http.NewRequest("POST", OMSEndpoint, bytes.NewBuffer(marshalled))
		req.Header.Set("Content-Type", "application/json")

		resp, err := HTTPClient.Do(req)
		elapsed := time.Since(start)

		if err != nil {
			Log("Error when sending request %s \n", err.Error())
			Log("Failed to flush %d records after %s", len(dataItems), elapsed)
			return output.FLB_RETRY
		}

		if resp == nil || resp.StatusCode != 200 {
			if resp != nil {
				Log("Status %s Status Code %d", resp.Status, resp.StatusCode)
			}
			return output.FLB_RETRY
		}

		numRecords := len(dataItems)
		Log("Successfully flushed %d records in %s", numRecords, elapsed)
		FlushedRecordsCount += float64(numRecords)
		FlushedRecordsTimeTaken += float64(elapsed / time.Millisecond)
	}

	return output.FLB_OK
}

func containsKey(currentMap map[string]bool, key string) bool {
	_, c := currentMap[key]
	return c
}

func toString(s interface{}) string {
	switch t := s.(type) {
	case []byte:
		// prevent encoding to base64
		return string(t)
	default:
		return ""
	}
}

// GetContainerIDFromFilePath Gets the container ID From the file Path
func GetContainerIDFromFilePath(filepath string) string {
	start := strings.LastIndex(filepath, "-")
	end := strings.LastIndex(filepath, ".")
	if start >= end || start == -1 || end == -1 {
		// This means the file is not a managed Kubernetes docker log file.
		// Drop all records from the file
		Log("File %s is not a Kubernetes managed docker log file. Dropping all records from the file", filepath)
		return ""
	}
	return filepath[start+1 : end]
}

// InitializePlugin reads and populates plugin configuration
func InitializePlugin(pluginConfPath string) {

	IgnoreIDSet = make(map[string]bool)
	ImageIDMap = make(map[string]string)
	NameIDMap = make(map[string]string)

	pluginConfig, err := ReadConfiguration(pluginConfPath)
	if err != nil {
		Log("Error Reading plugin config path : %s \n", err.Error())
		log.Fatalf("Error Reading plugin config path : %s \n", err.Error())
	}

	omsadminConf, err := ReadConfiguration(pluginConfig["omsadmin_conf_path"])
	if err != nil {
		Log(err.Error())
		log.Fatalf("Error Reading omsadmin configuration %s\n", err.Error())
	}
	OMSEndpoint = omsadminConf["OMS_ENDPOINT"]
	WorkspaceID = omsadminConf["WORKSPACE_ID"]
	Log("OMSEndpoint %s", OMSEndpoint)

	// Initialize image,name map refresh ticker
	containerInventoryRefreshInterval, err := strconv.Atoi(pluginConfig["container_inventory_refresh_interval"])
	if err != nil {
		Log("Error Reading Container Inventory Refresh Interval %s", err.Error())
		Log("Using Default Refresh Interval of %d s\n", defaultContainerInventoryRefreshInterval)
		containerInventoryRefreshInterval = defaultContainerInventoryRefreshInterval
	}
	Log("containerInventoryRefreshInterval = %d \n", containerInventoryRefreshInterval)
	ContainerImageNameRefreshTicker = time.NewTicker(time.Second * time.Duration(containerInventoryRefreshInterval))

	// Initialize Kube System Refresh Ticker
	kubeSystemContainersRefreshInterval, err := strconv.Atoi(pluginConfig["kube_system_containers_refresh_interval"])
	if err != nil {
		Log("Error Reading Kube System Container Ids Refresh Interval %s", err.Error())
		Log("Using Default Refresh Interval of %d s\n", defaultKubeSystemContainersRefreshInterval)
		kubeSystemContainersRefreshInterval = defaultKubeSystemContainersRefreshInterval
	}
	Log("kubeSystemContainersRefreshInterval = %d \n", kubeSystemContainersRefreshInterval)
	KubeSystemContainersRefreshTicker = time.NewTicker(time.Second * time.Duration(kubeSystemContainersRefreshInterval))

	// Populate Computer field
	containerHostName, err := ioutil.ReadFile(pluginConfig["container_host_file_path"])
	if err != nil {
		// It is ok to log here and continue, because only the Computer column will be missing,
		// which can be deduced from a combination of containerId, and docker logs on the node
		Log("Error when reading containerHostName file %s.\n It is ok to log here and continue, because only the Computer column will be missing, which can be deduced from a combination of containerId, and docker logs on the nodes\n", err.Error())
	}
	Computer = strings.TrimSuffix(toString(containerHostName), "\n")
	Log("Computer == %s \n", Computer)

	// Initialize KubeAPI Client
	config, err := rest.InClusterConfig()
	if err != nil {
		Log("Error getting config %s.\nIt is ok to log here and continue, because the logs will be missing image and Name, but the logs will still have the containerID", err.Error())
	}

	ClientSet, err = kubernetes.NewForConfig(config)
	if err != nil {
		Log("Error getting clientset %s.\nIt is ok to log here and continue, because the logs will be missing image and Name, but the logs will still have the containerID", err.Error())
	}

	PluginConfiguration = pluginConfig

	CreateHTTPClient()
	/*CreateFluentClient()*/
	//CreateMDSDClient()
	go updateKubeSystemContainerIDs()
	go updateContainerImageNameMaps()
}

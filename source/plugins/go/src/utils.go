package main

import (
	"bufio"
	"crypto/tls"
	"errors"
	"fmt"
	"io/ioutil"
	"log"
	"net"
	"net/http"
	"net/url"
	"os"
	"path/filepath"
	"strings"
	"sync"
	"time"

	"github.com/Azure/azure-kusto-go/kusto"
	"github.com/Azure/azure-kusto-go/kusto/ingest"
	"github.com/Azure/go-autorest/autorest/azure/auth"
	"github.com/tinylib/msgp/msgp"
)

// ReadConfiguration reads a property file
func ReadConfiguration(filename string) (map[string]string, error) {
	config := map[string]string{}

	if len(filename) == 0 {
		return config, nil
	}

	file, err := os.Open(filename)
	if err != nil {
		SendException(err)
		time.Sleep(30 * time.Second)
		fmt.Printf("%s", err.Error())
		return nil, err
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		currentLine := scanner.Text()
		if equalIndex := strings.Index(currentLine, "="); equalIndex >= 0 {
			if key := strings.TrimSpace(currentLine[:equalIndex]); len(key) > 0 {
				value := ""
				if len(currentLine) > equalIndex {
					value = strings.TrimSpace(currentLine[equalIndex+1:])
				}
				config[key] = value
			}
		}
	}

	if err := scanner.Err(); err != nil {
		SendException(err)
		time.Sleep(30 * time.Second)
		log.Fatalf("%s", err.Error())
		return nil, err
	}

	return config, nil
}

// CreateHTTPClient used to create the client for sending post requests to OMSEndpoint
func CreateHTTPClient() {
	var transport *http.Transport
	if IsAADMSIAuthMode {
		transport = &http.Transport{}
	} else {
		certFilePath := PluginConfiguration["cert_file_path"]
		keyFilePath := PluginConfiguration["key_file_path"]
		if IsWindows == false {
			certFilePath = fmt.Sprintf(certFilePath, WorkspaceID)
			keyFilePath = fmt.Sprintf(keyFilePath, WorkspaceID)
		}
		cert, err := tls.LoadX509KeyPair(certFilePath, keyFilePath)
		if err != nil {
			message := fmt.Sprintf("Error when loading cert %s", err.Error())
			SendException(message)
			time.Sleep(30 * time.Second)
			Log(message)
			log.Fatalf("Error when loading cert %s", err.Error())
		}

		tlsConfig := &tls.Config{
			Certificates: []tls.Certificate{cert},
		}

		tlsConfig.BuildNameToCertificate()
		transport = &http.Transport{TLSClientConfig: tlsConfig}
	}
	// set the proxy if the proxy configured
	if ProxyEndpoint != "" {
		proxyEndpointUrl, err := url.Parse(ProxyEndpoint)
		if err != nil {
			message := fmt.Sprintf("Error parsing Proxy endpoint %s", err.Error())
			SendException(message)
			// if we fail to read proxy secret, AI telemetry might not be working as well
			Log(message)
		} else {
			transport.Proxy = http.ProxyURL(proxyEndpointUrl)
		}
	}

	HTTPClient = http.Client{
		Transport: transport,
		Timeout:   30 * time.Second,
	}

	Log("Successfully created HTTP Client")
}

// ToString converts an interface into a string
func ToString(s interface{}) string {
	switch t := s.(type) {
	case []byte:
		// prevent encoding to base64
		return string(t)
	default:
		return ""
	}
}

//mdsdSocketClient to write msgp messages
func CreateMDSDClient(dataType DataType, containerType string) {
	mdsdfluentSocket := "/var/run/mdsd/default_fluent.socket"
	if containerType != "" && strings.Compare(strings.ToLower(containerType), "prometheussidecar") == 0 {
		mdsdfluentSocket = fmt.Sprintf("/var/run/mdsd-%s/default_fluent.socket", containerType)
	}
	switch dataType {
	case ContainerLogV2:
		if MdsdMsgpUnixSocketClient != nil {
			MdsdMsgpUnixSocketClient.Close()
			MdsdMsgpUnixSocketClient = nil
		}
		/*conn, err := fluent.New(fluent.Config{FluentNetwork:"unix",
		FluentSocketPath:"/var/run/mdsd/default_fluent.socket",
		WriteTimeout: 5 * time.Second,
		RequestAck: true}) */
		conn, err := net.DialTimeout("unix",
			mdsdfluentSocket, 10*time.Second)
		if err != nil {
			Log("Error::mdsd::Unable to open MDSD msgp socket connection for ContainerLogV2 %s", err.Error())
			//log.Fatalf("Unable to open MDSD msgp socket connection %s", err.Error())
		} else {
			Log("Successfully created MDSD msgp socket connection for ContainerLogV2: %s", mdsdfluentSocket)
			MdsdMsgpUnixSocketClient = conn
		}
	case KubeMonAgentEvents:
		if MdsdKubeMonMsgpUnixSocketClient != nil {
			MdsdKubeMonMsgpUnixSocketClient.Close()
			MdsdKubeMonMsgpUnixSocketClient = nil
		}
		conn, err := net.DialTimeout("unix",
			mdsdfluentSocket, 10*time.Second)
		if err != nil {
			Log("Error::mdsd::Unable to open MDSD msgp socket connection for KubeMon events %s", err.Error())
			//log.Fatalf("Unable to open MDSD msgp socket connection %s", err.Error())
		} else {
			Log("Successfully created MDSD msgp socket connection for KubeMon events:%s", mdsdfluentSocket)
			MdsdKubeMonMsgpUnixSocketClient = conn
		}
	case InsightsMetrics:
		if MdsdInsightsMetricsMsgpUnixSocketClient != nil {
			MdsdInsightsMetricsMsgpUnixSocketClient.Close()
			MdsdInsightsMetricsMsgpUnixSocketClient = nil
		}
		conn, err := net.DialTimeout("unix",
			mdsdfluentSocket, 10*time.Second)
		if err != nil {
			Log("Error::mdsd::Unable to open MDSD msgp socket connection for insights metrics %s", err.Error())
			//log.Fatalf("Unable to open MDSD msgp socket connection %s", err.Error())
		} else {
			Log("Successfully created MDSD msgp socket connection for Insights metrics %s", mdsdfluentSocket)
			MdsdInsightsMetricsMsgpUnixSocketClient = conn
		}
	}
}

//ADX client to write to ADX
func CreateADXClient() {

	if ADXIngestor != nil {
		ADXIngestor = nil
	}

	authConfig := auth.NewClientCredentialsConfig(AdxClientID, AdxClientSecret, AdxTenantID)

	client, err := kusto.New(AdxClusterUri, kusto.Authorization{Config: authConfig})
	if err != nil {
		Log("Error::mdsd::Unable to create ADX client %s", err.Error())
		//log.Fatalf("Unable to create ADX connection %s", err.Error())
	} else {
		Log("Successfully created ADX Client. Creating Ingestor...")
		ingestor, ingestorErr := ingest.New(client, AdxDatabaseName, "ContainerLogV2")
		if ingestorErr != nil {
			Log("Error::mdsd::Unable to create ADX ingestor %s", ingestorErr.Error())
		} else {
			ADXIngestor = ingestor
		}
	}
}

func ReadFileContents(fullPathToFileName string) (string, error) {
	return ReadFileContentsImpl(fullPathToFileName, ioutil.ReadFile)
}

func ReadFileContentsImpl(fullPathToFileName string, readfilefunc func(string) ([]byte, error)) (string, error) {
	fullPathToFileName = strings.TrimSpace(fullPathToFileName)
	if len(fullPathToFileName) == 0 {
		return "", errors.New("ReadFileContents::filename is empty")
	}
	content, err := readfilefunc(fullPathToFileName) //no need to close
	if err != nil {
		return "", errors.New("ReadFileContents::Unable to open file " + fullPathToFileName)
	} else {
		return strings.TrimSpace(string(content)), nil
	}
}

func isValidUrl(uri string) bool {
	uri = strings.TrimSpace(uri)
	if len(uri) == 0 {
		return false
	}
	u, err := url.Parse(uri)
	if err != nil || u.Scheme == "" || u.Host == "" {
		return false
	}
	return true
}

func convertMsgPackEntriesToMsgpBytes(fluentForwardTag string, msgPackEntries []MsgPackEntry) []byte {
	var msgpBytes []byte

	fluentForward := MsgPackForward{
		Tag:     fluentForwardTag,
		Entries: msgPackEntries,
	}
	//determine the size of msgp message
	msgpSize := 1 + msgp.StringPrefixSize + len(fluentForward.Tag) + msgp.ArrayHeaderSize
	for i := range fluentForward.Entries {
		msgpSize += 1 + msgp.Int64Size + msgp.GuessSize(fluentForward.Entries[i].Record)
	}

	//allocate buffer for msgp message
	msgpBytes = msgp.Require(nil, msgpSize)

	//construct the stream
	msgpBytes = append(msgpBytes, 0x92)
	msgpBytes = msgp.AppendString(msgpBytes, fluentForward.Tag)
	msgpBytes = msgp.AppendArrayHeader(msgpBytes, uint32(len(fluentForward.Entries)))
	batchTime := time.Now().Unix()
	for entry := range fluentForward.Entries {
		msgpBytes = append(msgpBytes, 0x92)
		msgpBytes = msgp.AppendInt64(msgpBytes, batchTime)
		msgpBytes = msgp.AppendMapStrStr(msgpBytes, fluentForward.Entries[entry].Record)
	}

	return msgpBytes
}

// includes files in subdirectories
// TODO: consider replacing this with a iterator of some sort, constructing the map of
// files all at once might use more memory than necessary
func GetSizeOfAllFilesInDir(root_dir string) map[string]int64 {
	output_map := make(map[string]int64)
	getSizeOfAllFilesInDirImpl(&root_dir, "", &output_map)
	return output_map
}

func getSizeOfAllFilesInDirImpl(root_dir *string, preceeding_dir string, storage_dict *map[string]int64) {
	// container_full_path := filepath.Join(preceeding_dir_segments)
	files_and_folders, err := ioutil.ReadDir(filepath.Join(*root_dir, preceeding_dir))
	if err != nil {
		Log("ERROR: reading dir " + err.Error())
		return
	}
	for _, next_file := range files_and_folders {
		file_name := filepath.Join(preceeding_dir, next_file.Name())
		if next_file.IsDir() {
			// need to recurse more
			getSizeOfAllFilesInDirImpl(root_dir, file_name, storage_dict)
		} else {
			(*storage_dict)[file_name] = next_file.Size()
		}
	}
}

/*
This data structure is sort of like a map except that
*/
type AddressableMap struct {
	log_counts            []int64
	container_identifiers []string
	free_list             []int
	string_to_arr_index   map[string]int
	management_mut        *sync.Mutex
	debug_mode            bool
}

func Make_AddressableMap() AddressableMap {
	retval := AddressableMap{}
	// default size is 300 because a single node is unlikely to have more than 300 containers. This way the data structure is unlikely to ever need
	// to stop and copy all the values.
	retval.log_counts = make([]int64, 0, 300)
	retval.container_identifiers = make([]string, 0, 300)
	retval.free_list = make([]int, 0, 300)
	retval.string_to_arr_index = make(map[string]int)
	retval.management_mut = &sync.Mutex{}
	retval.debug_mode = false // this turns on internal consistency checks

	return retval
}

// creates an entry for new containers (the second return value will be true if the container is new)
func (collection *AddressableMap) get(container_identifier string) (*int64, bool) {
	slice_index, container_seen := collection.string_to_arr_index[container_identifier]
	if !container_seen {
		collection.management_mut.Lock()
		defer collection.management_mut.Unlock()

		if len(collection.free_list) > 0 {
			slice_index = collection.free_list[len(collection.free_list)-1]
			collection.free_list = collection.free_list[:len(collection.free_list)-1]
			collection.log_counts[slice_index] = 0
			collection.container_identifiers[slice_index] = ""
		} else {
			collection.log_counts = append(collection.log_counts, 0)
			collection.container_identifiers = append(collection.container_identifiers, "")
			slice_index = len(collection.log_counts) - 1
		}
		collection.container_identifiers[slice_index] = container_identifier
		collection.string_to_arr_index[container_identifier] = slice_index
	}

	if collection.debug_mode {
		collection._verify_state()
	}

	return &collection.log_counts[slice_index], !container_seen
}

// note: this function silently returns if the passed container is not already stored
func (collection *AddressableMap) delete(container_identifier string) {
	collection.management_mut.Lock()
	defer collection.management_mut.Unlock()

	index, value_exists := collection.string_to_arr_index[container_identifier]
	if !value_exists {
		return
	}

	collection.log_counts[index] = -1
	collection.container_identifiers[index] = ""
	collection.free_list = append(collection.free_list, index)
	delete(collection.string_to_arr_index, container_identifier)

	if collection.debug_mode {
		collection._verify_state()
	}
}

func (collection *AddressableMap) len() int {
	collection.management_mut.Lock()
	defer collection.management_mut.Unlock()
	return len(collection.string_to_arr_index)
}

func (collection *AddressableMap) export_values() ([]string, []int64) {
	collection.management_mut.Lock()
	defer collection.management_mut.Unlock()

	identifiers := make([]string, 0, len(collection.container_identifiers))
	values := make([]int64, 0, len(collection.container_identifiers))

	for k, v := range collection.string_to_arr_index {
		identifiers = append(identifiers, k)
		values = append(values, collection.log_counts[v])
	}

	return identifiers, values
}

// This is just meant for use in tests
func (collection *AddressableMap) _verify_state() {
	// make sure string_to_arr_index, log_counts, and container_identifiers all have the same length
	if len(collection.log_counts) != len(collection.container_identifiers) {
		panic("AddressableMap: len(collection.log_counts) != len(collection.container_identifiers)")
	}
	if len(collection.log_counts)-len(collection.free_list) != len(collection.string_to_arr_index) {
		panic("AddressableMap: len(collection.log_counts) - len(collection.free_list) != len(collection.string_to_arr_index)")
	}

	// make sure the free list doesn't have duplicate values
	free_list_items := make(map[int]bool)
	for i := 0; i < len(collection.free_list); i++ {
		if _, seen := free_list_items[collection.free_list[i]]; seen {
			panic(fmt.Sprintf("free list has duplicate value %d at index %d", collection.free_list[i], i))
		}
		if collection.free_list[i] < 0 {
			panic(fmt.Sprintf("free list has illegal value %d at index %d", collection.free_list[i], i))
		}
		free_list_items[collection.free_list[i]] = true
	}

	// make sure all the entries in the free list actually are free and any entries not in the free list have legal values
	for i := 0; i < len(collection.container_identifiers); i++ {
		if _, should_be_free := free_list_items[i]; should_be_free {
			if collection.log_counts[i] != -1 {
				panic(fmt.Sprintf("freed value in log_counts isn't -1 (value is %d at index %d)", collection.log_counts[i], i))
			}
			if collection.container_identifiers[i] != "" {
				panic(fmt.Sprintf("freed value in container_identifiers is not empty string (value is %s at index %d)", collection.container_identifiers[i], i))
			}
		} else {
			if collection.log_counts[i] == -1 {
				panic(fmt.Sprintf("unfreed value in log_counts is -1 (value is %d at index %d)", collection.log_counts[i], i))
			}
			if collection.container_identifiers[i] == "" {
				panic(fmt.Sprintf("unfreed value in container_identifiers is empty string (value is %s at index %d)", collection.container_identifiers[i], i))
			}
		}
	}
}

func slice_contains_str(str_slice []string, target_str string) bool {
	for _, val := range str_slice {
		if val == target_str {
			return true
		}
	}
	return false
}

func is_linux() bool {
	osType := os.Getenv("OS_TYPE")
	return strings.Compare(strings.ToLower(osType), "windows") != 0
}

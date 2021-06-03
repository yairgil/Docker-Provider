package main

import (
	"bufio"
	"crypto/tls"
	"encoding/json"
	"errors"
	"fmt"
	"io/ioutil"
	"log"
	"net"
	"net/http"
	"net/url"
	"os"
	"regexp"
	"strconv"
	"strings"
	"time"

	"github.com/Azure/azure-kusto-go/kusto"
	"github.com/Azure/azure-kusto-go/kusto/ingest"
	"github.com/Azure/go-autorest/autorest/azure/auth"
	"github.com/tinylib/msgp/msgp"
)

type IMDSResponse struct {
	AccessToken  string `json:"access_token"`
	ClientID     string `json:"client_id"`
	ExpiresIn    string `json:"expires_in"`
	ExpiresOn    string `json:"expires_on"`
	ExtExpiresIn string `json:"ext_expires_in"`
	NotBefore    string `json:"not_before"`
	Resource     string `json:"resource"`
	TokenType    string `json:"token_type"`
}

//TODO: do we need this entire sturct? we don't need most of the fields here, maybe we can just grab the parts we want.
type AgentConfiguration struct {
	Configurations []struct {
		Configurationid string `json:"configurationId"`
		Etag            string `json:"eTag"`
		Op              string `json:"op"`
		Content         struct {
			Datasources []struct {
				Configuration struct {
					Extensionname string `json:"extensionName"`
				} `json:"configuration"`
				ID      string `json:"id"`
				Kind    string `json:"kind"`
				Streams []struct {
					Stream                string `json:"stream"`
					Solution              string `json:"solution"`
					Extensionoutputstream string `json:"extensionOutputStream"`
				} `json:"streams"`
				Sendtochannels []string `json:"sendToChannels"`
			} `json:"dataSources"`
			Channels []struct {
				Endpoint string `json:"endpoint"`
				ID       string `json:"id"`
				Protocol string `json:"protocol"`
			} `json:"channels"`
			Extensionconfigurations struct {
				Containerinsights []struct {
					ID        string   `json:"id"`
					Originids []string `json:"originIds"`
					//TODO: make this a map so that if more types are added in the future it won't break. Also test if removing a type breaks the json deserialization
					Outputstreams struct {
						LinuxPerfBlob                   string `json:"LINUX_PERF_BLOB"`
						ContainerInventoryBlob          string `json:"CONTAINER_INVENTORY_BLOB"`
						ContainerLogBlob                string `json:"CONTAINER_LOG_BLOB"`
						ContainerinsightsContainerlogv2 string `json:"CONTAINERINSIGHTS_CONTAINERLOGV2"`
						ContainerNodeInventoryBlob      string `json:"CONTAINER_NODE_INVENTORY_BLOB"`
						KubeEventsBlob                  string `json:"KUBE_EVENTS_BLOB"`
						KubeHealthBlob                  string `json:"KUBE_HEALTH_BLOB"`
						KubeMonAgentEventsBlob          string `json:"KUBE_MON_AGENT_EVENTS_BLOB"`
						KubeNodeInventoryBlob           string `json:"KUBE_NODE_INVENTORY_BLOB"`
						KubePodInventoryBlob            string `json:"KUBE_POD_INVENTORY_BLOB"`
						KubePvInventoryBlob             string `json:"KUBE_PV_INVENTORY_BLOB"`
						KubeServicesBlob                string `json:"KUBE_SERVICES_BLOB"`
						InsightsMetricsBlob             string `json:"INSIGHTS_METRICS_BLOB"`
					} `json:"outputStreams"`
				} `json:"ContainerInsights"`
			} `json:"extensionConfigurations"`
		} `json:"content"`
	} `json:"configurations"`
}

type IngestionTokenResponse struct {
	Configurationid    string `json:"configurationId"`
	Ingestionauthtoken string `json:"ingestionAuthToken"`
}

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
	transport := &http.Transport{TLSClientConfig: tlsConfig}
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
		ingestor, ingestorErr := ingest.New(client, "containerinsights", "ContainerLogV2")
		if ingestorErr != nil {
			Log("Error::mdsd::Unable to create ADX ingestor %s", ingestorErr.Error())
		} else {
			ADXIngestor = ingestor
		}
	}
}

func ReadFileContents(fullPathToFileName string) (string, error) {
	fullPathToFileName = strings.TrimSpace(fullPathToFileName)
	if len(fullPathToFileName) == 0 {
		return "", errors.New("ReadFileContents::filename is empty")
	}
	content, err := ioutil.ReadFile(fullPathToFileName) //no need to close
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

var IMDSToken string
var IMDSTokenExpiration int64

var configurationId string
var channelId string

var ingestionToken string
var ingestionTokenExpiration int64

func getIngestionToken() (authToken string, err error) {
	// check if the ingestion token has not been fetched yet or is expiring soon. If so then re-fetch it first.
	// TODO: re-fetch token in a new thread if it is about to expire (don't block the main thread)

	if IMDSToken == "" || IMDSTokenExpiration >= time.Now().Unix()-30 { // refresh the token 10 seconds before it expires
		IMDSToken, IMDSTokenExpiration, err = getAccessTokenFromIMDS(true) //TODO: read from an env variable? Or maybe from OS?
		if err != nil {
			message := fmt.Sprintf("Error on getAccessTokenFromIMDS  %s \n", err.Error())
			Log(message)
			SendException(message)
			return "", err
		}
	}

	// ignore agent configuration expiring, the configuration and channel IDs will never change (without creating an agent restart)
	if configurationId == "" || channelId == "" {
		configurationId, channelId, _, err = getAgentConfiguration(IMDSToken)
		if err != nil {
			message := fmt.Sprintf("Error getAgentConfiguration %s \n", err.Error())
			Log(message)
			SendException(message)
			return "", err
		}
	}

	if ingestionToken == "" || ingestionTokenExpiration >= time.Now().Unix()-10 {
		ingestionToken, ingestionTokenExpiration, err = getIngestionAuthToken(IMDSToken, configurationId, channelId)
		if err != nil {
			message := fmt.Sprintf("Error getIngestionAuthToken %s \n", err.Error())
			Log(message)
			SendException(message)
			return "", err
		}
	}

	return ingestionToken, nil
}

func getAccessTokenFromIMDS(fromfile bool) (string, int64, error) {
	Log("Info getAccessTokenFromIMDS: start")
	MCS_ENDPOINT := os.Getenv("MCS_ENDPOINT")
	imdsAccessToken := ""

	var responseBytes []byte
	var err error

	if fromfile {
		// only used in windows
		responseBytes, err = ioutil.ReadFile(IMDSTokenPath)
		if err != nil {
			Log("getAccessTokenFromIMDS: Could not read IMDS token from file", err)
			return imdsAccessToken, 0, err
		}
	} else {
		var msi_endpoint *url.URL
		msi_endpoint, err := url.Parse("http://169.254.169.254/metadata/identity/oauth2/token?api-version=2018-02-01&resource=https://" + MCS_ENDPOINT + "/")
		if err != nil {
			Log("getAccessTokenFromIMDS: Error creating IMDS endpoint URL: %s", err.Error())
			return imdsAccessToken, 0, err
		}
		req, err := http.NewRequest("GET", msi_endpoint.String(), nil)
		if err != nil {
			Log("getAccessTokenFromIMDS: Error creating HTTP request: %s", err.Error())
			return imdsAccessToken, 0, err
		}
		req.Header.Add("Metadata", "true")

		// Call managed services for Azure resources token endpoint
		var resp *http.Response = nil
		for i := 0; i < 3; i++ {
			client := &http.Client{}
			resp, err = client.Do(req)
			if err != nil {
				message := fmt.Sprintf("getAccessTokenFromIMDS: Error calling token endpoint: %s", err.Error())
				Log(message)
				SendException(message) // send the exception here because this error is not returned. The calling function will send any returned errors to telemetry.
				continue
			}
			//TODO: is this the best place to defer closing the response body?
			defer resp.Body.Close()

			Log("getAccessTokenFromIMDS: IMDS Response Status: %d", resp.StatusCode)
			if resp.StatusCode != 200 {
				message := fmt.Sprintf("getAccessTokenFromIMDS: IMDS Request failed with an error code : %d", resp.StatusCode)
				Log(message)
				SendException(message)
				continue
			}
			break // call succeeded, don't retry any more
		}
		if resp == nil {
			Log("getAccessTokenFromIMDS: IMDS Request ran out of retries")
			return imdsAccessToken, 0, err
		}

		// Pull out response body
		responseBytes, err = ioutil.ReadAll(resp.Body)
		if err != nil {
			Log("getAccessTokenFromIMDS: Error reading response body : %s", err.Error())
			return imdsAccessToken, 0, err
		}
	}

	// Unmarshall response body into struct
	var imdsResponse IMDSResponse
	err = json.Unmarshal(responseBytes, &imdsResponse)
	if err != nil {
		Log("getAccessTokenFromIMDS: Error unmarshalling the response: %s", err.Error())
		return imdsAccessToken, 0, err
	}
	imdsAccessToken = imdsResponse.AccessToken

	expiration, err := strconv.ParseInt(imdsResponse.ExpiresOn, 10, 64)
	if err != nil {
		Log("Error reading expiration time from response header body: %s", err.Error())

		// Log("getAccessTokenFromIMDS: HTTP response header")
		// for value := range FormatHeaderForPrinting(&resp.Header) {
		// 	Log("getAccessTokenFromIMDS: \t " + value)
		// }
		return imdsAccessToken, 0, err
	}

	Log("IMDS Token obtained: %s", imdsAccessToken)

	Log("Info getAccessTokenFromIMDS: end")

	return imdsAccessToken, expiration, nil
}

func getAgentConfiguration(imdsAccessToken string) (configurationId string, channelId string, expiration int64, err error) {
	Log("Info getAgentConfiguration: start")
	configurationId = ""
	channelId = ""
	apiVersion := "2020-08-01-preview"
	var amcs_endpoint *url.URL
	resourceId := os.Getenv("customResourceId")
	resourceRegion := os.Getenv("customRegion")
	MCS_ENDPOINT := os.Getenv("MCS_ENDPOINT")
	amcs_endpoint_string := fmt.Sprintf("https://%s.handler.control."+MCS_ENDPOINT+"%s/agentConfigurations?platform=linux&api-version=%s", resourceRegion, resourceId, apiVersion)
	amcs_endpoint, err = url.Parse(amcs_endpoint_string)

	var bearer = "Bearer " + imdsAccessToken
	// Create a new request using http
	req, err := http.NewRequest("GET", amcs_endpoint.String(), nil)
	if err != nil {
		message := fmt.Sprintf("Error creating HTTP request for AMCS endpoint: %s", err.Error())
		Log(message)
		return configurationId, channelId, 0, err
	}

	// add authorization header to the req
	req.Header.Add("Authorization", bearer)

	var resp *http.Response = nil

	for i := 0; i < 3; i++ {
		// Call managed services for Azure resources token endpoint
		client := &http.Client{}
		resp, err = client.Do(req)
		if err != nil {
			message := fmt.Sprintf("Error calling amcs endpoint: %s", err.Error())
			Log(message)
			SendException(message)
			continue
		}

		//TODO: is this the best place to defer closing the response body?
		defer resp.Body.Close()

		Log("getAgentConfiguration Response Status: %d", resp.StatusCode)
		if resp.StatusCode != 200 {
			message := fmt.Sprintf("getAgentConfiguration Request failed with an error code : %d", resp.StatusCode)
			Log(message)
			SendException(message)
			continue
		}
		break // call succeeded, don't retry any more
	}
	if resp == nil {
		Log("getAgentConfiguration Request ran out of retries")
		return configurationId, channelId, 0, err
	}

	// get content timeout
	expiration, err = getExpirationFromAmcsResponse(resp.Header)
	if err != nil {
		Log("getAgentConfiguration failed to parse auth token timeout")
		return configurationId, channelId, 0, err
	}
	Log(fmt.Sprintf("getAgentConfiguration: DCR expires at %d seconds (unix timestamp)", expiration))

	// Pull out response body
	//TODO: does responseBytes need to be closed?
	responseBytes, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		Log("Error reading response body from AMCS API call : %s", err.Error())
		return configurationId, channelId, expiration, err
	}

	// Unmarshall response body into struct
	var agentConfiguration AgentConfiguration
	err = json.Unmarshal(responseBytes, &agentConfiguration)
	if err != nil {
		Log("Error unmarshalling the response: %s", err.Error())
		return configurationId, channelId, expiration, err
	}

	if len(agentConfiguration.Configurations) == 0 {
		Log("Received empty agentConfiguration.Configurations array")
		return configurationId, channelId, expiration, err
	}

	if len(agentConfiguration.Configurations[0].Content.Channels) == 0 {
		Log("Received empty agentConfiguration.Configurations[0].Content.Channels")
		return configurationId, channelId, expiration, err
	}

	configurationId = agentConfiguration.Configurations[0].Configurationid
	channelId = agentConfiguration.Configurations[0].Content.Channels[0].ID

	Log("obtained configurationId: %s, channelId: %s", configurationId, channelId)

	Log("Info getAgentConfiguration: end")

	return configurationId, channelId, expiration, nil
}

func getIngestionAuthToken(imdsAccessToken string, configurationId string, channelId string) (ingestionAuthToken string, expiration int64, err error) {
	Log("Info getIngestionAuthToken: start")
	ingestionAuthToken = ""
	var amcs_endpoint *url.URL
	resourceId := os.Getenv("customResourceId")
	resourceRegion := os.Getenv("customRegion")
	MCS_ENDPOINT := os.Getenv("MCS_ENDPOINT")
	apiVersion := "2020-04-01-preview"
	var os_type string
	if os.Getenv("OS_TYPE") == "windows" {
		os_type = "windows"
	} else {
		os_type = "linux"
	}
	amcs_endpoint_string := fmt.Sprintf("https://%s.handler.control."+MCS_ENDPOINT+"%s/agentConfigurations/%s/channels/%s/issueIngestionToken?platform=%s&api-version=%s", resourceRegion, resourceId, configurationId, channelId, os_type, apiVersion)
	amcs_endpoint, err = url.Parse(amcs_endpoint_string)

	var bearer = "Bearer " + imdsAccessToken

	// Create a new request using http
	req, err := http.NewRequest("GET", amcs_endpoint.String(), nil)
	if err != nil {
		Log("Error creating HTTP request for AMCS endpoint: %s", err.Error())
		return ingestionAuthToken, 0, err
	}

	// add authorization header to the req
	req.Header.Add("Authorization", bearer)

	var resp *http.Response = nil

	for i := 0; i < 3; i++ {
		// Call managed services for Azure resources token endpoint
		client := &http.Client{}
		resp, err = client.Do(req)
		if err != nil {
			message := fmt.Sprintf("Error calling amcs endpoint for ingestion auth token: %s", err.Error())
			Log(message)
			SendException(message)
			resp = nil
			continue
		}

		//TODO: is this the best place to defer closing the response body?
		defer resp.Body.Close()

		Log("getIngestionAuthToken Response Status: %d", resp.StatusCode)
		if resp.StatusCode != 200 {
			message := fmt.Sprintf("getIngestionAuthToken Request failed with an error code : %d", resp.StatusCode)
			Log(message)
			SendException(message)
			resp = nil
			continue
		}
		break
	}

	if resp == nil {
		Log("ran out of retries calling AMCS for ingestion token")
		return ingestionAuthToken, 0, err
	}

	expiration, err = getExpirationFromAmcsResponse(resp.Header)
	if err != nil {
		Log("getIngestionAuthToken failed to parse auth token expiration time")
		return ingestionAuthToken, 0, err
	}
	Log("getIngestionAuthToken: token times out at time %d (unix timestamp)", expiration)

	// Pull out response body
	responseBytes, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		Log("Error reading response body from AMCS Ingestion API call : %s", err.Error())
		return ingestionAuthToken, expiration, err
	}

	// Unmarshall response body into struct
	var ingestionTokenResponse IngestionTokenResponse
	err = json.Unmarshal(responseBytes, &ingestionTokenResponse)
	if err != nil {
		Log("Error unmarshalling the response: %s", err.Error())
		return ingestionAuthToken, expiration, err
	}

	ingestionAuthToken = ingestionTokenResponse.Ingestionauthtoken

	// Log("ingestionAuthToken obtained: %s", ingestionAuthToken)

	Log("Info getIngestionAuthToken: end")
	return ingestionAuthToken, expiration, nil
}

var cacheControlHeaderRegex = regexp.MustCompile(`max-age=([0-9]+)`)

func getExpirationFromAmcsResponse(header http.Header) (bestBeforeTime int64, err error) {
	// Get a timeout from a AMCS HTTP response header
	timeout := 0

	cacheControlHeader, valueInMap := header["Cache-Control"]
	if !valueInMap {
		return 0, errors.New("Cache-Control not in passed header")
	}

	for _, entry := range cacheControlHeader {
		match := cacheControlHeaderRegex.FindStringSubmatch(entry)
		if len(match) == 2 {
			timeout, err = strconv.Atoi(match[1])
			if err != nil {
				Log("getIngestionAuthToken: error getting timeout from auth token. Header: " + strings.Join(cacheControlHeader, ","))
				return 0, err
			}

			bestBeforeTime = time.Now().Unix() + int64(timeout)

			return bestBeforeTime, nil
		}
	}

	return 0, errors.New("didn't find timeout in header")
}

// func FormatHeaderForPrinting(header *http.Header) chan string {
// 	ch := make(chan string)

// 	go func() {

// 		for key, value := range *header {
// 			if key == "access_token" {
// 				ch <- key + ": " + "<secret redacted>"
// 			} else {
// 				ch <- key + ": " + strings.Join(value, ",")
// 			}
// 		}
// 		close(ch)
// 	}()

// 	return ch
// }

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

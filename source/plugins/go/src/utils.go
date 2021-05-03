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
	"strings"
	"time"
    "encoding/json"

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
					ID            string   `json:"id"`
					Originids     []string `json:"originIds"`
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
	cert, err := tls.LoadX509KeyPair(PluginConfiguration["cert_file_path"], PluginConfiguration["key_file_path"])
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
func CreateMDSDClient() {
	if MdsdMsgpUnixSocketClient != nil {
		MdsdMsgpUnixSocketClient.Close()
		MdsdMsgpUnixSocketClient = nil
	}
	/*conn, err := fluent.New(fluent.Config{FluentNetwork:"unix",
	  FluentSocketPath:"/var/run/mdsd/default_fluent.socket",
	  WriteTimeout: 5 * time.Second,
	  RequestAck: true}) */
	conn, err := net.DialTimeout("unix",
		"/var/run/mdsd/default_fluent.socket", 10*time.Second)
	if err != nil {
		Log("Error::mdsd::Unable to open MDSD msgp socket connection %s", err.Error())
		//log.Fatalf("Unable to open MDSD msgp socket connection %s", err.Error())
	} else {
		Log("Successfully created MDSD msgp socket connection")
		MdsdMsgpUnixSocketClient = conn
	}
}

//mdsdSocketClient to write msgp messages for KubeMonAgent Events
func CreateMDSDClientKubeMon() {
	if MdsdKubeMonMsgpUnixSocketClient != nil {
		MdsdKubeMonMsgpUnixSocketClient.Close()
		MdsdKubeMonMsgpUnixSocketClient = nil
	}
	/*conn, err := fluent.New(fluent.Config{FluentNetwork:"unix",
	  FluentSocketPath:"/var/run/mdsd/default_fluent.socket",
	  WriteTimeout: 5 * time.Second,
	  RequestAck: true}) */
	conn, err := net.DialTimeout("unix",
		"/var/run/mdsd/default_fluent.socket", 10*time.Second)
	if err != nil {
		Log("Error::mdsd::Unable to open MDSD msgp socket connection for KubeMon events %s", err.Error())
		//log.Fatalf("Unable to open MDSD msgp socket connection %s", err.Error())
	} else {
		Log("Successfully created MDSD msgp socket connection for KubeMon events")
		MdsdKubeMonMsgpUnixSocketClient = conn
	}
}

//mdsdSocketClient to write msgp messages for KubeMonAgent Events
func CreateMDSDClientInsightsMetrics() {
	if MdsdInsightsMetricsMsgpUnixSocketClient != nil {
		MdsdInsightsMetricsMsgpUnixSocketClient.Close()
		MdsdInsightsMetricsMsgpUnixSocketClient = nil
	}
	/*conn, err := fluent.New(fluent.Config{FluentNetwork:"unix",
	  FluentSocketPath:"/var/run/mdsd/default_fluent.socket",
	  WriteTimeout: 5 * time.Second,
	  RequestAck: true}) */
	conn, err := net.DialTimeout("unix",
		"/var/run/mdsd/default_fluent.socket", 10*time.Second)
	if err != nil {
		Log("Error::mdsd::Unable to open MDSD msgp socket connection %s for insights metrics", err.Error())
		//log.Fatalf("Unable to open MDSD msgp socket connection %s", err.Error())
	} else {
		Log("Successfully created MDSD msgp socket connection for Insights metrics")
		MdsdInsightsMetricsMsgpUnixSocketClient = conn
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

func getAccessTokenFromIMDS() (string, error) {
	Log("Info getAccessTokenFromIMDS: start")
	imdsAccessToken := ""	

	var msi_endpoint *url.URL
    msi_endpoint, err := url.Parse("http://169.254.169.254/metadata/identity/oauth2/token?api-version=2018-02-01&resource=https://monitor.azure.com/")
    if err != nil {
		Log("Error creating IMDS endpoint URL: %s", err.Error())
       return imdsAccessToken, err 
    }  
    req, err := http.NewRequest("GET", msi_endpoint.String(), nil)
    if err != nil {
	  Log("Error creating HTTP request: %s", err.Error())
      return imdsAccessToken, err
    }
    req.Header.Add("Metadata", "true")

    // Call managed services for Azure resources token endpoint
    client := &http.Client{}
    resp, err := client.Do(req) 
    if err != nil{
		Log("Error calling token endpoint: %s", err.Error())
	  return imdsAccessToken, err
    }

	Log("IMDS Response Status: %d", resp.StatusCode)
	if resp.StatusCode != 200 {
		Log("IMDS Request failed with an error code : %d", resp.StatusCode)
		return imdsAccessToken, err
	}

    // Pull out response body
    responseBytes,err := ioutil.ReadAll(resp.Body)
    defer resp.Body.Close()
    if err != nil {
		Log("Error reading response body : %s", err.Error())
      return imdsAccessToken, err
    }

    // Unmarshall response body into struct
    var imdsResponse IMDSResponse
    err = json.Unmarshal(responseBytes, &imdsResponse)
    if err != nil {
		Log("Error unmarshalling the response: %s", err.Error())
		return imdsAccessToken, err
    }
	imdsAccessToken = imdsResponse.AccessToken
	
	Log("IMDS Token obtained: %s", imdsAccessToken)

	Log("Info getAccessTokenFromIMDS: end")

	return imdsAccessToken, nil 
}

func getAgentConfiguration(imdsAccessToken string) (configurationId string, channelId string,  err error) {
	Log("Info getAgentConfiguration: start")
	configurationId = ""
	channelId = ""
	apiVersion := "2020-08-01-preview"
	var amcs_endpoint *url.URL
	resourceId := os.Getenv("customResourceId")
	resourceRegion := os.Getenv("customRegion")
	amcs_endpoint_string := fmt.Sprintf("https://%s.handler.control.monitor.azure.com%s/agentConfigurations?platform=linux&api-version=%s", resourceRegion, resourceId, apiVersion)
	amcs_endpoint, err = url.Parse(amcs_endpoint_string)

	var bearer = "Bearer " + imdsAccessToken
	// Create a new request using http    
    req, err := http.NewRequest("GET", amcs_endpoint.String(), nil)
    if err != nil {
	  Log("Error creating HTTP request for AMCS endpoint: %s", err.Error())
      return configurationId, channelId, err
    }

	// add authorization header to the req
	req.Header.Add("Authorization", bearer)

	// Call managed services for Azure resources token endpoint
	client := &http.Client{}
	resp, err := client.Do(req) 
	if err != nil{
	   Log("Error calling amcs endpoint: %s", err.Error())
	   return configurationId, channelId, err
	}

	Log("getAgentConfiguration Response Status: %d", resp.StatusCode)
	if resp.StatusCode != 200 {
		Log("getAgentConfiguration Request failed with an error code : %d", resp.StatusCode) 
		return configurationId, channelId, err
	}

	// Pull out response body
    responseBytes, err := ioutil.ReadAll(resp.Body)
    defer resp.Body.Close()
    if err != nil {
		Log("Error reading response body from AMCS API call : %s", err.Error())
		return configurationId, channelId, err
    }

	// Unmarshall response body into struct
	var agentConfiguration AgentConfiguration
	err = json.Unmarshal(responseBytes, &agentConfiguration)
	if err != nil {
		 Log("Error unmarshalling the response: %s", err.Error())
		 return configurationId, channelId, err
	}

	if len(agentConfiguration.Configurations) == 0 {		
		Log("Received empty agentConfiguration.Configurations array")
		return configurationId, channelId, err
	}
	 
	if len(agentConfiguration.Configurations[0].Content.Channels) == 0 {		
		Log("Received empty agentConfiguration.Configurations[0].Content.Channels")
		return configurationId, channelId, err
	} 	

	configurationId = agentConfiguration.Configurations[0].Configurationid	
	channelId = agentConfiguration.Configurations[0].Content.Channels[0].ID

	Log("obtained configurationId: %s, channelId: %s", configurationId, channelId)

	Log("Info getAgentConfiguration: end")

 	return configurationId, channelId, nil  
}

func getIngestionAuthToken(imdsAccessToken string, configurationId string, channelId string) (ingestionAuthToken string, err error) { 
	Log("Info getIngestionAuthToken: start")
	ingestionAuthToken = ""
	var amcs_endpoint *url.URL
	resourceId := os.Getenv("customResourceId")
	resourceRegion := os.Getenv("customRegion")
	apiVersion := "2020-04-01-preview"	
	amcs_endpoint_string := fmt.Sprintf("https://%s.handler.control.monitor.azure.com%s/agentConfigurations/%s/channels/%s/issueIngestionToken?platform=linux&api-version=%s", resourceRegion, resourceId, configurationId, channelId, apiVersion)
	amcs_endpoint, err = url.Parse(amcs_endpoint_string)

	var bearer = "Bearer " + imdsAccessToken
	// Create a new request using http    
    req, err := http.NewRequest("GET", amcs_endpoint.String(), nil)
    if err != nil {
	  Log("Error creating HTTP request for AMCS endpoint: %s", err.Error())
      return ingestionAuthToken, err
    }

	// add authorization header to the req
	req.Header.Add("Authorization", bearer)

	// Call managed services for Azure resources token endpoint
	client := &http.Client{}
	resp, err := client.Do(req) 
	if err != nil{
	   Log("Error calling amcs endpoint for ingestion auth token: %s", err.Error())
	   return ingestionAuthToken, err
    }	

    Log("getIngestionAuthToken Response Status: %d", resp.StatusCode)
	
	if resp.StatusCode != 200 {
		Log("getIngestionAuthToken Request failed with an error code : %d", resp.StatusCode) 
		return ingestionAuthToken, err        
	}

	// Pull out response body
    responseBytes, err := ioutil.ReadAll(resp.Body)
    defer resp.Body.Close()
    if err != nil {
		Log("Error reading response body from AMCS Ingestion API call : %s", err.Error())
		return ingestionAuthToken, err    
    }

	 // Unmarshall response body into struct
	var ingestionTokenResponse IngestionTokenResponse
	err = json.Unmarshal(responseBytes, &ingestionTokenResponse)
	if err != nil {
		 Log("Error unmarshalling the response: %s", err.Error())
		 return ingestionAuthToken, err    
	}
	
	ingestionAuthToken = ingestionTokenResponse.Ingestionauthtoken

	Log("ingestionAuthToken obtained: %s", ingestionAuthToken)

	Log("Info getIngestionAuthToken: end")
 	return ingestionAuthToken, nil  
}

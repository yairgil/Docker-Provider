package main

import (
	"encoding/json"
	"errors"
	"fmt"
	"io/ioutil"
	"net/http"
	"net/url"
	"os"
	"regexp"
	"strconv"
	"strings"
	"time"
)

const IMDSTokenPathForWindows = "c:/etc/imds-access-token/token" // only used in windows

var IMDSToken string
var IMDSTokenExpiration int64

var ConfigurationId string
var ChannelId string

var IngestionAuthToken string
var IngestionAuthTokenExpiration int64

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

func getAccessTokenFromIMDS() (string, int64, error) {
	Log("Info getAccessTokenFromIMDS: start")	
	useIMDSTOkenProxyEndPoint := os.Getenv("USE_IMDS_TOKEN_PROXY_END_POINT")
	imdsAccessToken := ""
	var responseBytes []byte
	var err error
	
	if (useIMDSTOkenProxyEndPoint != "" && strings.Compare(strings.ToLower(useIMDSTOkenProxyEndPoint), "true") == 0) {
		Log("Info Reading IMDS Access Token from IMDS Token proxy endpoint")
		MCS_ENDPOINT := os.Getenv("MCS_ENDPOINT")
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
			// client := &http.Client{}
			resp, err = HTTPClient.Do(req)
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

	} else {
		Log("Info Reading IMDS Access Token from file : %s", IMDSTokenPathForWindows)
		responseBytes, err = ioutil.ReadFile(IMDSTokenPathForWindows)
		if err != nil {
			Log("getAccessTokenFromIMDS: Could not read IMDS token from file: %s", err.Error())
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
		Log("Error parsing ExpiresOn field from IMDS response: %s", err.Error())		
		return imdsAccessToken, 0, err
	}
	Log("Info getAccessTokenFromIMDS: end")
	return imdsAccessToken, expiration, nil
}

func getAgentConfiguration(imdsAccessToken string) (configurationId string, channelId string, err error) {
	Log("Info getAgentConfiguration: start")
	configurationId = ""
	channelId = ""	
	apiVersion := "2020-08-01-preview"
	var amcs_endpoint *url.URL
	osType := os.Getenv("OS_TYPE")
	resourceId := os.Getenv("AKS_RESOURCE_ID")
	resourceRegion := os.Getenv("AKS_REGION")
	MCS_ENDPOINT := os.Getenv("MCS_ENDPOINT")
	amcs_endpoint_string := fmt.Sprintf("https://%s.handler.control."+MCS_ENDPOINT+"%s/agentConfigurations?platform=%s&api-version=%s", resourceRegion, resourceId, osType, apiVersion)
	amcs_endpoint, err = url.Parse(amcs_endpoint_string)

	var bearer = "Bearer " + imdsAccessToken
	// Create a new request using http
	req, err := http.NewRequest("GET", amcs_endpoint.String(), nil)
	if err != nil {
		message := fmt.Sprintf("Error creating HTTP request for AMCS endpoint: %s", err.Error())
		Log(message)
		return configurationId, channelId, err
	}
	req.Header.Set("Authorization", bearer)

	var resp *http.Response = nil

	for i := 0; i < 3; i++ {	
		resp, err = HTTPClient.Do(req)
		if err != nil {
			message := fmt.Sprintf("Error calling AMCS endpoint: %s", err.Error())
			Log(message)
			SendException(message)
			continue
		}
		if resp != nil && resp.Body != nil {
			defer resp.Body.Close() 
	    }
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
		return configurationId, channelId, err
	}
	responseBytes, err := ioutil.ReadAll(resp.Body)
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

func getIngestionAuthToken(imdsAccessToken string, configurationId string, channelId string) (ingestionAuthToken string, expiration int64, err error) {
	Log("Info getIngestionAuthToken: start")
	ingestionAuthToken = ""
	refreshInterval = 0
	var amcs_endpoint *url.URL
	osType := os.Getenv("OS_TYPE")
	resourceId := os.Getenv("AKS_RESOURCE_ID")
	resourceRegion := os.Getenv("AKS_REGION")
	MCS_ENDPOINT := os.Getenv("MCS_ENDPOINT")
	apiVersion := "2020-04-01-preview"
	amcs_endpoint_string := fmt.Sprintf("https://%s.handler.control."+MCS_ENDPOINT+"%s/agentConfigurations/%s/channels/%s/issueIngestionToken?platform=%s&api-version=%s", resourceRegion, resourceId, configurationId, channelId, osType, apiVersion)
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
		resp, err = HTTPClient.Do(req)
		if err != nil {
			message := fmt.Sprintf("Error calling amcs endpoint for ingestion auth token: %s", err.Error())
			Log(message)
			SendException(message)
			resp = nil
			continue
		}
		
		if resp != nil && resp.Body != nil {
			defer resp.Body.Close() 
	    }

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
	
	// Pull out response body
	responseBytes, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		Log("Error reading response body from AMCS Ingestion API call : %s", err.Error())
		return ingestionAuthToken, refreshInterval, err
	}

	// Unmarshall response body into struct
	var ingestionTokenResponse IngestionTokenResponse
	err = json.Unmarshal(responseBytes, &ingestionTokenResponse)
	if err != nil {
		Log("Error unmarshalling the response: %s", err.Error())
		return ingestionAuthToken, refreshInterval, err
	}

	ingestionAuthToken = ingestionTokenResponse.Ingestionauthtoken

	refreshInterval, err = getTokenRefreshIntervalFromAmcsResponse(resp.Header)
	if err != nil {
		Log("getIngestionAuthToken failed to parse max-age response header")
		return ingestionAuthToken, 0, err
	}
	Log("getIngestionAuthToken: refresh interval %d seconds", refreshInterval)

	Log("Info getIngestionAuthToken: end")
	return ingestionAuthToken, refreshInterval, nil
}

var cacheControlHeaderRegex = regexp.MustCompile(`max-age=([0-9]+)`)

func getTokenRefreshIntervalFromAmcsResponse(header http.Header) (bestBeforeTime int64, err error) {
	refreshInterval := 0
	cacheControlHeader, valueInMap := header["Cache-Control"]
	if !valueInMap {
		return 0, errors.New("Cache-Control not in passed header")
	}

	for _, entry := range cacheControlHeader {
		match := cacheControlHeaderRegex.FindStringSubmatch(entry)
		if len(match) == 2 {
			refreshInterval, err = strconv.Atoi(match[1])
			if err != nil {
				Log("getIngestionAuthToken: error getting timeout from auth token. Header: " + strings.Join(cacheControlHeader, ","))
				return 0, err
			}		
			return refreshInterval, nil
		}
	}

	return 0, errors.New("didn't find timeout in header")
}

func refreshIngestionAuthToken() {
	for ; true; <-IngestionAuthTokenRefreshTicker.C { 
		var err error
		if IMDSToken == "" || IMDSTokenExpiration <= (time.Now().Unix() + 60 * 60) { // refresh the token 1 hr expiry
			imdsToken, imdsTokenExpiry, err = getAccessTokenFromIMDS() 
			if err != nil {
				message := fmt.Sprintf("Error on getAccessTokenFromIMDS  %s \n", err.Error())
				Log(message)
				SendException(message)				
			} else {
				IMDSToken = imdsToken
				IMDSTokenExpiration = imdsTokenExpiry
			}
		}		
		if IMDSToken == "" {
			message := "IMDSToken is empty"
			Log(message)
			SendException(message)
			continue 
		} 	
		// ignore agent configuration expiring, the configuration and channel IDs will never change (without creating an agent restart)
		if ConfigurationId == "" || ChannelId == "" {			
			ConfigurationId, ChannelId, err = getAgentConfiguration(IMDSToken)
			if err != nil {
				message := fmt.Sprintf("Error getAgentConfiguration %s \n", err.Error())
				Log(message)
				SendException(message)
				continue 
			}
		}
		if IMDSToken == "" ConfigurationId == "" || ChannelId == ""  {
			message := "IMDSToken or ConfigurationId or ChannelId empty"
			Log(message)
			SendException(message)
			continue 
		}
		ingestionAuthToken, refreshIntervalInSeconds, err = getIngestionAuthToken(IMDSToken, ConfigurationId, ChannelId)
		if err != nil {
			message := fmt.Sprintf("Error getIngestionAuthToken %s \n", err.Error())
			Log(message)
			SendException(message)
			continue 
		}				
		IngestionAuthTokenUpdateMutex.Lock()
		ODSIngestionAuthToken = ingestionAuthToken
		IngestionAuthTokenUpdateMutex.Unlock()
		if refreshIntervalInSeconds > 0 && refreshIntervalInSeconds != defaultIngestionAuthTokenRefreshIntervalSeconds {
			ticker.Reset(time.Second * refreshIntervalInSeconds)  
		}
	}
}

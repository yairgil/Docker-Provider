package main

import (
	"bufio"
	"crypto/tls"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"net/url"
	"os"
	"strings"
	"time"
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

	var proxyUrl *url.URL
	proxyConfigString := ReadProxyConfiguration(PluginConfiguration["omsproxy_conf_path"])
	if proxyConfigString != "" {	
		proxyConfigMap := ParseProxyConfiguration(proxyConfigString)	
		proxyAddr :=  proxyConfigMap["protocol"] + "://" + proxyConfigMap["addr"] + ":" + proxyConfigMap["port"]
		Log("Proxy address endpoint %s",proxyAddr)
		var parseError error
		proxyUrl, parseError = url.Parse(proxyAddr)	
		if parseError != nil {
				message := fmt.Sprintf("Error parsing omsproxy url %s\n", parseError.Error())
				Log(message)
				SendException(message)
				time.Sleep(30 * time.Second)
				log.Fatalln(message)
		} 		
	}
	
	transport := &http.Transport{TLSClientConfig: tlsConfig, Proxy: http.ProxyURL(proxyUrl)}

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

func ReadProxyConfiguration(proxyConfFile string) (string) {
	proxyConfigString := ""	
	if _, err := os.Stat(proxyConfFile); err == nil {
		Log("Proxy configuration file %s", proxyConfFile )
		omsproxyConf, err := ioutil.ReadFile(proxyConfFile)
		if err != nil {
			message := fmt.Sprintf("Error Reading omsproxy configuration %s\n", err.Error())
			Log(message)
			SendException(message)
			time.Sleep(30 * time.Second)
			log.Fatalln(message)
		} else {
			proxyConfigString = strings.TrimSpace(string(omsproxyConf))
			Log("proxy configuration %s", proxyConfigString)			
		}
	}

	return proxyConfigString
}

//format of proxyconfig string is http://user01:password@proxy01.contoso.com:8080
//TBD -- should be more sophisticated. refer parse_proxy_config in baseagent code for improvement
func ParseProxyConfiguration(proxyConfigString string) (map[string]string) {
	 configMap := make(map[string]string)
	 if proxyConfigString != "" {
		proxyConfigParts := strings.Split(proxyConfigString, "://")	
		if len(proxyConfigParts) > 1 {
		  configMap["protocol"] = proxyConfigParts[0] 
		  proxyConfigSubParts := strings.Split(proxyConfigParts[1], "@")
		  if len(proxyConfigParts) > 1 {
			 creds := strings.Split(proxyConfigSubParts[0], ":")
			 if len(creds) > 1 {
				configMap["user"] = creds[0]
				configMap["pass"] = creds[1]
			 }
			addressport := strings.Split(proxyConfigSubParts[1], ":")
			if len(addressport) > 1 {
				configMap["addr"] = addressport[0]
				configMap["port"] = addressport[1]
			 }
		  } 
		}
	}
	return configMap
}

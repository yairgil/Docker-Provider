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
func CreateHTTPClient(proxyConfigMap map[string]string) {
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
	if proxyConfigMap != nil && len(proxyConfigMap) > 0 {	
		//proxy url format is http://<user>:<pass>@<addr>:<port>
		proxyAddr :=  "http://" + proxyConfigMap["user"] + ":" + proxyConfigMap["pass"] + "@" + proxyConfigMap["addr"] + ":" + proxyConfigMap["port"]
		Log("Proxy address endpoint %s", proxyAddr)
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

// ReadProxyConfiguration reads the proxy config
func ReadProxyConfiguration(proxyConfFile string) (map[string]string, error) {
	proxyConfigMap := map[string]string{}	
	if len(proxyConfFile) == 0 {
		return nil, errors.New("error: proxy config file path is empty")
	}
	if _, err := os.Stat(proxyConfFile); err == nil {
		Log("Proxy configuration file %s", proxyConfFile )
		omsproxyConf, err := ioutil.ReadFile(proxyConfFile)
		if err != nil {			
			message := fmt.Sprintf("Error Reading omsproxy configuration %s\n", err.Error())
			Log(message)
			SendException(message)
			time.Sleep(30 * time.Second)
			log.Fatalln(message)

			return nil, err
		} else {
			proxyConfigString := strings.TrimSpace(string(omsproxyConf))	
			if proxyConfigString == "" {
				return nil,  errors.New("error: provided empty proxy configuration setting")
			}
			proxyConfigURL, err := url.Parse(proxyConfigString)	
			if err != nil {
				return nil, err
			}	

			protocol := proxyConfigURL.Scheme
			if strings.ToLower(protocol) != "http" && strings.ToLower(protocol) != "https" {
				return nil, errors.New("error: only supported protocol for proxy is either http or https")
			}
			proxyConfigMap["protocol"] = protocol

			user := proxyConfigURL.User.Username()
			if user == "" {
				return nil, errors.New("error: missing username for the proxy configuration")
			}
			proxyConfigMap["user"] = user

			pass, IsPassProvided := proxyConfigURL.User.Password()
			if IsPassProvided == false {
				return nil, errors.New("error: missing password for the proxy configuration")
			}
			proxyConfigMap["pass"] = pass
			
			addr, port, err := net.SplitHostPort(proxyConfigURL.Host)
			if err != nil {
				return nil, err
			}
			proxyConfigMap["addr"] = addr
			proxyConfigMap["port"] = port
		}
	}

	return proxyConfigMap, nil
}

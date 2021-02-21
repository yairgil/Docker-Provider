package influxexporter

import (
	"net"
	"time"
	"fmt"
	"errors"
)

var (
	MEInfluxUnixSocketClient net.Conn
)

//ME client to write influx data
func CreateMEClient() {
	if MEInfluxUnixSocketClient != nil {
		MEInfluxUnixSocketClient.Close()
		MEInfluxUnixSocketClient = nil
	}
	conn, err := net.DialTimeout("tcp",
		"0.0.0.0:8089", 10*time.Second)
	if err != nil {
		fmt.Println("Error::ME::Unable to open ME influx TCP socket connection %s", err.Error())
	} else {
		fmt.Println("Successfully created ME influx TCP socket connection")
		MEInfluxUnixSocketClient = conn
	}
}

func Write2ME(messages []byte) (numBytesWritten int, e error) {
	if MEInfluxUnixSocketClient == nil {
		fmt.Println("ME connection does not exist. Creating...")
		CreateMEClient()
	}
	if MEInfluxUnixSocketClient != nil {
		MEInfluxUnixSocketClient.SetWriteDeadline(time.Now().Add(10 * time.Second))
		if messages != nil && len(messages) > 0 {
			bytesWritten, e := MEInfluxUnixSocketClient.Write(messages)
			if e != nil { MEInfluxUnixSocketClient = nil}
			return bytesWritten, e
		}
	} 
	return 0, errors.New("Error opening TCP connection to ME")
}
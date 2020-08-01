package main

import (
  "log"
  "net/http"
  "time"
  "github.com/microsoft/ApplicationInsights-Go/appinsights"
)

var (
  telemetryClient    appinsights.TelemetryClient
  instrumentationKey string
)

func init() {
	instrumentationKey = "c7bd3894-56c1-47d7-a8e6-98440e2018b6"
	telemetryClient = appinsights.NewTelemetryClient(instrumentationKey)

	// name of the service submitting the telemetry
	telemetryClient.Context().Tags.Cloud().SetRole("hello-world")

	// turn on diagnostics to help troubleshoot problems with telemetry submission
	appinsights.NewDiagnosticsMessageListener(func(msg string) error {
		log.Printf("[%s] %s\n", time.Now().Format(time.UnixDate), msg)
		return nil
  	})
}

func handleRequestWithLog(h func(http.ResponseWriter, *http.Request)) http.HandlerFunc {
  return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
     startTime := time.Now().UTC()
     h(w, r)
     duration := time.Now().Sub(startTime)
     request := appinsights.NewRequestTelemetry(r.Method, r.URL.Path, duration, "200")
     request.Timestamp = time.Now().UTC()
     telemetryClient.Track(request)
  })
}

func helloWorld(w http.ResponseWriter, r *http.Request) {
    w.WriteHeader(http.StatusOK)
    w.Write([]byte(`Hello World`))
}

func main() {
  
	// hit http://localhost:8080/hello to generate data
   http.HandleFunc("/hello", handleRequestWithLog(helloWorld))
   log.Fatal(http.ListenAndServe(":8080", nil))
}
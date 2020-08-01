// Copyright 2019 OpenTelemetry Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Program otelcontribcol is the Omnition Telemetry Service built on top of
// OpenTelemetry Service.
package main

import (
	"log"

	"go.opentelemetry.io/collector/service"

	"github.com/open-telemetry/opentelemetry-collector-contrib/internal/version"
)

// func InitializeTelemetryClient(agentVersion string) (int, error) {
// 	encodedIkey := os.Getenv(envAppInsightsAuth)
// 	if encodedIkey == "" {
// 		return -1, errors.New("Missing Environment Variable")
// 	}

// 	decIkey, err := base64.StdEncoding.DecodeString(encodedIkey)
// 	if err != nil {
// 		log.Printf("Decoding Error %s", err.Error())
// 		return -1, err
// 	}

// 	return 0, nil
// }

func main() {
	// // print this: "\"os.Getenv(envAppInsightsAuth)\""
	// log.Printf("7.29.2020 @ 8:23pm --------- app insights auth is: %s\n", os.Getenv(envAppInsightsAuth))

	// ret, err := InitializeTelemetryClient("1.10.0.1")
	// if ret != 0 || err != nil {
	// 	message := fmt.Sprintf("Error During Telemetry Initialization :%s", err.Error())
	// 	log.Printf(message)
	// }
	// // later change this to use AI AUTH env variable
	// TelemetryClient := appinsights.NewTelemetryClient("c7bd3894-56c1-47d7-a8e6-98440e2018b6")
	// TelemetryClient.TrackMetric("Testing testing", 0)

	handleErr := func(message string, err error) {
		if err != nil {
			log.Fatalf("%s: %v", message, err)
			// exception := appinsights.NewExceptionTelemetry(err)
			// TelemetryClient.Track(exception)
		}
	}

	factories, err := components()
	handleErr("Failed to build components", err)

	info := service.ApplicationStartInfo{
		ExeName:  "otelcontribcol",
		LongName: "OpenTelemetry Contrib Collector",
		Version:  version.Version,
		GitHash:  version.GitHash,
	}

	svc, err := service.New(service.Parameters{Factories: factories, ApplicationStartInfo: info})
	handleErr("Failed to construct the application", err)

	err = svc.Start()
	handleErr("Application run finished with error", err)
}

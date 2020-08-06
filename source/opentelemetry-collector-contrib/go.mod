module github.com/open-telemetry/opentelemetry-collector-contrib

go 1.14

require (
	github.com/client9/misspell v0.3.4
	github.com/golangci/golangci-lint v1.28.1
	github.com/google/addlicense v0.0.0-20200622132530-df58acafd6d5
	github.com/jstemmer/go-junit-report v0.9.1
	github.com/Microsoft/ApplicationInsights-Go v0.4.2
	github.com/open-telemetry/opentelemetry-collector-contrib/exporter/azuremonitorexporter v0.6.0
	github.com/opentracing/opentracing-go v1.1.1-0.20190913142402-a7454ce5950e // indirect
	github.com/pavius/impi v0.0.3
	github.com/tcnksm/ghr v0.13.0
	go.opentelemetry.io/collector v0.6.0
	honnef.co/go/tools v0.0.1-2020.1.4
)

// Replace references to modules that are in this repository with their relateive paths
// so that we always build with current (latest) version of the source code.

replace github.com/open-telemetry/opentelemetry-collector-contrib/internal/common => ./internal/common

replace k8s.io/client-go => k8s.io/client-go v0.0.0-20190620085101-78d2af792bab

module github.com/open-telemetry/opentelemetry-collector-contrib

go 1.14

require (
	github.com/client9/misspell v0.3.4
	github.com/golangci/golangci-lint v1.24.0
	github.com/google/addlicense v0.0.0-20200301095109-7c013a14f2e2
	github.com/jstemmer/go-junit-report v0.9.1

	github.com/open-telemetry/opentelemetry-collector-contrib/exporter/azuremonitorexporter v0.0.0
	
	github.com/pavius/impi v0.0.0-20180302134524-c1cbdcb8df2b
	github.com/tcnksm/ghr v0.13.0
	go.opentelemetry.io/collector v0.4.0
	go.opentelemetry.io/otel v0.6.0
	go.opentelemetry.io/otel/exporters/trace/jaeger v0.6.0
	honnef.co/go/tools v0.0.1-2020.1.3
)

replace git.apache.org/thrift.git v0.12.0 => github.com/apache/thrift v0.12.0

replace github.com/apache/thrift => github.com/apache/thrift v0.0.0-20161221203622-b2a4d4ae21c7

// Replace references to modules that are in this repository with their relateive paths
// so that we always build with current (latest) version of the source code.

replace github.com/open-telemetry/opentelemetry-collector-contrib/internal/common => ./internal/common

replace github.com/open-telemetry/opentelemetry-collector-contrib/exporter/azuremonitorexporter => ./exporter/azuremonitorexporter

replace k8s.io/client-go => k8s.io/client-go v0.0.0-20190620085101-78d2af792bab

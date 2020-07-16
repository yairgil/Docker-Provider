module github.com/open-telemetry/opentelemetry-collector-contrib

go 1.14

require (
	contrib.go.opencensus.io/resource v0.1.2 // indirect
	github.com/bmizerany/perks v0.0.0-20141205001514-d9a9656a3a4b // indirect
	github.com/bombsimon/wsl/v2 v2.0.0 // indirect
	github.com/client9/misspell v0.3.4
	github.com/golangci/golangci-lint v1.28.1
	github.com/google/addlicense v0.0.0-20200622132530-df58acafd6d5
	github.com/jstemmer/go-junit-report v0.9.1
	github.com/klauspost/cpuid v1.2.0 // indirect
	github.com/open-telemetry/opentelemetry-collector-contrib/exporter/azuremonitorexporter v0.6.0

	github.com/pavius/impi v0.0.3
	github.com/prashantv/protectmem v0.0.0-20171002184600-e20412882b3a // indirect
	github.com/streadway/quantile v0.0.0-20150917103942-b0c588724d25 // indirect
	github.com/tcnksm/ghr v0.13.0
	github.com/uber-go/atomic v1.4.0 // indirect
	github.com/uber/tchannel-go v1.16.0 // indirect
	go.opentelemetry.io/collector v0.6.0
	go.opentelemetry.io/otel v0.6.0
	honnef.co/go/tools v0.0.1-2020.1.4
	k8s.io/client-go v0.0.0-20190620085101-78d2af792bab
)

// Replace references to modules that are in this repository with their relateive paths
// so that we always build with current (latest) version of the source code.

replace github.com/open-telemetry/opentelemetry-collector-contrib/internal/common => ./internal/common

replace k8s.io/client-go => k8s.io/client-go v0.0.0-20190620085101-78d2af792bab

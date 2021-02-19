module github.com/vishiy/opentelemetry-collector-builder

go 1.14

require (
	github.com/influxdata/influxdb-client-go/v2 v2.2.2 // indirect
	github.com/vishiy/influxexporter v0.0.0-00010101000000-000000000000
	go.opentelemetry.io/collector v0.20.0
)

replace github.com/vishiy/influxexporter => ../influxexporter

BASE_DIR := $(subst /build/linux,,$(PWD))

.PHONY: otelcollector
otelcollector:
	rm -rf otelcollector
	go get
	go build -o otelcollector .
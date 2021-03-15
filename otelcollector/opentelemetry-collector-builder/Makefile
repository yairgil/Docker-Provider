BASE_DIR := $(subst /build/linux,,$(PWD))

otelcollectortarget:
	rm -rf otelcollector
	go get
	go build -o otelcollector .
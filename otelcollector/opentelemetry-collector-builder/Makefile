BASE_DIR := $(subst /build/linux,,$(PWD))
include $(BASE_DIR)/build/version

otelcollectortarget:
	rm -rf otelcollector
	go get
	go build -o otelcollector .
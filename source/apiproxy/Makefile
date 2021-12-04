BASE_DIR := $(subst /build/linux,,$(PWD))
include $(BASE_DIR)/build/version

apiproxy:
	@echo "========================= Building  apiproxy go code  ========================="
	export BUILDVERSION=$(CONTAINER_BUILDVERSION_MAJOR).$(CONTAINER_BUILDVERSION_MINOR).$(CONTAINER_BUILDVERSION_PATCH)-$(CONTAINER_BUILDVERSION_BUILDNR)
	export BUILDDATE=$(CONTAINER_BUILDVERSION_DATE)
	@echo $(BUILDVERSION)
	@echo $(BUILD_DATE)
	@echo "========================= cleanup existing .so and .h file  ========================="
	rm -rf *.so *.h apiproxy *~
	@echo "========================= go get  ========================="
	go get
	@echo "========================= go build  ========================="
	go build -ldflags "-X 'main.revision=$(BUILDVERSION)' -X 'main.builddate=$(BUILDDATE)'" .
	@echo "========================= make binary as executable  ========================="
	chmod +x apiproxy

test:
	go test -cover -race -coverprofile=coverage.txt -covermode=atomic

clean:
	rm -rf *.so *.h apiproxy *~

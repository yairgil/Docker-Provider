GITVERSION := 0.1
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	BUILDDATE := $(shell date --rfc-3339=seconds)
endif
ifeq ($(UNAME_S),Darwin)
	BUILDDATE := $(shell gdate --rfc-3339=seconds)
endif

fbplugin:
	sudo go get
	sudo go build -ldflags "-X 'main.revision=$(GITVERSION)' -X 'main.builddate=$(BUILDDATE)'" -buildmode=c-shared -o out_oms.so .

test:
	sudo go test -cover -race -coverprofile=coverage.txt -covermode=atomic

clean:
	sudo rm -rf *.so *.h *~

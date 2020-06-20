#!/bin/bash

set +e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo $DIR

echo "set GOPATH, GOARCH and GOOS env variables"
export GOOS="linux"
export GOARCH="amd64"
export GOPATH=$DIR/../source/plugins/go
echo "GOPATH:"$GOPATH
echo "GOPROXY":$GOPROXY
go env
echo "start:get the go modules"
cd $DIR/../source/plugins/go/src
go get
echo "end:get the go modules"

cd $DIR/../build/linux
echo "----------- Build Docker Provider -------------------------------"
make
cd $DIR

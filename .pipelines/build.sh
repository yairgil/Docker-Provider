#!/bin/bash

set +e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo $DIR

echo "start:get the go modules"
cd $DIR/../source/plugins/go/src
go get
echo "end:get the go modules"

cd $DIR/../build/linux
echo "----------- Build Docker Provider -------------------------------"
make
cd $DIR

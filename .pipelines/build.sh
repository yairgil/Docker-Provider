#!/bin/bash

set +e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo $DIR

cd $DIR/../build/linux

echo "get the go modules"
go get
echo "----------- Build Docker Provider -------------------------------"
make
cd $DIR

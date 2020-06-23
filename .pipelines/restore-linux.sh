#!/bin/bash
pwd
PWD=`pwd`
pushd $PWD

# Find location of this script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo $DIR

# change to source code directory
cd $DIR/../source/plugins/go
pwd

echo "set GOPATH, GOARCH and GOOS env variables"
export GOOS="linux"
export GOARCH="amd64"
export GOPATH=$DIR/../source/plugins/go
echo "GOPATH:"$GOPATH
go env

echo "get go modules"
go get

# Restore working directory
popd

# Exit with explicit 0 exit code so build will not fail
exit 0

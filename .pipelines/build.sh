#!/bin/bash

set +e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo $DIR

# grant file permission for go bin
sudo chmod 777 /usr/local/go/bin

cd $DIR/../build/linux

echo ----------- Build Docker Provider -------------------------------
make
cd $DIR

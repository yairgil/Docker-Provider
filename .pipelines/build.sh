#!/bin/bash

set +e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo $DIR

cd $DIR/../build/linux

echo ----------- Build Docker Provider -------------------------------
make
cd $DIR

set -e

export IN_UNIT_TEST=true

OLD_PATH=$(pwd)
SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
export REPO_ROOT=OLD_PATH/../..

cd $SCRIPTPATH/../../source/plugins/go/src
echo "# Runnign go generate"
go generate

echo "# Running go test ."
go test .

unset IN_UNIT_TEST
unset REPO_ROOT

cd $OLD_PATH

#!/bin/sh
set -x
set -e

results_dir="${RESULTS_DIR:-/tmp/results}"

function waitForResources {
    available=false
    max_retries=60
    sleep_seconds=10
    RESOURCETYPE=$1
    NAMESPACE=$2
	RESOURCE=$3
    for i in $(seq 1 $max_retries)
    do
    if [[ ! $(kubectl wait --for=condition=available ${RESOURCETYPE} ${RESOURCE} --all --namespace ${NAMESPACE}) ]]; then
        sleep ${sleep_seconds}
    else
        available=true
        break
    fi
    done

    echo "$available"
}

function validateCommonParameters {
    if [[ -z "${TENANT_ID}" ]]; then
		echo "ERROR: parameter TENANT_ID is required." > ${results_dir}/error
		python3 setup_failure_handler.py
	fi
	if [[ -z "${CLIENT_ID}" ]]; then
	   echo "ERROR: parameter CLIENT_ID is required." > ${results_dir}/error
	   python3 setup_failure_handler.py
	fi

	if [[ -z "${CLIENT_SECRET}" ]]; then
	   echo "ERROR: parameter CLIENT_SECRET is required." > ${results_dir}/error
	   python3 setup_failure_handler.py
	fi
}

function validateArcConfTestParameters {
	if [[ -z "${SUBSCRIPTION_ID}" ]]; then
		echo "ERROR: parameter SUBSCRIPTION_ID is required." > ${results_dir}/error
		python3 setup_failure_handler.py
	fi

	if [[ -z "${RESOURCE_GROUP}" ]]; then
		echo "ERROR: parameter RESOURCE_GROUP is required." > ${results_dir}/error
		python3 setup_failure_handler.py
	fi

	if [[ -z "${CLUSTER_NAME}" ]]; then
		echo "ERROR: parameter CLUSTER_NAME is required." > ${results_dir}/error
		python3 setup_failure_handler.py
	fi

	if [[ -z "${CI_ARC_RELEASE_TRAIN}" ]]; then
		echo "ERROR: parameter CI_ARC_RELEASE_TRAIN is required." > ${results_dir}/error
		python3 setup_failure_handler.py
	fi

	if [[ -z "${CI_ARC_VERSION}" ]]; then
		echo "ERROR: parameter CI_ARC_VERSION is required." > ${results_dir}/error
		python3 setup_failure_handler.py
	fi

	if [[ -z "${CI_TEST_BRANCH}" ]]; then
		echo "ERROR: parameter CI_TEST_BRANCH is required." > ${results_dir}/error
		python3 setup_failure_handler.py
	fi
}

function addArcK8sCLIExtension {
   az extension add --name k8s-extension 2> ${results_dir}/error || python3 setup_failure_handler.py
}

function createArcCIExtension {
	az k8s-extension create \
    --cluster-name $CLUSTER_NAME \
    --resource-group $RESOURCE_GROUP \
    --cluster-type connectedClusters \
    --extension-type Microsoft.AzureMonitor.Containers \
    --subscription $SUBSCRIPTION_ID \
    --scope cluster \
    --release-train $CI_ARC_RELEASE_TRAIN \
    --name azuremonitor-containers \
    --version $CI_ARC_VERSION 2> ${results_dir}/error || python3 setup_failure_handler.py
}

function deleteArcCIExtension {
    az k8s-extension delete --name azuremonitor-containers \
    --cluster-type connectedClusters \
	--cluster-name $CLUSTER_NAME \
	--resource-group $RESOURCE_GROUP || python3 setup_failure_handler.py
}

function login_to_azure {
	# Login with service principal
	az login --service-principal \
	-u ${CLIENT_ID} \
	-p ${CLIENT_SECRET} \
	--tenant ${TENANT_ID} 2> ${results_dir}/error || python3 setup_failure_handler.py
}


# saveResults prepares the results for handoff to the Sonobuoy worker.
# See: https://github.com/vmware-tanzu/sonobuoy/blob/master/docs/plugins.md
saveResults() {
    cd ${results_dir}

    # Sonobuoy worker expects a tar file.
	tar czf results.tar.gz *

	# Signal to the worker that we are done and where to find the results.
	printf ${results_dir}/results.tar.gz > ${results_dir}/done
}

# Ensure that we tell the Sonobuoy worker we are done regardless of results.
trap saveResults EXIT

# validate common params
validateCommonParameters

if [ "${IS_NON_ARC_K8S_TEST_ENVIRONMENT}" == "true" ]; then
   echo "skipping installing of ARC K8s container insights extension since the test environment is non-arc K8s"
else
   # validate params
   validateArcConfTestParameters

   # login to azure
   login_to_azure

   # Wait for resources in ARC ns
   waitSuccessArc="$(waitForResources deployment azure-arc)"
   if [ "${waitSuccessArc}" == false ]; then
	  echo "deployment is not avilable in namespace - azure-arc"
	  exit 1
   fi

   # add CLI extension
   addArcK8sCLIExtension

   # add ARC K8s container insights extension
   createArcCIExtension
fi

# Wait for deployment resources in kube-system ns
waitSuccessArc="$(waitForResources deployment omsagent-rs kube-system)"
if [ "${waitSuccessArc}" == false ]; then
    echo "omsagent-rs deployment is not avilable in namespace - kube-system"
    exit 1
fi

# Wait for ds resources in kube-system ns
# waitSuccessArc="$(waitForResources ds omsagent kube-system)"
# if [ "${waitSuccessArc}" == false ]; then
#     echo "omsagent is not avilable in namespace - kube-system"
#     exit 1
# fi

# The variable 'TEST_LIST' should be provided if we want to run specific tests. If not provided, all tests are run

NUM_PROCESS=$(pytest /e2etests/ --collect-only  -k "$TEST_NAME_LIST" -m "$TEST_MARKER_LIST" | grep "<Function\|<Class" -c)

export NUM_TESTS="$NUM_PROCESS"

pytest /e2etests/ --junitxml=/tmp/results/results.xml -d --tx "$NUM_PROCESS"*popen -k "$TEST_NAME_LIST" -m "$TEST_MARKER_LIST"

# cleanup extension resource
deleteArcCIExtension
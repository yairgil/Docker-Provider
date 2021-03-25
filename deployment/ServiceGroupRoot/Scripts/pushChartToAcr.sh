#!/bin/bash

export HELM_EXPERIMENTAL_OCI=1

echo "START - Release stage : ${RELEASE_STAGE}"

# login to acr
echo "Using acr : ${ACR_NAME}"
echo "login to acr:${ACR_NAME} using helm"
helm registry login $ACR_NAME  --username $ACR_APP_ID --password $ACR_APP_SECRET


echo "login to acr:${ACR_NAME} completed: ${ACR_NAME}"

case $RELEASE_STAGE in

  Canary)
    echo -n "Release stage - Canary"
    echo "start: push the chart version: ${CHART_VERSION} to acr repo: ${ACR_NAME}"
    echo "save the chart locally with acr full path"
    helm chart save charts/azuremonitor-containers/ ${ACR_NAME}/${REPO_PATH}:${CHART_VERSION}
    echo "pushing the helm chart to ACR: ${ACR}"
    helm chart push ${ACR_NAME}/${REPO_PATH}:${CHART_VERSION}
    echo "end: push the chart version: ${CHART_VERSION} to acr repo: ${ACR_NAME}"
    ;;

  Pilot)
    echo -n "Release stage - Pilot"
    echo "Pull canary region chart from MCR"
    ;;

  Prod1)
    echo -n "Release stage - Prod1"
    echo "Pull Prod1 region chart from MCR"
    ;;
   
  Prod2)
    echo -n "Release stage - Prod2"
    echo "Pull Prod2 region chart from MCR"
    ;;

  Prod3)
    echo -n "Release stage - Prod3"
    echo "Pull Prod3 region chart from MCR"
    ;;  

  FF)
    echo -n "Release stage - FF"
    echo "Pull FF region chart from MCR"
    ;;    

  MC)
    echo -n "Release stage - MC"
    echo "Pull MC region chart from MCR"
    ;;    

  *)
    echo -n "unknown release stage"
    ;;
esac

echo "END - Release stage : ${RELEASE_STAGE}"

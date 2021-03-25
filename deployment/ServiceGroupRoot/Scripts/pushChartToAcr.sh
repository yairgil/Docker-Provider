#!/bin/bash

export HELM_EXPERIMENTAL_OCI=1

echo "Using acr : ${ACR_NAME}"

echo "login to acr:${ACR_NAME} using helm"
helm registry login $ACR_NAME  --username $ACR_APP_ID --password $ACR_APP_SECRET

echo "login to acr:${ACR_NAME} completed: ${ACR_NAME}"

echo "start: push the chart version: ${CHART_VERSION} to acr repo: ${ACR_NAME}"

echo "save the chart locally with acr full path"
helm chart save charts/azuremonitor-containers/ ${ACR_NAME}/${REPO_PATH}:${CHART_VERSION}

echo "pushing the helm chart to ACR: ${ACR}"
helm chart push ${ACR_NAME}/${REPO_PATH}:${CHART_VERSION}

echo "end: push the chart version: ${CHART_VERSION} to acr repo: ${ACR_NAME}"

#!/bin/bash

echo "Using CDPX acr Server: ${CDPX_ACR}"
echo "Start: Import linux agent image from cdpx and push to acr: ${ACR_NAME}"
echo "Target acr: ${ACR_NAME}"
echo "Target agent repo name: ${IMAGE_REPO}"

echo "Creating target full image path"

TARGET_FULL_IMAGE_PATH=${IMAGE_PATH}/${IMAGE_REPO}:${IMAGE_TAG}

CDPX_FULL_IMAGE_PATH=${CDPX_ACR}/official/${CDPX_REPO}:${CDPX_IMAGE_TAG}

echo "Target image full path: ${TARGET_FULL_IMAGE_PATH}"
echo "Source image full path: ${CDPX_FULL_IMAGE_PATH}"

echo "Logging into az cli"
az login --identity

echo "Pushing the image from source: ${CDPX_FULL_IMAGE_PATH} to target acr: ${ACR_NAME}"

az acr import -n ${ACR_NAME} --source ${CDPX_FULL_IMAGE_PATH} -t ${TARGET_FULL_IMAGE_PATH} -u ${ACR_APP_ID} -p ${ACR_APP_SECRET} 

echo "Pushing the image to acr completed"
echo "Finished: Import linux agent image from cdpx and push to acr: ${ACR_NAME}"


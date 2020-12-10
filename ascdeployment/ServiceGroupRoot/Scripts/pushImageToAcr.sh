#!/bin/bash

CDPX_ACR="${CDPX_REPO_NAME}.azurecr.io"

echo "Using CDPX acr Server: ${CDPX_ACR}"
echo "Start: Import linux agent image from cdpx and push to acr: ${ACR_NAME}"
echo "Target release name: "$RELEASE_NAME
echo "Target acr: ${ACR_NAME}"
echo "Target agent repo name: ${IMAGE_REPO}"

imagetag=$RELEASE_NAME$IMAGE_TAG_SUFFIX

echo "Target Agent image tag: "$imagetag
echo "Creating target agent full image path"

fullImagePath=${IMAGE_PATH}/${IMAGE_REPO}:${imagetag}

echo "Target image full path: ${fullImagePath}"
echo "Source image full path: ${CDPX_ACR_RESOURCE_ID}/${CDPX_REPO_NAME}: ${CDPX_IMAGE_TAG}"
echo "Pushing the image from  ${CDPX_ACR_RESOURCE_ID}/${CDPX_REPO_NAME} to acr: ${ACR_NAME}"

CDPX_FULL_IMAGE_PATH=${CDPX_ACR}/artifact/3170cdd2-19f0-4027-912b-1027311691a2/official/${CDPX_REPO_NAME}:${CDPX_IMAGE_TAG}

az login --identity

echo "Attempt 1:"
az acr import -r ${CDPX_ACR_RESOURCE_ID} --source ${CDPX_REPO_NAME}:${CDPX_IMAGE_TAG} -n ${ACR_NAME} -t ${fullImagePath} 

echo "Attempt 2:"
az acr import --source ${CDPX_FULL_IMAGE_PATH} -n ${ACR_NAME} -t ${fullImagePath}

echo "Attempt 3:"
az acr import -n ${ACR_NAME} --source ${CDPX_FULL_IMAGE_PATH} -t ${fullImagePath} -u ${ACR_APP_ID} -p ${ACR_APP_SECRET} 

echo "Pushing the image to acr completed"

echo "Finished: Import linux agent image from cdpx and push to acr: ${ACR_NAME}"


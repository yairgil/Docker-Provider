#!/bin/bash

echo "Start: Import linux agent image from cdpx and push to acr: ${ACR_NAME}"
echo "Target release name: "$RELEASE_NAME
echo "Target acr: ${ACR_NAME}"
echo "Target agent repo name: ${IMAGE_REPO}"

imagetag=$RELEASE_NAME$IMAGE_TAG_SUFFIX

echo "Target Agent image tag: "$imagetag
echo "Creating target agent full image path"

fullImagePath=${IMAGE_PATH}/${IMAGE_REPO}:${imagetag}

echo "Target image full path: ${fullImagePath}"
echo "Pushing the image to acr:${ACR_NAME}"

az acr import -r ${CDPX_ACR_RESOURCE_ID} --source ${CDPX_REPO_NAME}:${CDPX_IMAGE_TAG} -n ${ACR_NAME} -t ${fullImagePath} 

echo "Pushing the image to acr completed"

echo "Finished: Import linux agent image from cdpx and push to acr: ${CIACR}"


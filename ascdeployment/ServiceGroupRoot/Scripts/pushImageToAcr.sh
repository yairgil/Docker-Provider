#!/bin/bash

echo "start: pull linux agent image from cdpx and push to acr: ${CIACR}"

echo "login to cdpxlinux acr:${CDPX_ACR}"
docker login $CDPX_ACR  --username $ACR_APP_ID --password $ACR_APP_SECRET
echo "login to cdpxlinux acr completed: ${CDPX_ACR}"

echo "pull agent image from cdpxlinux acr: ${CDPX_ACR}"
docker pull ${CDPX_ACR}/artifact/3170cdd2-19f0-4027-912b-1027311691a2/official/${CDPX_REPO_NAME}:${CDPX_AGENT_IMAGE_TAG}
echo "pull image from cdpxlinux acr completed: ${CDPX_ACR}"

echo "CI Release name is:"$CI_RELEASE
imagetag=$CI_RELEASE$CI_IMAGE_TAG_SUFFIX
echo "agentimagetag="$imagetag

echo "CI ACR : ${CI_ACR}"
echo "CI AGENT REPOSITORY NAME : ${CI_AGENT_REPO}"

echo "tag linux agent image"
fullImagePath=${CI_ACR}/${IMAGE_PATH}/${CI_AGENT_REPO}:${imagetag}
docker tag ${CDPX_ACR}/artifact/3170cdd2-19f0-4027-912b-1027311691a2/official/${CDPX_REPO_NAME}:${CDPX_AGENT_IMAGE_TAG} ${fullImagePath}

echo "login ciprod acr":$CI_ACR
docker login $CI_ACR --username $ACR_APP_ID --password $ACR_APP_SECRET
echo "login to ${CI_ACR} acr completed"

echo "pushing the image to ciprod acr:${CI_ACR}"
docker push ${fullImagePath}
echo "pushing the image to ciprod acr completed"

echo "end: pull linux agent image from cdpx and push to ciprod acr"


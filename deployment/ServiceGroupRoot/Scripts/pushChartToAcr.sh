#!/bin/bash

export HELM_EXPERIMENTAL_OCI=1
export MCR_NAME="mcr.microsoft.com"
export REPO_TYPE="preview"

# repo paths for SDP roll-out
# canary region
export CANARY_REGION_REPO_PATH="azuremonitor/containerinsights/canary/${REPO_TYPE}/azuremonitor-containers"
# pilot region
export PILOT_REGION_REPO_PATH="azuremonitor/containerinsights/prod1/${REPO_TYPE}/azuremonitor-containers"
# medium load regions (1)
export PROD2_REGION_REPO_PATH="azuremonitor/containerinsights/prod2/${REPO_TYPE}/azuremonitor-containers"
# medium load regions (2)
export PROD3_REGION_REPO_PATH="azuremonitor/containerinsights/prod3/${REPO_TYPE}/azuremonitor-containers"
# high load regions (1)
export PROD4_REGION_REPO_PATH="azuremonitor/containerinsights/prod4/${REPO_TYPE}/azuremonitor-containers"
# FF
export PROD5_REGION_REPO_PATH="azuremonitor/containerinsights/prod5/${REPO_TYPE}/azuremonitor-containers"
# MC 
export PROD6_REGION_REPO_PATH="azuremonitor/containerinsights/prod5/${REPO_TYPE}/azuremonitor-containers"

echo "START - Release stage : ${RELEASE_STAGE}"

# login to acr
echo "Using acr : ${ACR_NAME}"
echo "login to acr:${ACR_NAME} using helm"
helm registry login $ACR_NAME  --username $ACR_APP_ID --password $ACR_APP_SECRET


echo "login to acr:${ACR_NAME} completed: ${ACR_NAME}"

case $RELEASE_STAGE in

  Canary)
    echo -n "start: Release stage - Canary"
    acrFullPath=${ACR_NAME}/public/${CANARY_REGION_REPO_PATH}:${CHART_VERSION}    
    echo "start: push the chart : ${acrFullPath}"
    echo "save the chart locally with acr full path: ${acrFullPath}"
    helm chart save charts/azuremonitor-containers/ $acrFullPath
    echo "start: pushing the helm chart to ACR: ${acrFullPath}"
    helm chart push $acrFullPath
    echo "end: push the chart : ${acrFullPath}"    
    echo -n "end: Release stage - Canary"
    ;;

  Pilot | Prod1)
    # prod 1
    echo -n "start: Release stage - Pilot"    
    mcrFullPath=${MCR_NAME}/${CANARY_REGION_REPO_PATH}:${CHART_VERSION}            
    echo "Pulling canary region chart from MCR:${mcrFullPath} to push Pilot regions"
    helm chart pull ${mcrFullPath}
    echo "Exporting chart"    
    helm chart export ${mcrFullPath}    
    acrFullPath=${ACR_NAME}/public/${PILOT_REGION_REPO_PATH}:${CHART_VERSION} 
    echo "save the chart locally with acr full path: ${acrFullPath}"    
    helm chart save azuremonitor-containers/ ${acrFullPath} 
    echo "start: push the chart version: ${acrFullPath}"
    helm chart push ${acrFullPath} 
    echo "end: push the chart version: ${acrFullPath}"
    echo -n "end: Release stage - Pilot"    
    ;;

  Prod2 | MediumLow)
    # prod 2
    echo -n "start: Release stage - Medium Low Laod Regions - Prod2"    
    mcrFullPath=${MCR_NAME}/${PILOT_REGION_REPO_PATH}:${CHART_VERSION}
    echo "Pull Prod1 region chart from MCR:${mcrFullPath} to push to Prod2 regions"
    helm chart pull ${mcrFullPath}
    echo "Exporting chart"    
    helm chart export ${mcrFullPath}
    acrFullPath=${ACR_NAME}/public/${PROD2_REGION_REPO_PATH}:${CHART_VERSION}
    echo "save the chart locally with acr full path: ${acrFullPath}"    
    helm chart save azuremonitor-containers/ ${acrFullPath}    
    echo "start: push the chart version: ${acrFullPath}"
    helm chart push ${acrFullPath} 
    echo "end: push the chart version: ${acrFullPath}"
    echo -n "end: Release stage - Medium Low Laod Regions - Prod2"    
    ;;
   
  Prod3 | MediumHigh)
    # prod 3
    echo -n "start: Release stage - Medium High Laod Regions - Prod3"
    echo "Pull Prod2 region chart from MCR to push to Prod3 regions"
    mcrFullPath=${MCR_NAME}/${PROD2_REGION_REPO_PATH}:${CHART_VERSION}
    helm chart pull ${mcrFullPath}
    echo "Exporting chart"    
    helm chart export ${mcrFullPath}
    echo "save the chart locally with acr full path"
    acrFullPath=${ACR_NAME}/public/${PROD3_REGION_REPO_PATH}:${CHART_VERSION}
    helm chart save azuremonitor-containers/ ${acrFullPath}    
    echo "start: push the chart version: ${acrFullPath}"
    helm chart push ${acrFullPath} 
    echo "end: push the chart version: ${acrFullPath}"        
    echo -n "end: Release stage - Medium High Laod Regions - Prod3"
    ;;

  Prod4 | High)
    # prod 4    
    echo -n "start: Release stage - High Laod Regions - Prod4"
    echo "Pull Prod3 region chart from MCR to push to Prod4 regions"
    mcrFullPath=${MCR_NAME}/${PROD3_REGION_REPO_PATH}:${CHART_VERSION}    
    helm chart pull ${mcrFullPath}
    echo "Exporting chart"    
    helm chart export ${mcrFullPath}
    acrFullPath=${ACR_NAME}/public/${PROD4_REGION_REPO_PATH}:${CHART_VERSION}
    echo "save the chart locally with acr full path: ${acrFullPath}"    
    helm chart save azuremonitor-containers/ ${acrFullPath}    
    echo "start: push the chart version: ${acrFullPath}"
    helm chart push ${acrFullPath} 
    echo "end: push the chart version: ${acrFullPath}"  
    echo -n "end: Release stage - High Laod Regions - Prod4"  
    ;;  

  FF | Prod5)
    # prod 5
    echo -n "start: Release stage - FF"    
    mcrFullPath=${MCR_NAME}/${PROD4_REGION_REPO_PATH}:${CHART_VERSION}        
    echo "Pull Prod4 region chart from MCR:${mcrFullPath} to push to Prod5 regions"    
    helm chart pull ${mcrFullPath}
    echo "Exporting chart"    
    helm chart export ${mcrFullPath}
    echo "save the chart locally with acr full path"
    acrFullPath=${ACR_NAME}/public/${PROD5_REGION_REPO_PATH}:${CHART_VERSION}
    helm chart save azuremonitor-containers/ ${acrFullPath}    
    echo "start: push the chart version: ${acrFullPath}"
    helm chart push ${acrFullPath} 
    echo "end: push the chart version: ${acrFullPath}"   
    echo -n "end: Release stage - FF"     
    ;;    

  MC | Prod6)
    echo -n "Release stage - MC"
    echo "Pull MC region chart from MCR"
    ;;    

  *)
    echo -n "unknown release stage"
    ;;
esac

echo "END - Release stage : ${RELEASE_STAGE}"

#!/bin/bash

export HELM_EXPERIMENTAL_OCI=1
export MCR_NAME="mcr.microsoft.com"
export REPO_TYPE="preview"

# repo paths for arc k8s extension roll-out
# canary region
export CANARY_REGION_REPO_PATH="azuremonitor/containerinsights/canary/${REPO_TYPE}/azuremonitor-containers"
# pilot region
export PILOT_REGION_REPO_PATH="azuremonitor/containerinsights/prod1/${REPO_TYPE}/azuremonitor-containers"
# light load regions 
export LIGHT_LOAD_REGION_REPO_PATH="azuremonitor/containerinsights/prod2/${REPO_TYPE}/azuremonitor-containers"
# medium load regions
export MEDIUM_LOAD_REGION_REPO_PATH="azuremonitor/containerinsights/prod3/${REPO_TYPE}/azuremonitor-containers"
# high load regions
export HIGH_LOAD_REGION_REPO_PATH="azuremonitor/containerinsights/prod4/${REPO_TYPE}/azuremonitor-containers"
# FairFax regions
export FF_REGION_REPO_PATH="azuremonitor/containerinsights/prod5/${REPO_TYPE}/azuremonitor-containers"
# Mooncake regions
export MC_REGION_REPO_PATH="azuremonitor/containerinsights/prod6/${REPO_TYPE}/azuremonitor-containers"

# pull chart from previous stage mcr and push chart to next stage acr
pull_chart_from_source_mcr_to_push_to_dest_acr() {
    srcMcrFullPath=${1}
    destAcrFullPath=${2}

    if [ -z $srcMcrFullPath ]; then
       echo "-e error source mcr path must be provided "
       exit 1
    fi

    if [ -z $destAcrFullPath ]; then
       echo "-e error dest acr path must be provided "
       exit 1
    fi
    
    echo "Pulling chart from MCR:${srcMcrFullPath} ..."
    helm chart pull ${srcMcrFullPath}
    echo "Pulling chart from MCR:${srcMcrFullPath} completed."

    echo "Exporting chart to current directory ..."    
    helm chart export ${srcMcrFullPath}
    echo "Exporting chart to current directory completed."    

    
    echo "save the chart locally with dest acr full path : ${destAcrFullPath} ..."    
    helm chart save azuremonitor-containers/ ${destAcrFullPath} 
    echo "save the chart locally with dest acr full path : ${destAcrFullPath} completed."

    echo "pushing the chart to acr path: ${destAcrFullPath} ..."
    helm chart push ${destAcrFullPath} 
    echo "pushing the chart to acr path: ${destAcrFullPath} completed."
}

# push to local release candidate chart to canary region
push_local_chart_to_canary_region() {
  destAcrFullPath=${1}
  if [ -z $destAcrFullPath ]; then
      echo "-e error dest acr path must be provided "
      exit 1
  fi

  echo "save the chart locally with dest acr full path : ${destAcrFullPath} ..."    
  helm chart save charts/azuremonitor-containers/ $destAcrFullPath
  echo "save the chart locally with dest acr full path : ${destAcrFullPath} completed."

  echo "pushing the chart to acr path: ${destAcrFullPath} ..."
  helm chart push $destAcrFullPath
  echo "pushing the chart to acr path: ${destAcrFullPath} completed."

}

echo "START - Release stage : ${RELEASE_STAGE}"

# login to acr
echo "Using acr : ${ACR_NAME}"
echo "Using acr repo type: ${REPO_TYPE}"

echo "login to acr:${ACR_NAME} using helm ..."
helm registry login $ACR_NAME  --username $ACR_APP_ID --password $ACR_APP_SECRET
echo "login to acr:${ACR_NAME} using helm completed."

case $RELEASE_STAGE in

  Canary)
    echo -n "start: Release stage - Canary"
    destAcrFullPath=${ACR_NAME}/public/${CANARY_REGION_REPO_PATH}:${CHART_VERSION}  
    push_local_chart_to_canary_region $destAcrFullPath     
    echo -n "end: Release stage - Canary"
    ;;

  Pilot | Prod1)
    # prod 1
    echo -n "start: Release stage - Pilot"    
    srcMcrFullPath=${MCR_NAME}/${CANARY_REGION_REPO_PATH}:${CHART_VERSION}   
    destAcrFullPath=${ACR_NAME}/public/${PILOT_REGION_REPO_PATH}:${CHART_VERSION}   
    pull_chart_from_source_mcr_to_push_to_dest_acr $srcMcrFullPath $destAcrFullPath          
    echo -n "end: Release stage - Pilot"    
    ;;

  LightLoad | Pord2)
    # prod 2
    echo -n "start: Release stage - Light Load Regions - Prod2"    
    srcMcrFullPath=${MCR_NAME}/${PILOT_REGION_REPO_PATH}:${CHART_VERSION}
    destAcrFullPath=${ACR_NAME}/public/${LIGHT_LOAD_REGION_REPO_PATH}:${CHART_VERSION}
    pull_chart_from_source_mcr_to_push_to_dest_acr $srcMcrFullPath $destAcrFullPath              
    echo -n "end: Release stage - Light Load Regions - Prod2"    
    ;;
   
  MediumLoad | Prod3)
    # prod 3
    echo -n "start: Release stage - Medium Load Regions - Prod3"
    echo "Pull Prod2 region chart from MCR to push to Prod3 regions"
    srcMcrFullPath=${MCR_NAME}/${LIGHT_LOAD_REGION_REPO_PATH}:${CHART_VERSION}
    destAcrFullPath=${ACR_NAME}/public/${MEDIUM_LOAD_REGION_REPO_PATH}:${CHART_VERSION}
    pull_chart_from_source_mcr_to_push_to_dest_acr $srcMcrFullPath $destAcrFullPath     
    echo -n "end: Release stage - Medium Load Regions - Prod3"
    ;;

  HighLoad | Prod4)
    # prod 4    
    echo -n "start: Release stage - High Load Regions - Prod4"
    echo "Pull Prod3 region chart from MCR to push to Prod4 regions"
    srcMcrFullPath=${MCR_NAME}/${MEDIUM_LOAD_REGION_REPO_PATH}:${CHART_VERSION} 
    destAcrFullPath=${ACR_NAME}/public/${HIGH_LOAD_REGION_REPO_PATH}:${CHART_VERSION}   
    pull_chart_from_source_mcr_to_push_to_dest_acr $srcMcrFullPath $destAcrFullPath         
    echo -n "end: Release stage - High Load Regions - Prod4"  
    ;;  

  FF | Prod5)
    # prod 5
    echo -n "start: Release stage - FF"    
    srcMcrFullPath=${MCR_NAME}/${HIGH_LOAD_REGION_REPO_PATH}:${CHART_VERSION}        
    destAcrFullPath=${ACR_NAME}/public/${FF_REGION_REPO_PATH}:${CHART_VERSION}
    pull_chart_from_source_mcr_to_push_to_dest_acr $srcMcrFullPath $destAcrFullPath                   
    echo -n "end: Release stage - FF"     
    ;;    

  MC | Prod6)
    echo -n "Release stage - MC"
    echo "Pull MC region chart from MCR"
    srcMcrFullPath=${MCR_NAME}/${FF_REGION_REPO_PATH}:${CHART_VERSION}            
    destAcrFullPath=${ACR_NAME}/public/${MC_REGION_REPO_PATH}:${CHART_VERSION}
    pull_chart_from_source_mcr_to_push_to_dest_acr $srcMcrFullPath $destAcrFullPath
    echo -n "end: Release stage - MC"     
    ;;    

  *)
    echo -n "unknown release stage"
    ;;
esac

echo "END - Release stage : ${RELEASE_STAGE}"

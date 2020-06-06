#!/bin/bash
#
# Execute this directly in Azure Cloud Shell (https://shell.azure.com) by pasting (SHIFT+INS on Windows, CTRL+V on Mac or Linux)
# the following line (beginning with curl...) at the command prompt and then replacing the args:
#  This scripts Onboards Azure Monitor for containers to Kubernetes cluster hosted outside and connected to Azure via Azure Arc cluster
#
#      1. Creates the Default Azure log analytics workspace if doesn't exist one in specified subscription
#      2. Adds the ContainerInsights solution to the Azure log analytics workspace
#      3. Adds the workspaceResourceId tag on the provided Azure Arc Cluster
#      4. Installs Azure Monitor for containers HELM chart to the K8s cluster in Kubeconfig
# Prerequisites :
#     Azure CLI:  https://docs.microsoft.com/en-us/cli/azure/install-azure-cli?view=azure-cli-latest
#     Helm3 : https://helm.sh/docs/intro/install/
#
# bash <script> <azureArcResourceId> <kube-context> <azureLogAnayticsWorkspaceResourceId>(optional)

# For example to onboard to azure monitor for containers using Default Azure Log Analytics Workspace:
# bash onboarding_azuremonitor_for_containers.sh /subscriptions/57ac26cf-a9f0-4908-b300-9a4e9a0fb205/resourceGroups/AzureArcTest/providers/Microsoft.Kubernetes/connectedClusters/AzureArcTest1 MyK8sTestCluster

# For example to onboard to azure monitor for containers using existing Azure Log Analytics Workspace:
# bash onboarding_azuremonitor_for_containers.sh /subscriptions/57ac26cf-a9f0-4908-b300-9a4e9a0fb205/resourceGroups/AzureArcTest/providers/Microsoft.Kubernetes/connectedClusters/AzureArcTest1 MyK8sTestCluster  /subscriptions/57ac26cf-a9f0-4908-b300-9a4e9a0fb205/resourcegroups/test-la-workspace-rg/providers/microsoft.operationalinsights/workspaces/test-la-workspace

set -e
set -u
set -o pipefail

# default to public cloud since only supported cloud is azure public clod
export defaultAzureCloud="AzureCloud"

# this needs to be updated once code moved to ci_dev or ci_prod branch completly
export solutionTemplateUri="https://raw.githubusercontent.com/microsoft/OMS-docker/ci_feature/docs/templates/azuremonitor-containerSolution.json"

# default release name used during onboarding
export releasename="azmon-containers-release-1"

# resource type for azure arc clusters
export resourceProvider="Microsoft.Kubernetes/connectedClusters"

# cluster resource details
export clusterResourceId=""

# resource type for azure log analytics workspace
export workspaceResourceProvider="Microsoft.OperationalInsights/workspaces"

# workspace resource details
export workspaceResourceId=""
export kubeconfigContext=""
export proxyEndpoint=""

# default workspace region and code
export workspaceRegion="eastus"
export workspaceRegionCode="EUS"
export workspaceResourceGroup="DefaultResourceGroup-"$workspaceRegionCode
export workspaceName=""
export workspaceGuid=""
export workspaeKey=""

usage()
{
    local basename=`basename $0`
    echo
    echo "Enable Azure Monitor for containers:"
    echo "-------------------
    $basename -r <resource id of the cluster> -k <name of the kube context to use> [-w <resource id of existing workspace>] [-p <proxy endpoint>]
            or
    $basename --resource-id <resource id of the cluster> --kube-context <name of the kube context to use> [--workspace-id <resource id of existing workspace>] [--proxy <proxy endpoint>]
    --------------------------"
}

validate_params()
{

 clusterSubscriptionId="$(echo ${1} | cut -d'/' -f3)"
 clusterResourceGroup="$(echo ${1} | cut -d'/' -f5)"
 providerName="$(echo ${1} | cut -d'/' -f7)"
 clusterName="$(echo ${1} | cut -d'/' -f9)"
 # convert to lowercase for validation
 providerName=$(echo $providerName | tr "[:upper:]" "[:lower:]")

 kubeconfigContext="$(echo ${2})"

 echo "cluster SubscriptionId:" $subscriptionId
 echo "cluster ResourceGroup:" $resourceGroup
 echo "cluster ProviderName:" $providerName
 echo "cluster Name:" $clusterName

 if [ -z "$subscriptionId" -o -z "$resourceGroup" -o -z "$providerName" -o  -z "$clusterName" ]; then
    echo "-e invalid cluster resource id. Please try with valid fully qualified resource id of the cluster"
    exit 1
 fi

 if [[ $providerName != microsoft.* ]]; then
   echo "-e invalid azure cluster resource id format."
   exit 1
 fi

 if [ -z "$kubeconfigContext" ]; then
    echo "-e kubeconfig context is empty. Please try with valid kube-context of the cluster"
    exit 1
 fi

 workspaceId="$(echo ${3})"
 if [ ! -z "$workspaceId" ]; then
    workspaceSubscriptionId="$(echo $workspaceId | cut -d'/' -f3)"
    workspaceResourceGroup="$(echo $workspaceId | cut -d'/' -f5)"
    workspaceProviderName="$(echo $workspaceId | cut -d'/' -f7)"
    workspaceName="$(echo $workspaceId | cut -d'/' -f9)"
    # convert to lowercase for validation
    workspaceProviderName=$(echo $workspaceProviderName | tr "[:upper:]" "[:lower:]")
    echo "workspace SubscriptionId:" $workspaceSubscriptionId
    echo "workspace ResourceGroup:" $workspaceResourceGroup
    echo "workspace ProviderName:" $workspaceName
    echo "workspace Name:" $clusterName

   if [[ $workspaceProviderName != microsoft.operationalinsights* ]]; then
     echo "-e invalid azure cluster resource id format."
     exit 1
   fi
 fi

 proxyEndpoint="$(echo ${4})"
 if [ ! -z "$proxyEndpoint" ]; then
    # Validate Proxy Endpoint URL
    # extract the protocol://
    proto="$(echo $proxyEndpoint | grep :// | sed -e's,^\(.*://\).*,\1,g')"
    # convert the protocol prefix in lowercase for validation
    proxyprotocol=$(echo $proto | tr "[:upper:]" "[:lower:]")
    if [ "$proxyprotocol" != "http://" -a "$proxyprotocol" != "https://" ]; then
      echo "-e error proxy endpoint should be in this format http(s)://<user>:<pwd>@<hostOrIP>:<port>"
    fi
    # remove the protocol
    url="$(echo ${proxyEndpoint/$proto/})"
    # extract the creds
    creds="$(echo $url | grep @ | cut -d@ -f1)"
    user="$(echo $creds | cut -d':' -f1)"
    pwd="$(echo $creds | cut -d':' -f2)"
    # extract the host and port
    hostport="$(echo ${url/$creds@/} | cut -d/ -f1)"
    # extract host without port
    host="$(echo $hostport | sed -e 's,:.*,,g')"
    # extract the port
    port="$(echo $hostport | sed -e 's,^.*:,:,g' -e 's,.*:\([0-9]*\).*,\1,g' -e 's,[^0-9],,g')"

    if [ -z "$user" -o -z "$pwd" -o -z "$host" -o -z "$port" ]; then
      echo "-e error proxy endpoint should be in this format http(s)://<user>:<pwd>@<hostOrIP>:<port>"
    else
      echo "successfully validated provided proxy endpoint is valid and expected format"
    fi
 fi

}

parse_args()
{

 if [ $# -le 2 ]
  then
    usage
    exit 1
 fi

# Transform long options to short ones
for arg in "$@"; do
  shift
  case "$arg" in
    "--resource-id")  set -- "$@" "-r" ;;
    "--kube-context") set -- "$@" "-k" ;;
    "--workspace-id") set -- "$@" "-w" ;;
    "--proxy") set -- "$@" "-p" ;;
    "--"*)   usage ;;
    *)        set -- "$@" "$arg"
  esac
done

local OPTIND opt

while getopts 'hk:r:w:p:' opt; do
    case "$opt" in
      h)
      usage
        ;;

      k)
        kubeconfigContext="$OPTARG"
        echo "name of kube-context is $OPTARG"
        ;;

      r)
        clusterResourceId="$OPTARG"
        echo "clusterResourceId is $OPTARG"

        ;;

      w)
        workspaceResourceId="$OPTARG"
        echo "workspaceResourceId is $OPTARG"
        ;;

      p)
        proxyEndpoint="$OPTARG"
        echo "proxyEndpoint is $OPTARG"
        ;;
      ?)
        usage
        exit 1
        ;;
    esac
  done
  shift "$(($OPTIND -1))"

}

configure_to_public_cloud()
{
  echo "Set AzureCloud as active cloud for az cli"
  az cloud set -n $AzureCloud
}

validate_cluster_identity()
{
  identitytype=$(az resource show -g ${resourceGroup} -n ${clusterName} --resource-type $resourceProvider --query identity.type)
  identitytype=$(echo "$identitytype" | tr "[:upper:]" "[:lower:]")
  echo "cluster identity type:" $identitytype
}

create_default_log_analytics_workspace()
{
  echo "Using or creating default Log Analytics Workspace since workspaceResourceId parameter not set..."

  subscriptionId="$(echo ${1})"

  clusterRegion=$(az resource show --ids ${subscriptionId} --query location)
  echo "cluster region:" $clusterRegion


  # mapping fors for default Azure Log Analytics workspace
  declare -A AzureCloudLocationToOmsRegionCodeMap=(
  [australiasoutheast]=ASE
  [australiaeast]=EAU
  [australiacentral]=CAU
  [canadacentral]=CCA
  [centralindia]=CIN
  [centralus]=CUS
  [eastasia]=EA
  [eastus]=EUS
  [eastus2]=EUS2
  [eastus2euap]=EAP
  [francecentral]=PAR
  [japaneast]=EJP
  [koreacentral]=SE
  [northeurope]=NEU
  [southcentralus]=SCUS
  [southeastasia]=SEA
  [uksouth]=SUK
  [usgovvirginia]=USGV
  [westcentralus]=EUS
  [westeurope]=WEU
  [westus]=WUS
  [westus2]=WUS2
  )

  declare -A AzureCloudRegionToOmsRegionMap=(
  [australiacentral]=australiacentral
  [australiacentral2]=australiacentral
  [australiaeast]=australiaeast
  [australiasoutheast]=australiasoutheast
  [brazilsouth]=southcentralus
  [canadacentral]=canadacentral
  [canadaeast]=canadacentral
  [centralus]=centralus
  [centralindia]=centralindia
  [eastasia]=eastasia
  [eastus]=eastus
  [eastus2]=eastus2
  [francecentral]=francecentral
  [francesouth]=francecentral
  [japaneast]=japaneast
  [japanwest]=japaneast
  [koreacentral]=koreacentral
  [koreasouth]=koreacentral
  [northcentralus]=eastus
  [northeurope]=northeurope
  [southafricanorth]=westeurope
  [southafricawest]=westeurope
  [southcentralus]=southcentralus
  [southeastasia]=southeastasia
  [southindia]=centralindia
  [uksouth]=uksouth
  [ukwest]=uksouth
  [westcentralus]=eastus
  [westeurope]=westeurope
  [westindia]=centralindia
  [westus]=westus
  [westus2]=westus2
  )

  if [ -n "${AzureCloudRegionToOmsRegionMap[$clusterRegion]}" ];
  then
    workspaceRegion=${AzureCloudRegionToOmsRegionMap[$clusterRegion]}
  fi
  echo "Workspace Region:"$workspaceRegion

  if [ -n "${AzureCloudLocationToOmsRegionCodeMap[$workspaceRegion]}" ];
  then
    workspaceRegionCode=${AzureCloudLocationToOmsRegionCodeMap[$workspaceRegion]}
  fi
  echo "Workspace Region Code:"$workspaceRegionCode

  workspaceResourceGroup="DefaultResourceGroup-"$workspaceRegionCode
  isRGExists=$(az group exists -g $workspaceResourceGroup)
  workspaceName="DefaultWorkspace-"$subscriptionId"-"$workspaceRegionCode

  if $isRGExists
  then echo "using existing default resource group:"$workspaceResourceGroup
  else
    echo "creating resource group: $workspaceResourceGroup in region: $workspaceRegion"
    az group create -g $workspaceResourceGroup -l $workspaceRegion
  fi

  workspaceList=$(az resource list -g $workspaceResourceGroup -n $workspaceName  --resource-type $workspaceResourceProvider)
  if [ "$workspaceList" = "[]" ];
  then
  # create new default workspace since no mapped existing default workspace
  echo '{"location":"'"$workspaceRegion"'", "properties":{"sku":{"name": "standalone"}}}' > WorkspaceProps.json
  cat WorkspaceProps.json
  workspace=$(az resource create -g $workspaceResourceGroup -n $workspaceName --resource-type $workspaceResourceProvider --is-full-object -p @WorkspaceProps.json)
  else
    echo "using existing default workspace:"$workspaceName
  fi

  workspaceResourceId=$(az resource show -g $workspaceResourceGroup -n $workspaceName  --resource-type $workspaceResourceProvider --query id)
  workspaceResourceId=$(echo $workspaceResourceId | tr -d '"')
}

add_container_insights_solution()
{
  resourceId="$(echo ${1})"

  # extract resource group from workspace resource id
  resourceGroup="$(echo ${resourceId} | cut -d'/' -f5)"

  echo "adding containerinsights solution to workspace"
  solution=$(az deployment group create -g $resourceGroup --template-uri $solutionTemplateUri --parameters workspaceResourceId=$resourceId --parameters workspaceRegion=$workspaceRegion)
}

get_workspace_guid_and_key()
{
  # extract resource parts from workspace resource id
  resourceId="$(echo ${1})"
  resourceId=$(echo $resourceId | tr -d '"')
  subId="$(echo ${resourceId} | cut -d'/' -f3)"
  rgName="$(echo ${resourceId} | cut -d'/' -f5)"
  wsName="$(echo ${resourceId} | cut -d'/' -f9)"

  # get the workspace guid
  workspaceGuid=$(az resource show -g $rgName -n $wsName --resource-type $workspaceResourceProvider --query properties.customerId)
  workspaceGuid=$(echo $workspaceGuid | tr -d '"')
  echo "workspaceGuid:"$workspaceGuid

  echo "getting workspace primaryshared key"
  workspaceKey=$(az rest --method post --uri $workspaceResourceId/sharedKeys?api-version=2015-11-01-preview --query primarySharedKey)
  workspaceKey=$(echo $workspaceKey | tr -d '"')
}

install_helm_chart()
{

 echo "installing Azure Monitor for containers HELM chart ..."

 echo "adding helm incubator repo"
 helm repo add incubator https://kubernetes-charts-incubator.storage.googleapis.com/

 echo "updating helm repo to get latest charts"
 helm repo update

 helm upgrade --install azmon-containers-release-1 --set omsagent.secret.wsid=$workspaceGuid,omsagent.secret.key=$workspaceKey,omsagent.env.clusterId=$clusterResourceId incubator/azuremonitor-containers --kube-context ${kubeconfigContext}
 echo "chart installation completed."

}

login_to_azure()
{
  echo "login to the azure interactively"
  az login --use-device-code
}

set_azure_subscription()
{
 subscriptionId="$(echo ${1})"
 echo "set the subscription id: ${subscriptionId}"
 az account set -s ${subscriptionId}

}

# parse args
parse_args $@

# validate parameters
validate_params $clusterResourceId $kubeconfigContext $workspaceResourceId $proxyEndpoint

# configure azure cli for public cloud
configure_to_public_cloud

# parse cluster resource id
clusterSubscriptionId="$(echo $clusterResourceId | cut -d'/' -f3)"
clusterResourceGroup="$(echo $clusterResourceId | cut -d'/' -f5)"
providerName="$(echo $clusterResourceId | cut -d'/' -f7)"
clusterName="$(echo $clusterResourceId | cut -d'/' -f9)"

# login to azure
login_to_azure

# set the cluster subscription id as active sub for azure cli
set_azure_subscription $clusterSubscriptionId

if [ -z $workspaceResourceId ]; then
  create_default_log_analytics_workspace $clusterSubscriptionId
else
  echo "using provided azure log analytics workspace:${workspaceResourceId}"
  workspaceResourceId=$(echo $workspaceResourceId | tr -d '"')
  workspaceSubscriptionId="$(echo ${workspaceResourceId} | cut -d'/' -f3)"
  workspaceResourceGroup="$(echo ${workspaceResourceId} | cut -d'/' -f5)"
  workspaceName="$(echo ${workspaceResourceId} | cut -d'/' -f9)"

  # set the azure subscription to azure cli
  echo "set the subscription id as active subscription for azure cli: ${workspaceSubscriptionId}"
  set_azure_subscription $workspaceSubscriptionId

  workspaceRegion=$(az resource show --ids ${workspaceResourceId} --query location)
  workspaceRegion=$(echo $workspaceRegion | tr -d '"')
  echo "Workspace Region:"$workspaceRegion
fi

# add container insights solution
add_container_insights_solution $workspaceResourceId

# get workspace guid and key
get_workspace_guid_and_key $workspaceResourceId

echo "set the cluster subscription id: ${clusterSubscriptionId}"
set_azure_subscription $clusterSubscriptionId

echo "attach loganalyticsworkspaceResourceId tag on to cluster resource"
status=$(az  resource update --set tags.logAnalyticsWorkspaceResourceId=$workspaceResourceId -g $clusterResourceGroup -n $clusterName --resource-type $resourceProvider)

install_helm_chart

# portal link
echo "Proceed to https://aka.ms/azmon-containers-azurearc to view health of your newly onboarded Azure Arc cluster"

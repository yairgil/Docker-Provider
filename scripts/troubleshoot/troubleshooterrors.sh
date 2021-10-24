#!/bin/bash
#
#  This script troubleshoots errors related to onboarding of Azure Monitor for containers to Kubernetes cluster hosted outside and connected to Azure via Azure Arc cluster
# Prerequisites :
#     Azure CLI:  https://docs.microsoft.com/en-us/cli/azure/install-azure-cli?view=azure-cli-latest

# bash troubelshooterror.sh --resource-id <clusterResourceId> --kube-context <kube-context> --cloudName

set -e
set -o pipefail

logFile="TroubleshootDump.log"
clusterType="connectedClusters"
extensionInstanceName="azuremonitor-containers"
# resource type for azure log analytics workspace
workspaceResourceProvider="Microsoft.OperationalInsights/workspaces"
workspaceSolutionResourceProvider="Microsoft.OperationsManagement/solutions"

write_to_log_file() {
  echo "$@"
  echo "$@" >> $logFile
}

login_to_azure() {
  if [ "$isUsingServicePrincipal" = true ]; then
    write_to_log_file "login to the azure using provided service principal creds"
    az login --service-principal --username="$servicePrincipalClientId" --password="$servicePrincipalClientSecret" --tenant="$servicePrincipalTenantId"
  else
    write_to_log_file "login to the azure interactively"
    az login --use-device-code
  fi
}

set_azure_subscription() {
  local subscriptionId="$(echo ${1})"
  write_to_log_file "setting the subscription id: ${subscriptionId} as current subscription for the azure cli"
  az account set -s ${subscriptionId}
  write_to_log_file "successfully configured subscription id: ${subscriptionId} as current subscription for the azure cli"
}

usage() {
  local basename=$(basename $0)
  echo
  echo "Troubleshooting Errors related to Azure Monitor for containers:"
  echo "$basename --resource-id <cluster resource id> [--kube-context <name of the kube context >]"
}

parse_args() {

  if [ $# -le 1 ]; then
    usage
    exit 1
  fi

  # Transform long options to short ones
  for arg in "$@"; do
    shift
    case "$arg" in
    "--resource-id") set -- "$@" "-r" ;;
    "--kube-context") set -- "$@" "-k" ;;
    "--"*) usage ;;
    *) set -- "$@" "$arg" ;;
    esac
  done

  local OPTIND opt

  while getopts 'hk:r:' opt; do
    case "$opt" in
    h)
      usage
      ;;

    k)
      kubeconfigContext="$OPTARG"
      write_to_log_file "name of kube-context is $OPTARG"
      ;;

    r)
      clusterResourceId="$OPTARG"
      write_to_log_file "clusterResourceId is $OPTARG"
      ;;

    ?)
      usage
      exit 1
      ;;
    esac
  done
  shift "$(($OPTIND - 1))"

  local subscriptionId="$(echo ${clusterResourceId} | cut -d'/' -f3)"
  local resourceGroup="$(echo ${clusterResourceId} | cut -d'/' -f5)"

  # get resource parts and join back to get the provider name
  local providerNameResourcePart1="$(echo ${clusterResourceId} | cut -d'/' -f7)"
  local providerNameResourcePart2="$(echo ${clusterResourceId} | cut -d'/' -f8)"
  local providerName="$(echo ${providerNameResourcePart1}/${providerNameResourcePart2})"

  local clusterName="$(echo ${clusterResourceId} | cut -d'/' -f9)"

  # convert to lowercase for validation
  providerName=$(echo $providerName | tr "[:upper:]" "[:lower:]")

  write_to_log_file "cluster SubscriptionId:" $subscriptionId
  write_to_log_file "cluster ResourceGroup:" $resourceGroup
  write_to_log_file "cluster ProviderName:" $providerName
  write_to_log_file "cluster Name:" $clusterName

  if [ -z "$subscriptionId" -o -z "$resourceGroup" -o -z "$providerName" -o -z "$clusterName" ]; then
    write_to_log_file "-e invalid cluster resource id. Please try with valid fully qualified resource id of the cluster"
    exit 1
  fi

  if [[ $providerName != microsoft.* ]]; then
    write_to_log_file "-e invalid azure cluster resource id format."
    exit 1
  fi

  # detect the resource provider from the provider name in the cluster resource id
  if [ $providerName = "microsoft.kubernetes/connectedclusters" ]; then
    write_to_log_file "provider cluster resource is of Azure Arc enabled Kubernetes cluster type"
    isArcK8sCluster=true
    resourceProvider=$arcK8sResourceProvider
  else
    write_to_log_file "-e not valid azure arc enabled kubernetes cluster resource id"
    exit 1
  fi

  if [ -z "$kubeconfigContext" ]; then
    write_to_log_file "using or getting current kube config context since --kube-context parameter not set "
  fi

  if [ ! -z "$servicePrincipalClientId" -a ! -z "$servicePrincipalClientSecret" -a ! -z "$servicePrincipalTenantId" ]; then
    write_to_log_file "using service principal creds (clientId, secret and tenantId) for azure login since provided"
    isUsingServicePrincipal=true
  fi
}

command_exists() {
	command -v "$@" > /dev/null 2>&1
}

validate_ci_extension () {
  extension=$(az k8s-extension show -c ${4} -g ${3} -t $clusterType -n $extensionInstanceName)
  write_to_log_file $extension
  configurationSettings=$(az k8s-extension show -c ${4} -g ${3} -t $clusterType -n $extensionInstanceName --query "configurationSettings.logAnalyticsWorkspaceResourceID")
  if [ -z "$configurationSettings" ]; then
     write_to_log_file "-e error configurationSettings either null or empty"
     exit 1
  fi
  logAnalyticsWorkspaceResourceID=$(az k8s-extension show -c ${4} -g ${3} -t $clusterType -n $extensionInstanceName --query "configurationSettings.logAnalyticsWorkspaceResourceID")
  if [ -z "$logAnalyticsWorkspaceResourceID" ]; then
     write_to_log_file "-e error logAnalyticsWorkspaceResourceID either null or empty in the config settings"
     exit 1
  fi

  provisioningState=$(az k8s-extension show -c ${4} -g ${3} -t $clusterType -n $extensionInstanceName  --query "provisioningState")
  if [ -z "$provisioningState" ]; then
     write_to_log_file "-e error provisioningState either null or empty in the config settings"
     exit 1
  fi
  if [ $provisioningState = "Succeeded" ]; then
     write_to_log_file "-e error expected state of extension provisioningState MUST be Succeeded state but actual state is ${provisioningState}"
     exit 1
  fi
  logAnalyticsWorkspaceDomain=$(az k8s-extension show -c ${4} -g ${3} -t $clusterType -n $extensionInstanceName --query 'configurationSettings."omsagent.domain"')
  if [ -z "$logAnalyticsWorkspaceDomain" ]; then
     write_to_log_file "-e error logAnalyticsWorkspaceDomain either null or empty in the config settings"
     exit 1
  fi
  azureCloudName=${1}
  if [ "$azureCloudName" = "azureusgovernment" ]; then
     if [ $logAnalyticsWorkspaceDomain = "opinsights.azure.us" ]; then
        write_to_log_file "-e error expected value of logAnalyticsWorkspaceDomain  MUST opinsights.azure.us but actual value is ${logAnalyticsWorkspaceDomain}"
        exit 1
     fi
  elif [ "$azureCloudName" = "azurecloud" ]; then
    if [ $logAnalyticsWorkspaceDomain = "opinsights.azure.com" ]; then
      write_to_log_file "-e error expected value of logAnalyticsWorkspaceDomain  MUST opinsights.azure.com but actual value is ${logAnalyticsWorkspaceDomain}"
      exit 1
    fi
  elif [ "$azureCloudName" = "azurechinacloud" ]; then
    if [ $logAnalyticsWorkspaceDomain = "opinsights.azure.cn" ]; then
      write_to_log_file "-e error expected value of logAnalyticsWorkspaceDomain  MUST opinsights.azure.cn but actual value is ${logAnalyticsWorkspaceDomain}"
      exit 1
    fi
  fi

  workspaceSubscriptionId="$(echo ${logAnalyticsWorkspaceResourceID} | cut -d'/' -f3 | tr "[:upper:]" "[:lower:]")"
  workspaceResourceGroup="$(echo ${logAnalyticsWorkspaceResourceID} | cut -d'/' -f5)"
  workspaceName="$(echo ${logAnalyticsWorkspaceResourceID} | cut -d'/' -f9)"

  clusterSubscriptionId=${2}
  # set the azure subscription to azure cli if the workspace in different sub than cluster
  if [[ "$clusterSubscriptionId" != "$workspaceSubscriptionId" ]]; then
    write_to_log_file "switch subscription id of workspace as active subscription for azure cli since workspace in different subscription than cluster: ${workspaceSubscriptionId}"
    isClusterAndWorkspaceInSameSubscription=false
    set_azure_subscription $workspaceSubscriptionId
  fi
  workspaceList=$(az resource list -g $workspaceResourceGroup -n $workspaceName --resource-type $workspaceResourceProvider)
  if [ "$workspaceList" = "[]" ]; then
     write_to_log_file "-e error workspace:${logAnalyticsWorkspaceResourceID} doesnt exist"
     exit 1
  fi

  ciSolutionResourceId="/subscriptions/${workspaceSubscriptionId}/resourceGroups/${workspaceResourceGroup}/Microsoft.OperationsManagement/solutions/ContainerInsights(${workspaceName})"
  ciSolutionResourceName=$(az resource show --ids "$ciSolutionResourceId"  --query name)
  if [[ "$ciSolutionResourceName" != "ContainerInsights(${workspaceName})" ]]; then
     write_to_log_file "-e error ContainerInsights solution on workspace ${logAnalyticsWorkspaceResourceID} doesnt exist"
     exit 1
  fi

  publicNetworkAccessForIngestion=$(az resource show --ids ${logAnalyticsWorkspaceResourceID} --query properties.publicNetworkAccessForIngestion)
  write_to_log_file "workspace publicNetworkAccessForIngestion: ${publicNetworkAccessForIngestion}"
  if [[ "$publicNetworkAccessForIngestion" != "Enabled" ]]; then
     write_to_log_file "-e error Unless private link configured, publicNetworkAccessForIngestion MUST be enabled for data ingestion"
     exit 1
  fi
  publicNetworkAccessForQuery=$(az resource show --ids ${logAnalyticsWorkspaceResourceID} --query properties.publicNetworkAccessForQuery)
  write_to_log_file "workspace publicNetworkAccessForQuery: ${publicNetworkAccessForQuery}"
  if [[ "$publicNetworkAccessForIngestion" != "Enabled" ]]; then
    write_to_log_file "-e error Unless private link configured, publicNetworkAccessForQuery MUST be enabled for data query"
    exit 1
  fi

  workspaceCappingDailyQuotaGb=$(az resource show --ids ${logAnalyticsWorkspaceResourceID} --query properties.workspaceCapping.dailyQuotaGb)
  write_to_log_file "workspaceCapping dailyQuotaGb: ${workspaceCappingDailyQuotaGb}"
  if [[ "$workspaceCappingDailyQuotaGb" != "1.0" ]]; then
    write_to_log_file "-e error workspace configured daily quota and verify ingestion data reaching over the quota: ${workspaceCappingDailyQuotaGb}"
    exit 1
  fi
}

if command_exists az; then
   write_to_log_file "detected azure cli installed"
   azCLIVersion=$(az -v)
   write_to_log_file "azure-cli version: ${azCLIVersion}"
   azCLIExtension=$(az extension list --query "[?name=='k8s-extension'].name | [0]")
   if [ $azCLIExtension = "k8s-extension" ]; then
      azCLIExtensionVersion=$(az extension list --query "[?name=='k8s-extension'].version | [0]")
      write_to_log_file "detected k8s-extension and current installed version: ${azCLIExtensionVersion}"
      az extension update --name 'k8s-extension'
   else
     write_to_log_file "adding k8s-extension since k8s-extension doesnt exist as installed"
     az extension add --name 'k8s-extension'
   fi
   azCLIExtensionVersion=$(az extension list --query "[?name=='k8s-extension'].version | [0]")
   write_to_log_file "current installed k8s-extension version: ${azCLIExtensionVersion}"
else
  write_to_log_file "-e error azure cli doesnt exist as installed"
  write_to_log_file "Please install Azure-CLI as per the instructions https://docs.microsoft.com/en-us/cli/azure/install-azure-cli and rerun the troubleshooting script"
  exit 1
fi

# parse and validate args
parse_args $@

# parse cluster resource id
clusterSubscriptionId="$(echo $clusterResourceId | cut -d'/' -f3 | tr "[:upper:]" "[:lower:]")"
clusterResourceGroup="$(echo $clusterResourceId | cut -d'/' -f5)"
providerName="$(echo $clusterResourceId | cut -d'/' -f7)"
clusterName="$(echo $clusterResourceId | cut -d'/' -f9)"

azureCloudName=$(az cloud show --query name -o tsv | tr "[:upper:]" "[:lower:]" | tr -d "[:space:]")
write_to_log_file "azure cloud name: ${azureCloudName}"

# login to azure interactively
login_to_azure

# set the cluster subscription id as active sub for azure cli
set_azure_subscription $clusterSubscriptionId

#validate ci extension
validate_ci_extension $azureCloudName $clusterSubscriptionId $clusterResourceGroup $clusterName
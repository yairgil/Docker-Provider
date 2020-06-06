#!/bin/bash
#
# Execute this directly in Azure Cloud Shell (https://shell.azure.com) by pasting (SHIFT+INS on Windows, CTRL+V on Mac or Linux)
# the following line (beginning with curl...) at the command prompt and then replacing the args:
#  This scripts disables monitoring on to monitoring enabled Azure ARC K8s cluster
#
#      1. Deletes the existing Azure Monitor for containers helm release
#      2. Deletes logAnalyticsWorkspaceResourceId tag on the provided Azure Arc Cluster
# Prerequisites :
#     Azure CLI:  https://docs.microsoft.com/en-us/cli/azure/install-azure-cli?view=azure-cli-latest
#     Helm3 : https://helm.sh/docs/intro/install/
#
# bash <script> --resource-id <azureArcResourceId> --kube-context <kube-context>
# bash <script> -r <azureArcResourceId> -k <kube-context>
# For example to disables azure monitor for containers on monitoring enabled Azure Arc K8s cluster
# bash disable_monitoring.sh -r /subscriptions/57ac26cf-a9f0-4908-b300-9a4e9a0fb205/resourceGroups/AzureArcTest/providers/Microsoft.Kubernetes/connectedClusters/AzureArcTest1 -k MyK8sTestCluster

set -e
set -u
set -o pipefail

# default release name used during onboarding
releasename="azmon-containers-release-1"
# resource type for azure arc clusters
resourceProvider="Microsoft.Kubernetes/connectedClusters"

usage()
{
    local basename=`basename $0`
    echo
    echo "Disable Azure Monitor for containers:"
    echo "-------------------
    $basename -r <resource id of the cluster> -k <name of the kubeconfig context to use>
            or
    $basename --resource-id <resource id of the cluster> --kube-context <name of the kubeconfig context to use>
    --------------------------"
}

delete_helm_release()
{
  echo "deleting chart release:" $releasename
  kubeconfigContext="$(echo ${1})"
  releases=$(helm list --filter $releasename --kube-context $kubeconfigContext)
  echo $releases
  if [[ "$releases" == *"$releasename"* ]]; then
    helm del $releasename --kube-context $kubeconfigContext
  else
    echo "there is no existing release of azure monitor for containers"
  fi
  echo "deletion of chart release done."
}

remove_monitoring_tags()
{
  echo "deleting monitoring tags ..."

  subscriptionId="$(echo ${1} | cut -d'/' -f3)"
  resourceGroup="$(echo ${1} | cut -d'/' -f5)"
  providerName="$(echo ${1} | cut -d'/' -f7)"
  clusterName="$(echo ${1} | cut -d'/' -f9)"

  echo "login to the azure interactively"
  az login --use-device-code

  echo "set the cluster subscription id: ${subscriptionId}"
  az account set -s ${subscriptionId}

  identitytype=$(az resource show -g ${resourceGroup} -n ${clusterName} --resource-type $resourceProvider --query identity.type)
  identitytype=$(echo "$identitytype" | tr "[:upper:]" "[:lower:]")
  echo "cluster identity type:" $identitytype

  echo "remove the value of loganalyticsworkspaceResourceId tag on to cluster resource"
  status=$(az resource update --set tags.logAnalyticsWorkspaceResourceId='' -g $resourceGroup -n $clusterName --resource-type $resourceProvider)

  echo "deleting of monitoring tags completed.."
}


validate_params()
{

 subscriptionId="$(echo ${1} | cut -d'/' -f3)"
 resourceGroup="$(echo ${1} | cut -d'/' -f5)"
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
    "--resource-id") set -- "$@" "-r" ;;
    "--kube-context") set -- "$@" "-k" ;;
    "--help")   set -- "$@" "-h" ;;
    "--"*)   usage ;;
    *)        set -- "$@" "$arg"
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
        echo "name of kube-context is $OPTARG"
        ;;

      r)
        clusterResourceId="$OPTARG"
        echo "clusterResourceId is $OPTARG"
        ;;
      ?)
        usage
        exit 1
        ;;
    esac
  done
  shift "$(($OPTIND -1))"
}


# parse args
parse_args $@

# validate parameters
validate_params $clusterResourceId $kubeconfigContext

# delete helm release
delete_helm_release $kubeconfigContext

# remove monitoring tags on the cluster resource to make fully off boarded
remove_monitoring_tags $clusterResourceId

echo "successfully disabled monitoring addon for cluster":$clusterResourceId
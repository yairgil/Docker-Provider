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
# bash <script> <azureArcResourceId> <kube-context>

# For example to disables azure monitor for containers on monitoring enabled Azure Arc K8s cluster
# bash disable_monitoring.sh /subscriptions/57ac26cf-a9f0-4908-b300-9a4e9a0fb205/resourceGroups/AzureArcTest/providers/Microsoft.Kubernetes/connectedClusters/AzureArcTest1 MyK8sTestCluster

usage()
{
    local basename=`basename $0`
    echo
    echo "Disable Azure Monitor for containers:"
    echo "$basename -resourceId <resource id of the cluster> -kube-context <name of the kubeconfig context to use>"
}

delete_helm_release()
{
  echo "deleting chart release:" $releasename
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

  identitytype=$(az resource show -g ${resourceGroup} -n ${clusterName} --resource-type "Microsoft.Kubernetes/connectedClusters" --query identity.type)
  identitytype=$(echo "$identitytype" | tr "[:upper:]" "[:lower:]")
  echo "cluster identity type:" $identitytype

  echo "remove the value of loganalyticsworkspaceResourceId tag on to cluster resource"
  status=$(az resource update --set tags.logAnalyticsWorkspaceResourceId='' -g $resourceGroup -n $clusterName --resource-type Microsoft.Kubernetes/connectedClusters)

  echo "deleting of monitoring tags completed.."
}


validate_params()
{

 subscriptionId="$(echo ${1} | cut -d'/' -f3)"
 resourceGroup="$(echo ${1} | cut -d'/' -f5)"
 providerName="$(echo ${1} | cut -d'/' -f7)"
 clusterName="$(echo ${1} | cut -d'/' -f9)"

 kubeconfigContext="$(echo ${2})"

 echo "cluster SubscriptionId:" $subscriptionId
 echo "cluster ResourceGroup:" $resourceGroup
 echo "cluster ProviderName:" $providerName
 echo "cluster Name:" $clusterName
 if [ -z "$subscriptionId" -o -z "$resourceGroup" -o -z "$providerName" -o  -z "$clusterName" ]; then
    echo "-e invalid cluster resource id. Please try with valid fully qualified resource id of the cluster"
    exit 1
 fi

 if [ -z "$kubeconfigContext" ]; then
    echo "-e kubeconfig context is empty. Please try with valid kube-context of the cluster"
    exit 1
 fi

}
echo "disable monitoring addon ..."

# release name used in the onboarding
export releasename="azmon-containers-release-1"
if [ $# -le 1 ]
then
  echo "-e This should be invoked with at least 2 arguments, clusterResourceId and kubeContext and logAnalyticsWorkspaceResourceId(optional)"
  usage
  exit 1
fi

echo "clusterResourceId:"${1}
echo "kubeconfig context:"${2}

clusterResourceId="$(echo ${1})"
kubeconfigContext="$(echo ${2})"

# validate parameters
validate_params $clusterResourceId $kubeconfigContext

# delete
delete_helm_release

remove_monitoring_tags $clusterResourceId

echo "successfully disabled monitoring addon for cluster":$clusterResourceId
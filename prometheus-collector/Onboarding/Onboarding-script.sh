#!/bin/bash
#
# Execute this directly in Azure Cloud Shell (https://shell.azure.com) by pasting (SHIFT+INS on Windows, CTRL+V on Mac or Linux)
# the following line (beginning with curl...) at the command prompt and then replacing the args:
# This script configures required artifacts for MAC and Grafana usage
# Azure CLI:  https://docs.microsoft.com/en-us/cli/azure/install-azure-cli?view=azure-cli-latest
#
#   [Required]  ${1}  subscriptionId    subscriptionId where resources are created
#   [Required]  ${2}  resourceGroup      resource group of the AKS cluster
#   [Required]  ${3}  monitoringAccountName           name of the AKS cluster
#   [Required]  ${3}  grafanaName           name of the AKS cluster
#   [Required]  ${3}  location           name of the AKS cluster
#   [Required]  ${3}  aksResourceId           name of the AKS cluster

#
# For example:
#
# bash Onboarding-script.sh "0e4773a2-8221-441a-a06f-17db16ab16d4" "rashmi-canary-template" "rashmi-canary-script-3" "rashmi-canary-grafana-2" "eastus2euap" "/subscriptions/0e4773a2-8221-441a-a06f-17db16ab16d4/resourcegroups/rashmi-canary-template/providers/Microsoft.ContainerService/managedClusters/rashmi-canary-template"
#
# Cross rg for aks resource -
# bash Onboarding-script.sh "0e4773a2-8221-441a-a06f-17db16ab16d4" "rashmi-canary-template" "rashmi-canary-script-3" "rashmi-canary-grafana-2" "eastus2euap" "/subscriptions/0e4773a2-8221-441a-a06f-17db16ab16d4/resourcegroups/rashmi-canary-template-2/providers/Microsoft.ContainerService/managedClusters/rashmi-canary-template-2"
#
# Cross sub for aks resource -
# bash Onboarding-script.sh "0e4773a2-8221-441a-a06f-17db16ab16d4" "rashmi-canary-template" "rashmi-canary-script-3" "rashmi-canary-grafana-2" "eastus2euap" /subscriptions/8f6da2d9-ff10-4800-9239-c7e0e8b3407f/resourcegroups/rashmi-canary-10/providers/Microsoft.ContainerService/managedClusters/rashmi-canary-10
echo "subscriptionId"= ${1}
echo "resourceGroup" = ${2}
echo "monitoringAccountName"= ${3}
echo "grafanaName"= ${4}
echo "location" = ${5}
echo "aksResourceId" = ${6}

# az login --use-device-code

subscriptionId=${1}
resourceGroup=${2}
monitoringAccountName=${3}
grafanaName=${4}
location=${5}
aksResourceId=${6}

macAccountLength=${#3}

# Checking for length of MAC name, since it is used in dc artifacts creation (max is 44)
if [ $macAccountLength -gt 24 ]
then
    echo "Monitoring account name is longer than 35 characters, please use a shorter name, exiting."
    exit 1
fi

trimmedLocation=$(echo $location | sed 's/ //g' | awk '{print tolower($0)}')
echo $trimmedLocation
if [ $trimmedLocation != "eastus2euap" ] && [ $trimmedLocation != "eastus" ] && [ $trimmedLocation != "eastus2" ] && [ $trimmedLocation != "weseurope" ]
then
    echo "Location not in a supported region - eastus, eastus2, westeurope"
    exit 1
fi

aksResourceSplitarray=($(echo $aksResourceId | tr "/" "\n"))
aksResourceIdLength=${#aksResourceSplitarray[@]}

if [ $aksResourceIdLength != 8 ]
then
    echo "Incorrect AKS Resource ID specified, please specify an id in this format - /subscriptions/00000000-0000-0000-0000-000000000000/resourcegroups/rg-name/providers/Microsoft.ContainerService/managedClusters/clustername"
    exit 1
fi

echo "Getting AKS Subscription id and resource group for DCR association..."
aksSubId=${aksResourceSplitarray[1]}
aksRgName=${aksResourceSplitarray[3]}
echo "AKSSubId: $aksSubId"
echo "AKSRg: $aksRgName"

#az login
az extension add -n amg
az account set -s $subscriptionId

az group create --location $trimmedLocation --name $resourceGroup

echo "Creating Grafana instance, if it doesnt exist: $grafanaName"
az grafana create -g $resourceGroup -n $grafanaName

grafanaSmsi=$(az grafana show -g $resourceGroup -n $grafanaName --query 'identity.principalId')
echo "Got System Assigned Identity for Grafana instance: $grafanaSmsi"
echo "Removing quotes from MSI"
grafanaSmsi=$(sed -e 's/^"//' -e 's/"$//' <<<"$grafanaSmsi")

#Template to create all resources required for MAC ingestion e2e
echo "Creating all resources required for MAC ingestion"
az deployment group create --resource-group $resourceGroup --template-file RootTemplate.json \
--parameters monitoringAccountName=$monitoringAccountName monitoringAccountLocation=$trimmedLocation \
targetAKSResource=$aksResourceId AKSSubId=$aksSubId AKSRg=$aksRgName

macId=$(az resource show -g $resourceGroup -n $monitoringAccountName --resource-type "Microsoft.Monitor/Accounts" --query 'id')
echo "Got MAC id: $macId"
echo "Removing quotes from MAC Id"
macId=$(sed -e 's/^"//' -e 's/"$//' <<<"$macId")

echo "Assigning MAC reader role to grafana's system assigned MSI"
az role assignment create --assignee-object-id $grafanaSmsi --assignee-principal-type ServicePrincipal --scope $macId --role "Monitoring Data Reader MAC"

promQLEndpoint=$(az resource show -g $resourceGroup -n $monitoringAccountName --resource-type "Microsoft.Monitor/Accounts" --query 'properties.metrics.prometheusQueryEndpoint')
echo "PromQLEndpoint: $promQLEndpoint"

macPromDataSourceConfig='{
    "id": 6,
    "uid": "prometheus-mac",
    "orgId": 1,
    "name": "Monitoring Account",
    "type": "prometheus",
    "typeLogoUrl": "",
    "access": "proxy",
    "url": PROM_QL_PLACEHOLDER,
    "password": "",
    "user": "",
    "database": "",
    "basicAuth": false,
    "basicAuthUser": "",
    "basicAuthPassword": "",
    "withCredentials": false,
    "isDefault": false,
    "jsonData": {
        "azureAuth": true,
        "azureCredentials": {
            "authType": "msi"
        },
        "azureEndpointResourceId": "https://prometheus.monitor.azure.com",
        "httpMethod": "POST",
        "httpHeaderName1": "x-ms-use-new-mdm-namespace"
    },
    "secureJsonData": {
        "httpHeaderValue1": "true"
    },
    "version": 1,
    "readOnly": false
}'


populatedMACPromDataSourceConfig=${macPromDataSourceConfig//PROM_QL_PLACEHOLDER/$promQLEndpoint}

az grafana data-source create -n $grafanaName --definition "$populatedMACPromDataSourceConfig"

echo "Downloading dashboards package"
wget https://github.com/microsoft/Docker-Provider/raw/prometheus-collector/prometheus-collector/dashboards.tar.gz
tar -zxvf dashboards.tar.gz 

echo "Creating dashboards"
for FILE in dashboards/*.json; do
    az grafana dashboard import -g $resourceGroup -n $grafanaName --overwrite --definition $FILE
done;

echo "Onboarding was completed successfully, please deploy the prometheus-collector helm chart for data collection"


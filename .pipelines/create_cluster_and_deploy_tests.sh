#!/bin/bash

# export resource_suffix=$(cat /dev/urandom | tr -dc 'a-z0-9' | fold -w 5 | head -n 1)
export resource_suffix=7m6am
export location=westus2

export resource_group_name=e2e-tests-$resource_suffix
export cluster_name=e2e-tests-$resource_suffix
export la_workspace_name=la-workspace-$resource_suffix


# # create cluster
# echo "Create resource group ($resource_group_name)"
# az group create -n $resource_group_name -l $location

# echo
# echo "Create cluster ($cluster_name)"
# az aks create -n $cluster_name -g $resource_group_name \
#     --node-count 1 \
#     --network-plugin azure \
#     --tags created_on="$(date)",created_by="ci_dev build pipeline",bestbefore=$(date -d "now 2 days") \
# #  --attach-acr

# echo
# echo "Add a windows node pool to the AKS cluster"
# az aks nodepool add --cluster-name $cluster_name -g $resource_group_name \
#     --name win \
#     --os-type Windows \
#     --node-count 1

# create workspace
# (note: the | tr -d '"'  removes double-quotes. They mess up the ARM template below)
export la_workspace_resource_id=$(az monitor log-analytics workspace create -g $resource_group_name -n $la_workspace_name | jq '.id' | tr -d '"')
export la_workspace_guid=$(az monitor log-analytics workspace create -g $resource_group_name -n $la_workspace_name | jq '.customerId' | tr -d '"')
export la_workspace_shared_key=$(az monitor log-analytics workspace get-shared-keys -g e2e-tests-7m6am -n workspace-7m6am | jq '.primarySharedKey' | tr -d '"')


az aks get-credentials -n $cluster_name -g $resource_group_name

# TODO: apply CI solution to workspace

solution_template=$(cat << EOF
{
    "\$schema": "https://schema.management.azure.com/schemas/2015-01-01/deploymentTemplate.json#",
    "contentVersion": "1.0.0.0",
    "resources": [
        {
            "type": "Microsoft.Resources/deployments",
            "name": "solution-deployment",
            "apiVersion": "2017-05-10",
            "subscriptionId": "[split('$la_workspace_resource_id','/')[2]]",
            "resourceGroup": "[split('$la_workspace_resource_id','/')[4]]",
            "properties": {
                "mode": "Incremental",
                "template": {
                    "\$schema": "https://schema.management.azure.com/schemas/2015-01-01/deploymentTemplate.json#",
                    "contentVersion": "1.0.0.0",
                    "parameters": {},
                    "variables": {},
                    "resources": [
                        {
                            "apiVersion": "2015-11-01-preview",
                            "type": "Microsoft.OperationsManagement/solutions",
                            "location": "$location",
                            "name": "[Concat('ContainerInsights', '(', split('$la_workspace_resource_id','/')[8], ')')]",
                            "properties": {
                                "workspaceResourceId": "$la_workspace_resource_id"
                            },
                            "plan": {
                                "name": "[Concat('ContainerInsights', '(', split('$la_workspace_resource_id','/')[8], ')')]",
                                "product": "[Concat('OMSGallery/', 'ContainerInsights')]",
                                "promotionCode": "",
                                "publisher": "Microsoft"
                            }
                        }
                    ]
                },
                "parameters": {}
            }
        }
    ]
}

EOF
)

echo $solution_template > temp_arm_template.json
az group deployment create --resource-group $resource_group_name --template-file temp_arm_template.json
rm temp_arm_template.json


# TODO: apply agent to cluster

# TODO: apply e2e tests to cluster
# TODO: build e2e test image as part of build pipeline


# echo
# echo "Delete all created resources"
# az aks delete -n $cluster_name -g $resource_group_name -y --no-wait
# az group delete -n $resource_group_name -y


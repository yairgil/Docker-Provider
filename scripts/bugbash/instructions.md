# Instructions to enable monitoring with custom settings

1. Disable existing monitoring addon on existing cluster
```
  az account set -s <subscriptionOfthecluster>
  # get workspaceResourceId id of the configured workspace and this will be used in step 2
   az aks show -g <resourceGroupNameofCluster> -n <nameofTheCluster> | grep logAnalyticsWorkspaceResourceID
   #
   az aks disable-addons -a monitoring -g <resourceGroupNameofCluster> -n <nameofTheCluster>
```
2. Onboard monitoring addon without omsagent (i.e. omsagent => false in the addon profile)

```
# download existing cluster onboarding template and parameter file
curl -LO https://raw.githubusercontent.com/microsoft/Docker-Provider/gangams/bug-bash-instructions/scripts/bugbash/existingClusterOnboarding.json
curl -LO https://raw.githubusercontent.com/microsoft/Docker-Provider/gangams/bug-bash-instructions/scripts/bugbash/existingClusterParam.json

# update aksResourceId, aksResourceLocation and workspaceResourceId in existingClusterParam.json, and execute below command

az group deployment create --resource-group <resourceGroupNameofCluster> --template-file ./existingClusterOnboarding.json --parameters @./existingClusterParam.json

```
3. If cluster is MSI enabled then, for Metrics USER_ASSIGNED_IDENTITY_CLIENT_ID value needs to be provided and here are the instructions 

 # get the existing UAI on the vmss and assign with Metrics Publisher role permission
 az vmss identity show -n <vmss-name> -g <MC-resource-group of the cluster>

 # metrics role assignment 
 az role assignment create --assignee <clientIdofUAI> --scope <clusterResourceId> --role "Monitoring Metrics Publisher" 

az role assignment create --assignee "766650bd-d06b-403d-b1ad-26c0fa12da5c" --scope "/subscriptions/72c8e8ca-dc16-47dc-b65c-6b5875eb600a/resourcegroups/gangams-aks-win-test/providers/Microsoft.ContainerService/managedClusters/gangams-aks-win-test" --role "Monitoring Metrics Publisher" 

4. Replace the place holders in the omsagent.yaml and deploy the yaml with targeted image (Linux and/or Windows)

```
# verify you have the context of the cluster you want to use
kubectl config current-context

# downdload omsagent.yaml from dev or targted branch
curl -LO https://raw.githubusercontent.com/microsoft/Docker-Provider/ci_dev/kubernetes/omsagent.yaml

# replace the existing image with targeted linux and/or windows agent image
# replace the VALUE_AKS_RESOURCE_ID_VALUE, VALUE_AKS_RESOURCE_REGION_VALUE, WSID and Key values in the omsagent.yaml
  > Note: values of WSID and Key has to be base64 encoded and can obtained from azure portal -> log analytics workspace->settings -> advanced settings-> agents management
 # apply the omsagent.yaml
 kubectl apply -f omsagent.yaml
```

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
curl -LO https://raw.githubusercontent.com/microsoft/Docker-Provider/gangams/fbit-customizations/scripts/custom-configuration/fbit/existingClusterOnboarding.json
curl -LO https://raw.githubusercontent.com/microsoft/Docker-Provider/gangams/fbit-customizations/scripts/custom-configuration/fbit/existingClusterParam.json

# update aksResourceId, aksResourceLocation and workspaceResourceId in existingClusterParam.json, and execute below command

az group deployment create --resource-group <resourceGroupNameofCluster> --template-file ./existingClusterOnboarding.json --parameters @./existingClusterParam.json

```
3. Replace the place holders in the omsagent.yaml and deploy the yaml

```
# verify you have the context of the cluster you want to use
kubectl config current-context

# downdload omsagent.yaml
curl -LO https://raw.githubusercontent.com/microsoft/Docker-Provider/gangams/fbit-customizations/scripts/custom-configuration/fbit/omsagent.yaml

# replace the VALUE_AKS_RESOURCE_ID_VALUE, VALUE_AKS_RESOURCE_REGION_VALUE, WSID and Key values in the omsagent.yaml
  > Note: values of WSID and Key has to be base64 encoded and can obtained from azure portal -> log analytics workspace->settings -> advanced settings-> agents management
 # apply the omsagent.yaml
 kubectl apply -f omsagent.yaml
```
# Instructions to Enable CI extension DCR for AKS clusters

1. Get the Subscription whitelisted in AMCS for CI Extension
2. Create AKS & LA workspace in `eastus2euap` region 
3. Update the [ci-dcr.json](https://raw.githubusercontent.com/microsoft/Docker-Provider/gangams/enable-aad-auth-linux/scripts/dcr-onboarding/ci-dcr.json) file with workspace resource id & region 
   > Note - Only tested Microsoft-Perf stream so far and waiting for AMCS deployment other streams
4. Create CI Extension DCR in the resource group of LA workspace using this https://docs.microsoft.com/en-us/rest/api/monitor/datacollectionrules/create#code-try-0
5. Associate CI extension DCR to AKS cluster using this https://docs.microsoft.com/en-us/rest/api/monitor/datacollectionruleassociations/create#code-try-0
6. Deploy the Monitoring addon with ARM template with `enabled: false`

```
# download existing cluster onboarding template and parameter file
curl -LO https://raw.githubusercontent.com/microsoft/Docker-Provider/gangams/fbit-customizations/scripts/custom-configuration/fbit/existingClusterOnboarding.json
 > Note - Update below parameter file with AKS cluster, LA workspace resource id and other parameters
curl -LO https://raw.githubusercontent.com/microsoft/Docker-Provider/gangams/fbit-customizations/scripts/custom-configuration/fbit/existingClusterParam.json
# update aksResourceId, aksResourceLocation and workspaceResourceId in existingClusterParam.json, and execute below command
az group deployment create --resource-group <resourceGroupNameofCluster> --template-file ./existingClusterOnboarding.json --parameters @./existingClusterParam.json
```

7. Assign the System assigned identity to VMSS nodes (Linux & windows) of the AKS cluster

```
 # set the subscription context
 az account set -s <subscriptionIdOftheCluster>

 # update the resource group and cluster name for your cluster
 > Note -  resource group  here is MC resource group of the cluster name
 az vmss identity assign -g <MC Resource Group> -n <VMSS name>
```




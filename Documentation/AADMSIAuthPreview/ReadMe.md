Note - This is limited private preview. For any support issues, please reach out to us at [askcoin@microsoft.com](mailto:askcoin@microsoft.com). Please don't open a support ticket.
This private preview supports AAD MSI Auth using System Identity for Azure Kubernetes Clusters with Managed Identity.

# Azure Monitor Container Insights
No change in the functionality and features whats supported and available - https://docs.microsoft.com/en-us/azure/azure-monitor/containers/container-insights-overview

# Pre-requisites
 1. Register this feature on to your Azure subscription where you want to enable AAD MSI auth preview feature
     ```bash
     az account set -s <subscriptionId>
     az feature register --name OmsagentUseAADMSIAuthPreview --namespace Microsoft.ContainerService
     ```
 2. AKS Cluster(s) MUST be with System Assigned Managed Identity
    Verify whether  AKS Cluster with Managed Identity or not using below command.
     ``` bash
       az aks show -g <clusterResourceGroupName> -n <clusterName> --query "servicePrincipalProfile"
     ```
     The o/p of this command should be `{ "clientId": "msi" }`
  3. AKS Cluster(s) with Service Principal MUST be upgraded to use Managed Identity(MI). Refer https://docs.microsoft.com/en-us/azure/aks/use-managed-identity on how to upgrade to MI.
  4. Install Azure CLI version 2.26.0 or higher and AKS-Preview CLI version 0.5.22 or higher.

     a. Install [Azure CLI](https://docs.microsoft.com/en-us/cli/azure/install-azure-cli) based on your platform

     b. Install aks-preview CLI version
        ``` bash
         az extension add --name aks-preview
        ```
     c. Verify Azure CLI version is 2.26.0 or higher and aks-preview CLI version 0.5.22 or higher using below command
        ``` bash
          az version
        ```

## How to onboard Container Insights with AAD MSI Auth

> Note: `--enable-msi-auth-for-monitoring` newly flag introduced  for Monitoring addon AAD MSI auth enablement

### Existing AKS Clusters

``` bash
  az aks enable-addons -a monitoring --enable-msi-auth-for-monitoring -g <clusterResourceGroup> -n <clusterName>
```

### New AKS Clusters
> Note: If you want to windows node pools or other features, you can included in the command
``` bash
  az aks create  -g <clusterResourceGroup> -n <clusterName> --enable-addons monitoring --enable-msi-auth-for-monitoring
```

### Known Issues
- For enabling of recommended alerts in AKS Insights page, you may need to click "Enable" button though its not required  with AAD MSI Auth but will be addressing this post preview release
- This preview feature supported only in Azure Public cloud regions.
- AAD MSI Auth feature not supported for AKS Clusters with [Bring your own Control Plane Managed Identity](https://docs.microsoft.com/en-us/azure/aks/use-managed-identity#bring-your-own-control-plane-mi)

# Instructions to enable monitoring with custom settings

## 1. Disable existing monitoring addon on existing cluster
```
  az account set -s <subscriptionOfthecluster>
  az aks disable-addons -a monitoring -g <resourceGroupNameofCluster> -n <nameofTheCluster>
```
## 2. Onboard monitoring addon without omsagent (i.e. omsagent => false in the addon profile)

```


```

## 3. Replace the place holders in the omsagent.yaml and deploy the yaml
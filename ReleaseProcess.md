# Release Instructions
Last updated 4/23/2021

# 1. Production agent image and automatic deployment to CIPROD cluster

Here are the high-level instructions to get the CIPROD`<MM><DD><YYYY>` image for the production release
1. create feature branch from ci_dev
   - update the following:
      <!--      > Note: This required since Azure Dev Ops pipeline doesnt support --build-arg yet to automate this.    -- What does this line mean? -->
      - Ensure IMAGE_TAG updated with release candiate image tag in kubernetes/linux/Dockerfile and kubernetes/windows/Dockerfile
      - Update build version (dockerProviderVersion) and date in build/version
      - Update the image tag and dockerProviderVersion, and any other deployment changes, in kubernetes/omsagent.yaml
      - Update the chart version in charts/azuremonitor-containers/Chart.yaml
      - Update the image tag and dockerProviderVersion version in charts/azuremonitor-containers/values.yaml 
      - Update chart version in scripts/onboarding/managed/enable-monitoring.ps1, scripts/onboarding/managed/enable-monitoring.sh, and scripts/onboarding/managed/upgrade-monitoring.sh
      - Release notes
   - Create a PR to merge these changes back into ci_dev
2. Changes in ci_dev are automatically deployed to the CIDEV cluster (in the build subscription) so validate E2E to make sure everthing works
3. <a name="create_ci_prod_pr_anchor">If everything validated in DEV, make a release branch off ci_prod, pull in changes from ci_dev, then make a PR to merge the release branch back into ci_prod. Request the dev team review the PR but don't merge it yet.</a>
4. Go to the [release pipeline](https://github-private.visualstudio.com/microsoft/_releaseDefinition?definitionId=11&_a=environments-editor-preview) and update the variables with the new chart version and image tag
   1. From the release pipeline overview screen, navigate to variables -> Variable groups -> manage variable groups. Create a new variable group with the following variables:
      - CIHELMCHARTVERSION <VersionValue> # For example, 2.7.4
      - CIImageTagSuffix <ImageTag> # ciprod08072020 or ciprod08072020-1 etc.
   2. Go back to the release pipeline overview screen, navigate to variables -> Variable groups -> Link variable group, and select the variable group you just created
   3. Save the pipeline
5. Merge the PR you created [before the last step](#create_ci_prod_pr_anchor) into ci_prod. This will trigger automatic deployment of latest bits to CIPROD cluster with CIPROD`<MM><DD><YYYY>` image to test and scale cluters, AKS, AKS-Engine
   - Monitor the release [here](https://github-private.visualstudio.com/microsoft/_build?definitionId=243). The build pipeline is not stable yet, so make sure it completes sucessfully
   - Note: production image automatically pushed to CIPROD Public cloud ACR which will inturn replicated to Public cloud MCR.
6. Validate all the scenarios against clusters in build subscription and scale clusters. 
   - Deploy latest omsagent yaml with release candidate agent image in to supported k8s versions and validate all the critical scenarios. In perticular, throughly validate the updates going as part of this release and also make sure no regressions. If  this passes, deploy onto scale cluster and  validate perf and scale aspects. Scale cluster in AME cloud and co-ordinate with agent team who has access to this cluster to deploy the release candiate onto this cluster.
7. Delete the scale cluster when you are done validating (it's in AME)


# 2. Production Image to MCR CN for Azure China Cloud

No action required here, images are automatically synched to MCR CN from Public cloud MCR.

# 3. Release of the agent

## AKS

TODO:
- agent baker (before 8pm friday) - reference link: https://github.com/Azure/AgentBaker/pull/775
- make PR against AKS RP (monday)

## AKS-Engine

Make PR against [AKS-Engine](https://github.com/Azure/aks-engine). Refer PR https://github.com/Azure/aks-engine/pull/2318

## Arc for Kubernetes 

Ev2 pipeline used to deploy the chart of the Arc K8s Container Insights Extension as per Safe Deployment Process. 
Here is the high level process
```
 1. Specify chart version of the release candidate and trigger [container-insights-arc-k8s-extension-ci_prod-release](https://github-private.visualstudio.com/microsoft/_release?_a=releases&view=all)
 2. Get the approval from one of team member for the release
 3. Once the approved, release should be triggered automatically 
 4. use `cimon-arck8s-eastus2euap` for validating latest release in canary region
 5. TBD - Notify vendor team for the validation on all Arc K8s supported platforms
```

## Microsoft Charts Repo release for On-prem K8s

Since HELM charts repo being deprecated, Microsoft charts repo being used for HELM chart release of on-prem K8s clusters. 
To make chart release PR, fork [Microsoft-charts-repo]([https://github.com/microsoft/charts/tree/gh-pages) and make the PR against `gh-pages` branch of the upstream repo. 

Refer PR - https://github.com/microsoft/charts/pull/23 for example.
Once the PR merged, latest version of HELM chart should be available in couple of mins in https://microsoft.github.io/charts/repo and https://artifacthub.io/.

Instructions to create PR
```
# 1. create helm package for the release candidate 
   git clone git@github.com:microsoft/Docker-Provider.git
   git checkout ci_prod
   cd ~/Docker-Provider/charts/azuremonitor-containers # this path based on where you have cloned the repo
   helm package . 

# 2. clone your fork repo and checkout gh_pages branch # gh_pages branch used as release branch 
   cd ~ 
   git clone <your-forked-repo-of-microsoft-charts-repo>
   cd  ~/charts # assumed the root dir of the clone is charts
   git checkout gh_pages

# 3. copy release candidate helm package 
   cd ~/charts/repo/azuremonitor-containers 
   # update chart version value with the version of chart being released
   cp ~/Docker-Provider/charts/azuremonitor-containers/azuremonitor-containers-<chart-version>.tgz  .  
   cd ~/charts/repo
   # update repo index file 
   helm repo index  .
    
# 4. Review the changes and make PR. Please note, you may need to revert unrelated changes automatically added by `helm repo index .` command

```

# 4. Monitor agent roll-out status

In Container Insights Agent (AKS) telemetry dashboard, update the agent roll status by  region chart with released agent image and track rollout status. If you see any issues with agent rollout, reach out AKS on-call team for the help on investigation and understanding whats going on.




# Problems:
- ciprod-rc-aks16-weu - naming makes no sense. we should rename that cluster. also it says aks16 so k8s version 16 is deprecated. i remember i upgraded it to latest when i deployed in jan
- Deployment to scale cluster is broken, cluster API server is unresponsive (kubectl can't communicate)
- deployment pipeline doesn't clean up old scale clusters, two are still running since March release (that's an expensive thing to not clean up)
- Permission errors when updating the variable group in the deployment pipeline. (Vishwa can't edit the pipeline variables)
- Windows build fails on finalize step. (TODO: check cdpx email chain later for updates/fixes)

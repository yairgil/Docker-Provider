# Onboard to Azure Monitor for containers Health(Tab) limited preview

For on-boarding to Health(Tab), you would need to follow two steps
1. Configure agent through configmap to collect health data. [Learn more about ConfigMap](https://docs.microsoft.com/azure/azure-monitor/insights/container-insights-agent-config#configmap-file-settings-overview)
2. Access `Health tab` with feature flag to view health tab. [aka.ms/HealthPreview](https://aka.ms/Healthpreview)


## Configure agent through ConfigMap
1. Include the following section in ConfigMap yaml file
```cmd:agent-settings: |-
    [agent_settings.health_model]
      enabled = true
```
2. Run the following kubectl command:
   `kubectl apply -f <configmap_yaml_file.yaml>`
   
Example: `kubectl apply -f container-azm-ms-agentconfig.yaml`.

The configuration change can take a few minutes to finish before taking effect, and all omsagent pods in the cluster will restart. The restart is a rolling restart for all omsagent pods, not all restart at the same time. When the restarts are finished, a message is displayed that's similar to the following and includes the result: `configmap "container-azm-ms-agentconfig"` created.


## Access health(tab) in Azure Monitor for containers Insights experience
1. You can access Health(tab) by accessing protal through this link. [aka.ms/HealthPreview](https://aka.ms/Healthpreview). This link includes feature flag to access Health(tab).


For any question please reachout to us at [askcoin@microsoft.com](mailto:askcoin@microsoft.com)


AZURE_PUBLIC_CLOUD_ENDPOINTS = {
    "activeDirectory": "https://login.microsoftonline.com/",
    "activeDirectoryDataLakeResourceId": "https://datalake.azure.net/",
    "activeDirectoryGraphResourceId": "https://graph.windows.net/",
    "activeDirectoryResourceId": "https://management.core.windows.net/",
    "appInsightsResourceId": "https://api.applicationinsights.io",
    "appInsightsTelemetryChannelResourceId": "https://dc.applicationinsights.azure.com/v2/track",
    "batchResourceId": "https://batch.core.windows.net/",
    "gallery": "https://gallery.azure.com/",
    "logAnalyticsResourceId": "https://api.loganalytics.io",
    "management": "https://management.core.windows.net/",
    "mediaResourceId": "https://rest.media.azure.net",
    "microsoftGraphResourceId": "https://graph.microsoft.com/",
    "ossrdbmsResourceId": "https://ossrdbms-aad.database.windows.net",
    "resourceManager": "https://management.azure.com/",
    "sqlManagement": "https://management.core.windows.net:8443/",
    "vmImageAliasDoc": "https://raw.githubusercontent.com/Azure/azure-rest-api-specs/master/arm-compute/quickstart-templates/aliases.json"
}

AZURE_DOGFOOD_ENDPOINTS = {
    "activeDirectory": "https://login.windows-ppe.net/",
    "activeDirectoryDataLakeResourceId": None,
    "activeDirectoryGraphResourceId": "https://graph.ppe.windows.net/",
    "activeDirectoryResourceId": "https://management.core.windows.net/",
    "appInsightsResourceId": None,
    "appInsightsTelemetryChannelResourceId": None,
    "batchResourceId": None,
    "gallery": "https://df.gallery.azure-test.net/",
    "logAnalyticsResourceId": None,
    "management": "https://management-preview.core.windows-int.net/",
    "mediaResourceId": None,
    "microsoftGraphResourceId": None,
    "ossrdbmsResourceId": None,
    "resourceManager": "https://api-dogfood.resources.windows-int.net/",
    "sqlManagement": None,
    "vmImageAliasDoc": None
}

AZURE_CLOUD_DICT = {"AZURE_PUBLIC_CLOUD" : AZURE_PUBLIC_CLOUD_ENDPOINTS, "AZURE_DOGFOOD": AZURE_DOGFOOD_ENDPOINTS}

TIMEOUT = 300

# Azure Monitor for Container Extension related
AGENT_RESOURCES_NAMESPACE = 'kube-system'
AGENT_DEPLOYMENT_NAME = 'omsagent-rs'
AGENT_DAEMONSET_NAME = 'omsagent'
AGENT_WIN_DAEMONSET_NAME = 'omsagent-win'

AGENT_DEPLOYMENT_PODS_LABEL_SELECTOR = 'rsName=omsagent-rs'
AGENT_DAEMON_SET_PODS_LABEL_SELECTOR = 'component=oms-agent'
AGENT_OMSAGENT_LOG_PATH = '/var/opt/microsoft/omsagent/log/omsagent.log'
AGENT_REPLICASET_WORKFLOWS = ["kubePodInventoryEmitStreamSuccess", "kubeNodeInventoryEmitStreamSuccess"]

# replicaset workflow streams
POD_INVENTORY_EMIT_STREAM = "kubePodInventoryEmitStreamSuccess"
NODE_INVENTORY_EMIT_STREAM = "kubeNodeInventoryEmitStreamSuccess"
DEPLOYMENT_INVENTORY_EMIT_STREAM = "kubestatedeploymentsInsightsMetricsEmitStreamSuccess"

# daemosnet workflow streams


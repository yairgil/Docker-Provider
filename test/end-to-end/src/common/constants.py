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

DEFAULT_RELEASE_TRAIN = 'stable'
HELM_CHART_PATH = 'azure-arc-k8sagents'
HELM_RELEASE_NAME = 'azure-arc'
HELM_RELEASE_NAMESPACE = 'default'
KUBERNETES_DISTRIBUTION = 'generic'
KUBERNETES_INFRASTRUCTURE = 'generic'
ARC_HELM_CHART_PARAMS_DICT = {}
TIMEOUT = 300

AZURE_ARC_NAMESPACE = 'azure-arc'

CLUSTER_METADATA_CRD_GROUP = 'arc.azure.com'
CLUSTER_METADATA_CRD_VERSION = 'v1beta1'
CLUSTER_METADATA_CRD_PLURAL = 'connectedclusters'
CLUSTER_METADATA_CRD_NAME = 'clustermetadata'
CLUSTER_METADATA_DICT = {'kubernetes_version': 0, 'total_node_count': 0, 'agent_version': 0}

METRICS_AGENT_LOG_LIST = ["Successfully connected to outputs.http_mdm", "Wrote batch of"]
# To be updated after a bug fix. ["Could not resolve", "Could not parse"]
METRICS_AGENT_ERROR_LOG_LIST = ["Could not parse"]
FLUENT_BIT_LOG_LIST = ["[engine] started (pid=1)", "[sp] stream processor started", "[http_mdm] Flush called for id: http_mdm_plugin"]
FLUENT_BIT_ERROR_LOG_LIST = ["[error] [in_tail] read error, check permissions"]
METRICS_AGENT_CONTAINER_NAME = 'metrics-agent'
FLUENT_BIT_CONTAINER_NAME = 'fluent-bit'

CLUSTER_TYPE = 'connectedClusters'
CLUSTER_RP = 'Microsoft.Kubernetes'
OPERATOR_TYPE = 'flux'
HELM_OPERATOR_VERSION = '0.6.0'
REPOSITORY_URL_HOP = 'https://github.com/Azure/arc-helm-demo.git'
CONFIGURATION_NAME_HOP = 'azure-arc-sample'
OPERATOR_SCOPE_HOP = 'cluster'
OPERATOR_NAMESPACE_HOP = 'arc-k8s-demo'
OPERATOR_NAMESPACE_HOP_DEFAULT = 'default'
OPERATOR_INSTANCE_NAME_HOP = 'azure-arc-sample'
OPERATOR_PARAMS_HOP = '--git-readonly --git-path=releases --registry-disable-scanning'
HELM_OPERATOR_PARAMS_HOP = '--set helm.versions=v3'
HELM_OPERATOR_POD_LABEL_LIST = ['arc-k8s-demo', 'helm-operator', 'azure-arc-sample']

REPOSITORY_URL_FOP = 'https://github.com/Azure/arc-k8s-demo.git'
CONFIGURATION_NAME_FOP = 'cluster-config'
OPERATOR_SCOPE_FOP = 'cluster'
OPERATOR_NAMESPACE_FOP = 'cluster-config'
OPERATOR_NAMESPACE_FOP_DEFAULT = 'default'
OPERATOR_INSTANCE_NAME_FOP = 'cluster-config'
OPERATOR_PARAMS_FOP = '--git-readonly --registry-disable-scanning'
FLUX_OPERATOR_POD_LABEL_LIST = ['cluster-config']
FLUX_OPERATOR_RESOURCES_POD_LABEL_LIST = ['azure-vote-back', 'azure-vote-front']
FLUX_OPERATOR_RESOURCE_NAMESPACE = 'default'
FLUX_OPERATOR_NAMESPACE_RESOURCE_LIST = ['team-a', 'team-b', 'itops']

AZURE_IDENTITY_CERTIFICATE_SECRET = 'azure-identity-certificate'
AZURE_IDENTITY_TOKEN_SECRET = 'identity-request-2a051a512c1afcd426dd4090206c017a675c0f002bf329cc3165a7ba3abdcc97-token'
ARC_CONFIG_NAME = 'azure-clusterconfig'
CLUSTER_IDENTITY_CRD_GROUP = 'clusterconfig.azure.com'
CLUSTER_IDENTITY_CRD_VERSION = 'v1beta1'
CLUSTER_IDENTITY_CRD_PLURAL = 'azureclusteridentityrequests'
CLUSTER_IDENTITY_CRD_NAME = 'identity-request-2a051a512c1afcd426dd4090206c017a675c0f002bf329cc3165a7ba3abdcc97'
IDENTITY_TOKEN_REFERENCE_DICTIONARY = {'dataName': 'cluster-identity-token', 'secretName': 'identity-request-2a051a512c1afcd426dd4090206c017a675c0f002bf329cc3165a7ba3abdcc97-token'}

CLEANUP_NAMESPACE_LIST = ['cluster-config', 'arc-k8s-demo', 'team-a', 'team-b', 'itops']
CLEANUP_DEPLOYMENT_LIST = ['azure-vote-back', 'azure-vote-front']
CLEANUP_SERVICE_LIST = ['azure-vote-back', 'azure-vote-front']

# Azure Monitor for Container Extension related
AZMON_CI_EXTENSION = False
AZMON_CI_EXTENSION_HELM_RELEASE_NAME  = "azuremonitor-containers"
AZMON_CI_EXTENSION_HELM_RELEASE_NAMESPACE = "default"
AZMON_CI_EXTENSION_HELM_REPO_PATH = "mcr.microsoft.com/azuremonitor/containerinsights/canary/preview/azuremonitor-containers"
AZMON_CI_EXTENSION_HELM_CHART_VERSION = "2.8.6"
AZMON_CI_EXTENSION_LOG_ANALYTICS_DOMAIN = "opinsights.azure.com"
AZMON_CI_EXTENSION_HELM_CHART_PATH = 'azuremonitor-containers'
AZMON_CI_EXTENSION_RESOURCES_NAMESPACE = 'kube-system'
AZMON_CI_EXTENSION_DEPLOYMENT_LABEL_LIST = ['omsagent']
AZMON_CI_EXTENSION_DAEMONSET_LABEL_LIST = ['omsagent']
AZMON_CI_EXTENSION_DEPLOYMENT_POD_LABEL_LIST = ['omsagent-rs']
AZMON_CI_EXTENSION_DAEMONSET_POD_LABEL_LIST = ['omsagent-ds']


AZMON_CI_EXTENSION_CLUSTER_IDENTITY_CRD_GROUP = 'clusterconfig.azure.com'
AZMON_CI_EXTENSION_CLUSTER_IDENTITY_CRD_VERSION = 'v1beta1'
AZMON_CI_EXTENSION_CLUSTER_IDENTITY_CRD_PLURAL = 'azureclusteridentityrequests'
AZMON_CI_EXTENSION_CLUSTER_IDENTITY_CRD_NAME = 'container-insights-clusteridentityrequest'


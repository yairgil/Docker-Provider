import pytest
import os
import time
import pickle

import constants
from helper import check_kubernetes_secret, get_helm_registry

from filelock import FileLock
from pathlib import Path
from kubernetes import client, config
from kubernetes_namespace_utility import list_namespace, delete_namespace
from kubernetes_pod_utility import get_pod_list, get_pod_logs
from kubernetes_deployment_utility import list_deployment, delete_deployment
from kubernetes_service_utility import list_service, delete_service
from results_utility import create_results_dir, append_result_output
from arm_rest_utility import fetch_aad_token, fetch_aad_token_credentials
from connected_cluster_utility import get_connected_cluster_client, delete_connected_cluster
from helm_utility import pull_helm_chart, export_helm_chart, add_helm_repo, install_helm_chart, delete_helm_release, list_helm_release

pytestmark = pytest.mark.arcagentstest

# Fixture to collect all the environment variables, install the helm charts and check the status of azure arc pods. It will be run before the tests.
@pytest.fixture(scope='session', autouse=True)
def env_dict():
    my_file = Path("env.pkl")  # File to store the environment variables.
    with FileLock(str(my_file) + ".lock"):  # Locking the file since each test will be run in parallel as separate subprocesses and may try to access the file simultaneously.
        env_dict = {}
        if not my_file.is_file():
            # Creating the results directory
            create_results_dir('/tmp/results')

            # Setting some environment variables
            env_dict['SETUP_LOG_FILE'] = '/tmp/results/setup'
            env_dict['TEST_CONNECTED_CLUSTER_LOG_FILE'] = '/tmp/results/testcc'
            env_dict['TEST_CONNECTED_CLUSTER_METADATA_LOG_FILE'] = '/tmp/results/testccmetadata'
            env_dict['TEST_IDENTITY_OPERATOR_LOG_FILE'] = '/tmp/results/identityop'
            env_dict['TEST_METRICS_AND_LOGGING_AGENT_LOG_FILE'] = '/tmp/results/metricsandlogs'
            env_dict['TEST_KUBERNETES_CONFIG_FOP_LOG_FILE'] = '/tmp/results/k8sconfigfop'
            env_dict['TEST_KUBERNETES_CONFIG_HOP_LOG_FILE'] = '/tmp/results/k8sconfighop'
            env_dict['TEST_CONTAINER_INSIGHTS_LOG_FILE'] = '/tmp/results/containerinsights'
            env_dict['NUM_TESTS_COMPLETED'] = 0

            # Collecting environment variables
            env_dict['TENANT_ID'] = os.getenv('TENANT_ID')
            env_dict['SUBSCRIPTION_ID'] = os.getenv('SUBSCRIPTION_ID')
            env_dict['RESOURCE_GROUP'] = os.getenv('RESOURCE_GROUP')
            env_dict['CLUSTER_NAME'] = os.getenv('CLUSTER_NAME')
            env_dict['LOCATION'] = os.getenv('LOCATION')
            env_dict['CLIENT_ID'] = os.getenv('CLIENT_ID')
            env_dict['CLIENT_SECRET'] = os.getenv('CLIENT_SECRET')

            # Collecting extension specific environment variables 
            env_dict['AZMON_CI_EXTENSION'] =  os.getenv('AZMON_CI_EXTENSION') if os.getenv('AZMON_CI_EXTENSION') else constants.AZMON_CI_EXTENSION
            env_dict['AZMON_CI_EXTENSION_HELM_RELEASE_NAME'] = constants.AZMON_CI_EXTENSION_HELM_RELEASE_NAME
            env_dict['AZMON_CI_EXTENSION_HELM_RELEASE_NAMESPACE'] = constants.AZMON_CI_EXTENSION_HELM_RELEASE_NAMESPACE
            # domain can be different for the different azure cloud type and default is public cloud
            env_dict['AZMON_CI_EXTENSION_LOG_ANALYTICS_DOMAIN'] = os.getenv('AZMON_CI_EXTENSION_LOG_ANALYTICS_DOMAIN') if os.getenv('AZMON_CI_EXTENSION_LOG_ANALYTICS_DOMAIN') else constants.AZMON_CI_EXTENSION_LOG_ANALYTICS_DOMAIN 

            env_dict['AZMON_CI_EXTENSION_HELM_CHART_PATH'] = os.getenv('AZMON_CI_EXTENSION_HELM_CHART_PATH') if os.getenv('AZMON_CI_EXTENSION_HELM_CHART_PATH') else constants.AZMON_CI_EXTENSION_HELM_CHART_PATH
            env_dict['AZMON_CI_EXTENSION_LOG_ANALYTICS_WORKSPACE_ID'] = os.getenv('AZMON_CI_EXTENSION_LOG_ANALYTICS_WORKSPACE_ID')
            env_dict['AZMON_CI_EXTENSION_LOG_ANALYTICS_WORKSPACE_KEY'] = os.getenv('AZMON_CI_EXTENSION_LOG_ANALYTICS_WORKSPACE_KEY')
            if env_dict['AZMON_CI_EXTENSION']:
                if not env_dict['AZMON_CI_EXTENSION_LOG_ANALYTICS_WORKSPACE_ID']:
                    pytest.fail('ERROR: variable AZMON_CI_EXTENSION_LOG_ANALYTICS_WORKSPACE_ID is required.')
                if not env_dict['AZMON_CI_EXTENSION_LOG_ANALYTICS_WORKSPACE_KEY']:
                    pytest.fail('ERROR: variable AZMON_CI_EXTENSION_LOG_ANALYTICS_WORKSPACE_KEY is required.')

            env_dict['AZURE_ENDPOINTS'] = constants.AZURE_CLOUD_DICT.get(os.getenv('AZURE_CLOUD')) if os.getenv('AZURE_CLOUD') else constants.AZURE_PUBLIC_CLOUD_ENDPOINTS

            env_dict['RELEASE_TRAIN'] = os.getenv('RELEASE_TRAIN') if os.getenv('RELEASE_TRAIN') else constants.DEFAULT_RELEASE_TRAIN
            env_dict['HELM_REGISTRY_PATH'] = os.getenv('HELM_REGISTRY_PATH')
            env_dict['HELM_REPO_NAME'] = os.getenv('HELM_REPO_NAME')
            env_dict['HELM_REPO_URL'] = os.getenv('HELM_REPO_URL')
            env_dict['HELM_CHART_PATH'] = os.getenv('HELM_CHART_PATH') if os.getenv('HELM_CHART_PATH') else constants.HELM_CHART_PATH
            env_dict['HELM_RELEASE_NAME'] = os.getenv('HELM_RELEASE_NAME') if os.getenv('HELM_RELEASE_NAME') else constants.HELM_RELEASE_NAME
            env_dict['HELM_RELEASE_NAMESPACE'] = os.getenv('HELM_RELEASE_NAMESPACE') if os.getenv('HELM_RELEASE_NAMESPACE') else constants.HELM_RELEASE_NAMESPACE
            env_dict['KUBERNETES_DISTRIBUTION'] = os.getenv('KUBERNETES_DISTRIBUTION') if os.getenv('KUBERNETES_DISTRIBUTION') else constants.KUBERNETES_DISTRIBUTION
            env_dict['KUBERNETES_INFRASTRUCTURE'] = os.getenv('KUBERNETES_INFRASTRUCTURE') if os.getenv('KUBERNETES_INFRASTRUCTURE') else constants.KUBERNETES_INFRASTRUCTURE
            env_dict['HELM_CHART_PARAMETERS'] = os.getenv('HELM_CHART_PARAMETERS')
            env_dict['TIMEOUT'] = int(os.getenv('TIMEOUT')) if os.getenv('TIMEOUT') else constants.TIMEOUT

            env_dict['CLUSTER_METADATA_FIELDS'] = os.getenv('CLUSTER_METADATA_FIELDS')

            env_dict['METRICS_AGENT_LOG_LIST'] = os.getenv('METRICS_AGENT_LOG_LIST')
            env_dict['FLUENT_BIT_LOG_LIST'] = os.getenv('FLUENT_BIT_LOG_LIST')

            env_dict['CLUSTER_TYPE'] = os.getenv('CLUSTER_TYPE')
            env_dict['CLUSTER_RP'] = os.getenv('CLUSTER_RP')
            env_dict['CONFIGURATION_NAME_HOP'] = os.getenv('CONFIGURATION_NAME_HOP')
            env_dict['REPOSITORY_URL_HOP'] = os.getenv('REPOSITORY_URL_HOP')
            env_dict['OPERATOR_SCOPE_HOP'] = os.getenv('OPERATOR_SCOPE_HOP')
            env_dict['OPERATOR_NAMESPACE_HOP'] = os.getenv('OPERATOR_NAMESPACE_HOP')
            env_dict['OPERATOR_INSTANCE_NAME_HOP'] = os.getenv('OPERATOR_INSTANCE_NAME_HOP')
            env_dict['OPERATOR_TYPE_HOP'] = os.getenv('OPERATOR_TYPE_HOP')
            env_dict['OPERATOR_PARAMS_HOP'] = os.getenv('OPERATOR_PARAMS_HOP')
            env_dict['HELM_OPERATOR_VERSION_HOP'] = os.getenv('HELM_OPERATOR_VERSION_HOP')
            env_dict['HELM_OPERATOR_PARAMS_HOP'] = os.getenv('HELM_OPERATOR_PARAMS_HOP')

            env_dict['CONFIGURATION_NAME_FOP'] = os.getenv('CONFIGURATION_NAME_FOP')
            env_dict['REPOSITORY_URL_FOP'] = os.getenv('REPOSITORY_URL_FOP')
            env_dict['OPERATOR_SCOPE_FOP'] = os.getenv('OPERATOR_SCOPE_FOP')
            env_dict['OPERATOR_NAMESPACE_FOP'] = os.getenv('OPERATOR_NAMESPACE_FOP')
            env_dict['OPERATOR_INSTANCE_NAME_FOP'] = os.getenv('OPERATOR_INSTANCE_NAME_FOP')
            env_dict['OPERATOR_TYPE_FOP'] = os.getenv('OPERATOR_TYPE_FOP')
            env_dict['OPERATOR_PARAMS_FOP'] = os.getenv('OPERATOR_PARAMS_FOP')

            print("Starting setup...")
            append_result_output("Starting setup...\n", env_dict['SETUP_LOG_FILE'])

            tenant_id = env_dict.get('TENANT_ID')
            if not tenant_id:
                pytest.fail('ERROR: variable TENANT_ID is required.')

            subscription_id = env_dict.get('SUBSCRIPTION_ID')
            if not subscription_id:
                pytest.fail('ERROR: variable SUBSCRIPTION_ID is required.')

            resource_group = env_dict.get('RESOURCE_GROUP')
            if not resource_group:
                pytest.fail('ERROR: variable RESOURCE_GROUP is required.')

            cluster_name = env_dict.get('CLUSTER_NAME')
            if not cluster_name:
                pytest.fail('ERROR: variable CLUSTER_NAME is required.')

            location = env_dict.get('LOCATION')
            if not location:
                pytest.fail('ERROR: variable LOCATION is required.')

            client_id = env_dict.get('CLIENT_ID')
            if not client_id:
                pytest.fail('ERROR: variable CLIENT_ID is required.')

            client_secret = env_dict.get('CLIENT_SECRET')
            if not client_secret:
                pytest.fail('ERROR: variable CLIENT_SECRET is required.')
            
            # Get aad token
            authority_uri = env_dict.get('AZURE_ENDPOINTS').get('activeDirectory') + tenant_id
            token = fetch_aad_token(client_id, client_secret, authority_uri, env_dict.get('AZURE_ENDPOINTS').get('management'))
            access_token = token.get('accessToken')

            # Fetch helm chart path
            release_train = env_dict.get('RELEASE_TRAIN')
            helm_registry_path = env_dict.get('HELM_REGISTRY_PATH') if env_dict.get('HELM_REGISTRY_PATH') else get_helm_registry(access_token, location, release_train)  # This env var can be used to install helm charts from a custom registry.
            print("Successfully fetched helm chart path.")

            # Pulling helm charts
            result = pull_helm_chart(helm_registry_path)
            append_result_output("Pulled helm chart: {}\n".format(result), env_dict['SETUP_LOG_FILE'])
            print("Successfully pulled helm charts.")

            # Exporting helm charts
            result = export_helm_chart(helm_registry_path, ".")
            append_result_output("Exported helm chart: {}\n".format(result), env_dict['SETUP_LOG_FILE'])
            print("Successfully exported helm charts.")

            # Adding a helm repository
            if env_dict.get('HELM_REPO_NAME') and env_dict.get('HELM_REPO_URL'):  # These parameters should be provided if the user wishes to use a helm repository instead of a helm registry.
                helm_repo_name = env_dict.get('HELM_REPO_NAME')
                helm_repo_url = env_dict.get('HELM_REPO_URL')
                result = add_helm_repo(helm_repo_name, helm_repo_url)
                append_result_output("Added helm repository: {}\n".format(result), env_dict['SETUP_LOG_FILE'])
                print("Successfully added helm repository.")
            
            # Fetch CI helm chart if AZMON_CI_EXTENSION enabled
            if env_dict['AZMON_CI_EXTENSION']:
                ci_helm_repo_path = env_dict.get('AZMON_CI_EXTENSION_HELM_REPO_PATH') if os.getenv('AZMON_CI_EXTENSION_HELM_REPO_PATH') else constants.AZMON_CI_EXTENSION_HELM_REPO_PATH 
                ci_helm_chart_version = env_dict.get('AZMON_CI_EXTENSION_HELM_CHART_VERSION') if os.getenv('AZMON_CI_EXTENSION_HELM_CHART_VERSION') else constants.AZMON_CI_EXTENSION_HELM_CHART_VERSION 
                ci_helm_registry_path = ci_helm_repo_path + ":" + ci_helm_chart_version                            
                # Pulling CI helm charts
                result = pull_helm_chart(ci_helm_registry_path)
                append_result_output("Pulled container insights helm chart: {}\n".format(result), env_dict['SETUP_LOG_FILE'])
                print("Successfully pulled container insights helm charts.")
                # Exporting helm charts
                result = export_helm_chart(ci_helm_registry_path, ".")
                append_result_output("Exported container insights helm chart: {}\n".format(result), env_dict['SETUP_LOG_FILE'])
                print("Successfully exported container insights helm charts.")

            # Loading in-cluster kube-config
            try:
                config.load_incluster_config()
            except Exception as e:
                pytest.fail("Error loading the in-cluster config: " + str(e))

            # Installing helm charts
            helm_chart_path = env_dict.get('HELM_CHART_PATH')  # This parameter might be used to onboard using charts present locally on the system. And it would be required to copy these charts in dockerfile and rebuild. 
            helm_release_name = env_dict.get('HELM_RELEASE_NAME')
            helm_release_namespace = env_dict.get('HELM_RELEASE_NAMESPACE')
            kubernetes_distribution = env_dict.get('KUBERNETES_DISTRIBUTION')
            kubernetes_infrastructure = env_dict.get('KUBERNETES_INFRASTRUCTURE')

            helm_chart_params_dict = {}  # Dictionary to store the helm chart parameters as key value pairs.
            helm_chart_params_dict['global.subscriptionId'] = subscription_id
            helm_chart_params_dict['global.resourceGroupName'] = resource_group 
            helm_chart_params_dict['global.resourceName'] = cluster_name
            helm_chart_params_dict['global.location'] = location
            helm_chart_params_dict['global.tenantId'] = tenant_id
            helm_chart_params_dict['global.clientId'] = client_id
            helm_chart_params_dict['global.clientSecret'] = client_secret
            helm_chart_params_dict['global.kubernetesDistro'] = kubernetes_distribution
            helm_chart_params_dict['global.kubernetesInfra'] = kubernetes_infrastructure
            if env_dict.get('HELM_CHART_PARAMETERS'):  # This environment variable should be provided as comma separated key value pairs. It will append/overwrite the helm chart parameters while installing helm release.
                helm_chart_params_list = env_dict.get('HELM_CHART_PARAMETERS').split(',')
                for params in helm_chart_params_list:
                    param = params.split('=')
                    param_key = param[0].strip()
                    param_value = param[1].strip()
                    helm_chart_params_dict[param_key] = param_value

            result = install_helm_chart(helm_release_name, helm_release_namespace, helm_chart_path, True, **helm_chart_params_dict)
            append_result_output("Installed Helm Chart: {}\n".format(result), env_dict['SETUP_LOG_FILE'])
            print("Successfully installed helm charts.")

            # Installing ci helm charts
            if env_dict['AZMON_CI_EXTENSION']:
                AZMON_CI_EXTENSION_HELM_CHART_PATH = env_dict.get('AZMON_CI_EXTENSION_HELM_CHART_PATH')  # This parameter might be used to onboard using charts present locally on the system. And it would be required to copy these charts in dockerfile and rebuild. 
                ci_helm_release_name = env_dict.get('AZMON_CI_EXTENSION_HELM_RELEASE_NAME')
                ci_helm_release_namespace = env_dict.get('AZMON_CI_EXTENSION_HELM_RELEASE_NAMESPACE')            
            
                ci_helm_chart_params_dict = {}  # Dictionary to store the helm chart parameters as key value pairs.            
                cluster_resource_id = "/subscriptions/{}/resourceGroups/{}/providers/Microsoft.Kubernetes/connectedClusters/{}".format(subscription_id, resource_group, cluster_name)
                ci_helm_chart_params_dict["Azure.Cluster.ResourceId"] = cluster_resource_id
                ci_helm_chart_params_dict["Azure.Cluster.Region"] = location
                log_analytics_workspace_domain = env_dict['AZMON_CI_EXTENSION_LOG_ANALYTICS_DOMAIN']           
                ci_helm_chart_params_dict["omsagent.domain"] = log_analytics_workspace_domain
                log_analytics_workspace_id = env_dict['AZMON_CI_EXTENSION_LOG_ANALYTICS_WORKSPACE_ID'] 
                ci_helm_chart_params_dict["omsagent.secret.wsid"] = log_analytics_workspace_id
                log_analytics_workspace_key = env_dict['AZMON_CI_EXTENSION_LOG_ANALYTICS_WORKSPACE_KEY'] 
                ci_helm_chart_params_dict["omsagent.secret.key"] = log_analytics_workspace_key
    
                if env_dict.get('CI_HELM_CHART_PARAMETERS'):  # This environment variable should be provided as comma separated key value pairs. It will append/overwrite the helm chart parameters while installing helm release.
                    helm_chart_params_list = env_dict.get('CI_HELM_CHART_PARAMETERS').split(',')
                    for params in helm_chart_params_list:
                        param = params.split('=')
                        param_key = param[0].strip()
                        param_value = param[1].strip()
                        helm_chart_params_dict[param_key] = param_value

                result = install_helm_chart(ci_helm_release_name, ci_helm_release_namespace, AZMON_CI_EXTENSION_HELM_CHART_PATH, True, **ci_helm_chart_params_dict)
                append_result_output("Installed CI Helm Chart: {}\n".format(result), env_dict['SETUP_LOG_FILE'])
                print("Successfully installed CI helm chart.")

            print("Setup Complete.")
            append_result_output("Setup Complete.\n", env_dict['SETUP_LOG_FILE'])

            with Path.open(my_file, "wb") as f:
                pickle.dump(env_dict, f, pickle.HIGHEST_PROTOCOL)
        else:
            with Path.open(my_file, "rb") as f:
                env_dict = pickle.load(f)
        
    yield env_dict
    
    my_file = Path("env.pkl")
    with FileLock(str(my_file) + ".lock"):
        with Path.open(my_file, "rb") as f:
            env_dict = pickle.load(f)

        env_dict['NUM_TESTS_COMPLETED'] = 1 + env_dict.get('NUM_TESTS_COMPLETED')
        if env_dict['NUM_TESTS_COMPLETED'] == int(os.getenv('NUM_TESTS')):

            # Collecting all the azure-arc pod logs.
            print('Collecting the azure-arc pod logs')
            try:
                config.load_incluster_config()
            except Exception as e:
                pytest.fail("Error loading the in-cluster config: " + str(e))
            
            api_instance = client.CoreV1Api()
            pod_list = get_pod_list(api_instance, constants.AZURE_ARC_NAMESPACE)
            for pod in pod_list.items:
                pod_name = pod.metadata.name
                for container in pod.spec.containers:
                    container_name = container.name
                    log = get_pod_logs(api_instance, constants.AZURE_ARC_NAMESPACE, pod_name, container_name)
                    append_result_output("Logs for the pod {} and container {}:\n".format(pod_name, container_name), "/tmp/results/{}-{}".format(pod_name, container_name))
                    append_result_output("{}\n".format(log), "/tmp/results/{}-{}".format(pod_name, container_name))

            # Checking if cleanup is required.
            if os.getenv('SKIP_CLEANUP'):
                return
            print('Starting cleanup...')
            append_result_output("Starting Cleanup...\n", env_dict['SETUP_LOG_FILE'])

            # Deleting extension resource and extension resource has to be deleted first since this has dependency on cluster resource
            # Cleaning up resources created by Container Insighst Extension  
            # ci extension related
            AZMON_CI_EXTENSION_helm_release_name = env_dict.get('AZMON_CI_EXTENSION_HELM_RELEASE_NAME')
            AZMON_CI_EXTENSION_helm_release_namespace = env_dict.get('AZMON_CI_EXTENSION_HELM_RELEASE_NAMESPACE')          
            if AZMON_CI_EXTENSION_helm_release_name in list_helm_release(AZMON_CI_EXTENSION_helm_release_namespace):
                print('Deleting the container insights helm chart')
                delete_helm_release(AZMON_CI_EXTENSION_helm_release_name, AZMON_CI_EXTENSION_helm_release_namespace)

            # Deleting the connected cluster resource
            print('Deleting the connected cluster resource')

            tenant_id = env_dict.get('TENANT_ID')
            subscription_id = env_dict.get('SUBSCRIPTION_ID')
            resource_group = env_dict.get('RESOURCE_GROUP')
            cluster_name = env_dict.get('CLUSTER_NAME')
            location = env_dict.get('LOCATION')
            client_id = env_dict.get('CLIENT_ID')
            client_secret = env_dict.get('CLIENT_SECRET')
            helm_release_name = env_dict.get('HELM_RELEASE_NAME')
            helm_release_namespace = env_dict.get('HELM_RELEASE_NAMESPACE')
          
            # Fetch aad token credentials from spn
            authority_uri = env_dict.get('AZURE_ENDPOINTS').get('activeDirectory') + tenant_id
            credential = fetch_aad_token_credentials(client_id, client_secret, authority_uri, env_dict.get('AZURE_ENDPOINTS').get('management'))
            print("Successfully fetched credentials object.")

            cc_client = get_connected_cluster_client(credential, subscription_id)
            delete_connected_cluster(cc_client, resource_group, cluster_name)

            # Deleting the helm release
            print("Cleaning up the azure arc helm release.")
            
            if helm_release_name in list_helm_release(helm_release_namespace):
                delete_helm_release(helm_release_name, helm_release_namespace)
            
          
            # Cleaning up resources created by default configurations
            print("Cleaning up the resources create by the flux operators")
            cleanup_namespace_list = constants.CLEANUP_NAMESPACE_LIST
            namespace_list = list_namespace(api_instance)
            for ns in namespace_list.items:
                namespace_name = ns.metadata.name
                if namespace_name in cleanup_namespace_list:
                    delete_namespace(api_instance, namespace_name)
            
            api_instance = client.AppsV1Api()
            cleanup_deployment_list = constants.CLEANUP_DEPLOYMENT_LIST
            deployment_list = list_deployment(api_instance, constants.FLUX_OPERATOR_RESOURCE_NAMESPACE)
            for deployment in deployment_list.items:
                deployment_name = deployment.metadata.name
                if deployment_name in cleanup_deployment_list:
                    delete_deployment(api_instance, constants.FLUX_OPERATOR_RESOURCE_NAMESPACE, deployment_name)

            api_instance = client.CoreV1Api()
            cleanup_service_list = constants.CLEANUP_SERVICE_LIST
            service_list = list_service(api_instance, constants.FLUX_OPERATOR_RESOURCE_NAMESPACE)
            for service in service_list.items:
                service_name = service.metadata.name
                if service_name in cleanup_service_list:
                    delete_service(api_instance, constants.FLUX_OPERATOR_RESOURCE_NAMESPACE, service_name)
            print("Cleanup Complete.")
            append_result_output("Cleanup Complete.\n", env_dict['SETUP_LOG_FILE'])
            return

        with Path.open(my_file, "wb") as f:
            pickle.dump(env_dict, f, pickle.HIGHEST_PROTOCOL)

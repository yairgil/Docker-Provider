import pytest
import constants

from kubernetes import client, config
from kubernetes_pod_utility import get_pod_list
from results_utility import append_result_output
from helper import check_kubernetes_pod_logs
from helper import check_kubernetes_pods_status, check_kubernetes_configuration_state, check_namespace_status
from helper import check_kubernetes_daemonset_status, check_kubernetes_deployments_status
from helper import check_kubernetes_crd_status

pytestmark = pytest.mark.arcagentstest


def test_resource_status(env_dict):
    print("Starting container insights extension check.")

    # if not env_dict.get('AZMON_CI_EXTENSION'):
    #    print("container insights extension test not included.")
    #    return True

    # Loading in-cluster kube-config
    try:
        config.load_incluster_config()
        #config.load_kube_config()
    except Exception as e:
        pytest.fail("Error loading the in-cluster config: " + str(e))

    timeout_seconds = env_dict.get('TIMEOUT')

    # checking the deployment status
    check_kubernetes_deployments_status(constants.AZMON_CI_EXTENSION_RESOURCES_NAMESPACE,
                                        env_dict['TEST_CONTAINER_INSIGHTS_LOG_FILE'], constants.AZMON_CI_EXTENSION_DEPLOYMENT_LABEL_LIST, timeout_seconds)

    # checking the daemonset status
    check_kubernetes_daemonset_status(constants.AZMON_CI_EXTENSION_RESOURCES_NAMESPACE,
                                      env_dict['TEST_CONTAINER_INSIGHTS_LOG_FILE'], constants.AZMON_CI_EXTENSION_DAEMONSET_LABEL_LIST, timeout_seconds)

    # Checking the status of deployment pods
    check_kubernetes_pods_status(constants.AZMON_CI_EXTENSION_RESOURCES_NAMESPACE,
                                 env_dict['TEST_CONTAINER_INSIGHTS_LOG_FILE'], constants.AZMON_CI_EXTENSION_DEPLOYMENT_POD_LABEL_LIST, timeout_seconds)

    # Checking the status of daemonset pods
    check_kubernetes_pods_status(constants.AZMON_CI_EXTENSION_RESOURCES_NAMESPACE,
                                 env_dict['TEST_CONTAINER_INSIGHTS_LOG_FILE'], constants.AZMON_CI_EXTENSION_DAEMONSET_POD_LABEL_LIST, timeout_seconds)

    # check the cluster identity crd status
    # status_dict = {}
    # status_dict['tokenReference'] = {}
    # status_dict['tokenReference']['dataName'] = 'cluster-identity-token'
    # status_dict['tokenReference']['secretName'] = 'container-insights-clusteridentityrequest-token'

    # check_kubernetes_crd_status(constants.AZMON_CI_EXTENSION_CLUSTER_IDENTITY_CRD_GROUP, constants.AZMON_CI_EXTENSION_CLUSTER_IDENTITY_CRD_VERSION,
    #                             constants.AZURE_ARC_NAMESPACE, constants.AZMON_CI_EXTENSION_CLUSTER_IDENTITY_CRD_PLURAL,
    #                             constants.AZMON_CI_EXTENSION_CLUSTER_IDENTITY_CRD_NAME, status_dict, env_dict['TEST_CONTAINER_INSIGHTS_LOG_FILE'], timeout_seconds)

    print("Successfully checked container insights extension.")

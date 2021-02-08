import pytest
import constants

from kubernetes import client, config
from helper import check_kubernetes_pods_status
from helper import check_kubernetes_daemonset_status, check_kubernetes_deployments_status

pytestmark = pytest.mark.agenttests


def test_resource_status(env_dict):
    print("Starting resource status check.")

    # Loading in-cluster kube-config
    try:
        config.load_incluster_config()
        #config.load_kube_config()
    except Exception as e:
        pytest.fail("Error loading the in-cluster config: " + str(e))

    timeout_seconds = env_dict.get('TIMEOUT')

    # checking the deployment status
    check_kubernetes_deployments_status(constants.AGENT_NAMESPACE,
                                        env_dict['TEST_AGENT_LOG_FILE'], constants.AGENT_DEPLOYMENT_LABEL_LIST, timeout_seconds)

    # checking the daemonset status
    check_kubernetes_daemonset_status(constants.AGENT_NAMESPACE,
                                      env_dict['TEST_AGENT_LOG_FILE'], constants.AGENT_DAEMONSET_LABEL_LIST, timeout_seconds)

    # Checking the status of deployment pods
    check_kubernetes_pods_status(constants.AGENT_NAMESPACE,
                                 env_dict['TEST_AGENT_LOG_FILE'], constants.AGENT_DEPLOYMENT_POD_LABEL_LIST, timeout_seconds)

    # Checking the status of daemonset pods
    check_kubernetes_pods_status(constants.AGENT_NAMESPACE,
                                 env_dict['TEST_AGENT_LOG_FILE'], constants.AGENT_DAEMONSET_POD_LABEL_LIST, timeout_seconds)
  

    print("Successfully checked resources status check.")

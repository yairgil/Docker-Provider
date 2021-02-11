import pytest
import constants

from kubernetes import client, config
from kubernetes_pod_utility import get_pod_list, get_log_file_content
from results_utility import append_result_output
from helper import check_kubernetes_deployment_status
from helper import check_kubernetes_daemonset_status
from helper import check_kubernetes_pods_status
from kubernetes.stream import stream
# from helper import check_kubernetes_daemonset_status
# from helper import check_kubernetes_pods_status
# from helper import check_kubernetes_pod_logs
# from helper import check_kubernetes_pods_status, check_namespace_status
# from helper import check_kubernetes_daemonset_status, check_kubernetes_deployment_status
# from helper import check_kubernetes_crd_status

pytestmark = pytest.mark.agentests

def test_rs_workflows(env_dict):
    print("Starting replicaset workflows.")
    append_result_output("test_resource_status start \n", env_dict['TEST_AGENT_LOG_FILE'])
    # Loading in-cluster kube-config
    try:
        config.load_incluster_config()
        #config.load_kube_config()
    except Exception as e:
        pytest.fail("Error loading the in-cluster config: " + str(e))

    print("getting pod list")
    api_instance = client.CoreV1Api()
    pod_list = get_pod_list(api_instance, constants.AGENT_RESOURCES_NAMESPACE, constants.AGENT_DEPLOYMENT_PODS_LABEL_SELECTOR)
    if not pod_list:
        pytest.fail("pod_list shouldnt be null or empty")        
    
    if len(pod_list.items) <= 0:
        pytest.fail("number of items in pod list should be greater than 0")        
   
    rspodName = pod_list.items[0].metadata.name
    logcontent = get_log_file_content(api_instance, constants.AGENT_RESOURCES_NAMESPACE, rspodName, constants.AGENT_OMSAGENT_LOG_PATH)    
    if not logcontent:
        pytest.fail("logcontent should not be null or empty")                 
    loglines  = logcontent.split("\n")
    if len(loglines) > 0:
        pytest.fail("number of log lines should be greater than 0")                 
        
    IsKubePodInventorySuccessful = False 
    IsKubeNodeInventorySuccessful = False 
    for line in loglines:
        if line.find(constants.POD_INVENTORY_EMIT_STREAM) >= 0:    
            IsKubePodInventorySuccessful = True         
        if line.find(constants.NODE_INVENTORY_EMIT_STREAM) >= 0:    
            IsKubePodInventorySuccessful = True                            
    
    if IsKubePodInventorySuccessful == False:
       pytest("KubePodInventory stream not emitted successfully")  
           
    if IsKubeNodeInventorySuccessful == False:
        pytest("IsKubeNodeInventorySuccessful stream not emitted successfully")  
        
    append_result_output("test_resource_status end \n", env_dict['TEST_AGENT_LOG_FILE'])    
    print("Successfully replicaset workflows.")

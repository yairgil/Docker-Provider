import pytest
import requests
import time

from kubernetes import client
from kubernetes_crd_utility import watch_crd_instance
from kubernetes_pod_utility import watch_pod_status, watch_pod_logs, watch_deployment_status, watch_daemonset_status
from kubernetes_deployment_utility import watch_deployment_status, watch_daemonset_status
from kubernetes_deamonset_utility import watch_daemonset_status
from kubernetes_configmap_utility import get_namespaced_configmap
from kubernetes_secret_utility import watch_kubernetes_secret
from kubernetes_namespace_utility import watch_namespace
from results_utility import append_result_output


# This function checks the status of the namespaces of the kubernetes cluster. The namespaces to be monitored are passed in as a list.
def check_namespace_status(outfile=None, namespace_list=None, timeout=300):
    namespace_dict = {}
    for namespace in namespace_list:
        namespace_dict[namespace] = 0
    append_result_output("Namespace dict: {}\n".format(namespace_dict), outfile)
    print("Generated the namespace dictionary.")

    # THe callback function to check the namespace status
    def namespace_event_callback(event):
        try:
            append_result_output("{}\n".format(event), outfile)
            namespace_name = event['raw_object'].get('metadata').get('name')
            namespace_status = event['raw_object'].get('status')
            if not namespace_status:
                return False
            if namespace_status.get('phase') == 'Active':
                namespace_dict[namespace_name] = 1
            if all(ele == 1 for ele in list(namespace_dict.values())):
                return True
            return False
        except Exception as e:
            pytest.fail("Error occured while processing the namespace event: " + str(e))
    
    # Checking the namespace status
    api_instance = client.CoreV1Api()
    watch_namespace(api_instance, timeout, namespace_event_callback)

# This function checks the status of daemonset in a given namespace. The daemonset to be monitored are identified using the pod label list parameter.        
def check_kubernetes_daemonset_status(daemonset_namespace, outfile=None, daemonset_label_list=None, timeout=300):
    daemonset_label_dict = {}
    if daemonset_label_list:  # This parameter is a list of label values to identify the daemonsets that we want to monitor in the given namespace
        for daemonset_label in daemonset_label_list:
            daemonset_label_dict[daemonset_label] = 0
    append_result_output("daemonset label dict: {}\n".format(daemonset_label_dict), outfile)
    print("Generated the daemonset dictionary.")

    # The callback function to check if the pod is in running state
    def daemonset_event_callback(event):
        try:
            append_result_output("{}\n".format(event), outfile)
            daemonset_status = event['raw_object'].get('status')
            daemonset_metadata = event['raw_object'].get('metadata')
            daemonset_metadata_labels = daemonset_metadata.get('labels')
            if not daemonset_metadata_labels:
                return False

            daemonset_metadata_label_values = daemonset_metadata_labels.values()  # It contains the list of all label values for the pod whose event was called.
            current_label_value = None  # This label value will be common in pod event and label list provided and will be monitored
            for label_value in daemonset_metadata_label_values:
                if label_value in daemonset_label_dict:
                    current_label_value = label_value
            if not current_label_value:
                return False

            currentNumberScheduled = daemonset_status.get('currentNumberScheduled')
            desiredNumberScheduled = daemonset_status.get('desiredNumberScheduled')
            numberAvailable = daemonset_status.get('numberAvailable')
            numberReady = daemonset_status.get('numberReady')
            numberMisscheduled = daemonset_status.get('numberMisscheduled')
            

            if (currentNumberScheduled != desiredNumberScheduled):
                 pytest.fail("currentNumberScheduled doesnt match with currentNumberScheduled for the daemonset {}.".format(daemonset_metadata.get('name')))    

            if (numberAvailable != numberReady):
                 pytest.fail("numberAvailable doesnt match with expected numberReady for the daemonset {}.".format(daemonset_metadata.get('name'))) 

            if (numberMisscheduled > 0 ):
                 pytest.fail("numberMisscheduled is greater than 0 for the daemonset {}.".format(daemonset_metadata.get('name'))) 
                 
            return True     
        except Exception as e:
            print("Error occured while processing the pod event: " + str(e))

    # Checking status of all pods
    if daemonset_label_dict:
        api_instance = client.AppsV1Api()
        watch_daemonset_status(api_instance, daemonset_namespace, timeout, daemonset_event_callback)

# This function checks the status of deployment in a given namespace. The deployment to be monitored are identified using the pod label list parameter.             
def check_kubernetes_deployments_status(deployment_namespace, outfile=None, deployment_label_list=None, timeout=300):
    deployment_label_dict = {}
    if deployment_label_list:  # This parameter is a list of label values to identify the deployments that we want to monitor in the given namespace
        for deployment_label in deployment_label_list:
            deployment_label_dict[deployment_label] = 0
    append_result_output("Deployment label dict: {}\n".format(deployment_label_dict), outfile)
    print("Generated the deployment dictionary.")

    # The callback function to check if the pod is in running state
    def deployment_event_callback(event):
        try:
            append_result_output("{}\n".format(event), outfile)
            deployment_status = event['raw_object'].get('status')
            deployment_metadata = event['raw_object'].get('metadata')
            deployment_metadata_labels = deployment_metadata.get('labels')
            if not deployment_metadata_labels:
                return False

            deployment_metadata_label_values = deployment_metadata_labels.values()  # It contains the list of all label values for the deployment whose event was called.
            current_label_value = None  # This label value will be common in deployment event and label list provided and will be monitored
            for label_value in deployment_metadata_label_values:
                if label_value in deployment_label_dict:
                    current_label_value = label_value
            if not current_label_value:
                return False

            availableReplicas = deployment_status.get('availableReplicas')
            readyReplicas = deployment_status.get('readyReplicas')
            replicas = deployment_status.get('replicas')

            if (replicas != availableReplicas):
                pytest.fail("availableReplicas doesnt match with expected replicas for the deployment {}.".format(deployment_metadata.get('name')))    

            if (replicas != readyReplicas):
                pytest.fail("readyReplicas doesnt match with expected replicas for the deployment {}.".format(deployment_metadata.get('name'))) 
                 
            return True     
        except Exception as e:
            print("Error occured while processing the pod event: " + str(e))

    # Checking status of all pods
    if deployment_label_dict:
        api_instance = client.AppsV1Api()
        watch_deployment_status(api_instance, deployment_namespace, timeout, deployment_event_callback)

# This function checks the status of pods in a given namespace. The pods to be monitored are identified using the pod label list parameter.
def check_kubernetes_pods_status(pod_namespace, outfile=None, pod_label_list=None, timeout=300):
    pod_label_dict = {}
    if pod_label_list:  # This parameter is a list of label values to identify the pods that we want to monitor in the given namespace
        for pod_label in pod_label_list:
            pod_label_dict[pod_label] = 0
    append_result_output("Pod label dict: {}\n".format(pod_label_dict), outfile)
    print("Generated the pods dictionary.")

    # The callback function to check if the pod is in running state
    def pod_event_callback(event):
        try:
            append_result_output("{}\n".format(event), outfile)
            pod_status = event['raw_object'].get('status')
            pod_metadata = event['raw_object'].get('metadata')
            pod_metadata_labels = pod_metadata.get('labels')
            if not pod_metadata_labels:
                return False

            pod_metadata_label_values = pod_metadata_labels.values()  # It contains the list of all label values for the pod whose event was called.
            current_label_value = None  # This label value will be common in pod event and label list provided and will be monitored
            for label_value in pod_metadata_label_values:
                if label_value in pod_label_dict:
                    current_label_value = label_value
            if not current_label_value:
                return False

            if pod_status.get('containerStatuses'):
                for container in pod_status.get('containerStatuses'):
                    if container.get('restartCount') > 0:
                        pytest.fail("The pod {} was restarted. Please see the pod logs for more info.".format(container.get('name')))
                    if not container.get('state').get('running'):
                        pod_label_dict[current_label_value] = 0
                        return False
                    else:
                        pod_label_dict[current_label_value] = 1
            if all(ele == 1 for ele in list(pod_label_dict.values())):
                return True
            return False
        except Exception as e:
            pytest.fail("Error occured while processing the pod event: " + str(e))

    # Checking status of all pods
    if pod_label_dict:
        api_instance = client.CoreV1Api()
        watch_pod_status(api_instance, pod_namespace, timeout, pod_event_callback)

# Function to check if the crd instance status has been updated with the status fields mentioned in the 'status_list' parameter
def check_kubernetes_crd_status(crd_group, crd_version, crd_namespace, crd_plural, crd_name, status_dict={}, outfile=None, timeout=300):
    # The callback function to check if the crd event received has been updated with the status fields
    def crd_event_callback(event):
        try:
            append_result_output("{}\n".format(event), outfile)
            crd_status = event['raw_object'].get('status')
            if not crd_status:
                return False
            for status_field in status_dict:
                if not crd_status.get(status_field):
                    return False
                if crd_status.get(status_field) != status_dict.get(status_field):
                    pytest.fail("The CRD instance status has been updated with incorrect value for '{}' field.".format(status_field))
            return True
        except Exception as e:
            pytest.fail("Error occured while processing crd event: " + str(e))

    # Checking if CRD instance has been updated with status fields
    api_instance = client.CustomObjectsApi()
    watch_crd_instance(api_instance, crd_group, crd_version, crd_namespace, crd_plural, crd_name, timeout, crd_event_callback)

# Function to monitor the pod logs. It will ensure that are logs passed in the 'log_list' parameter are present in the container logs.
def check_kubernetes_pod_logs(pod_namespace, pod_name, container_name, logs_list=None, error_logs_list=None, outfile=None, timeout=300):
    logs_dict = {}
    for log in logs_list:
        logs_dict[log] = 0
    print("Generated the logs dictionary.")

    # The callback function to examine the pod log
    def pod_log_event_callback(event):
        try:
            append_result_output("{}\n".format(event), outfile)
            for error_log in error_logs_list:
                if error_log in event:
                    pytest.fail("Error log found: " + event)
            for log in logs_dict:
                if log in event:
                    logs_dict[log] = 1
            if all(ele == 1 for ele in list(logs_dict.values())):
                return True
            return False
        except Exception as e:
            pytest.fail("Error occured while processing pod log event: " + str(e))

    # Checking the pod logs
    api_instance = client.CoreV1Api()
    watch_pod_logs(api_instance, pod_namespace, pod_name, container_name, timeout, pod_log_event_callback)

# Function to monitor the kubernetes secret. It will determine if the secret has been successfully created.
def check_kubernetes_secret(secret_namespace, secret_name, timeout=300):
    # The callback function to check if the secret event received has secret data
    def secret_event_callback(event):
        try:
            secret_data = event['raw_object'].get('data')
            if not secret_data:
                return False   
            return True
        except Exception as e:
            pytest.fail("Error occured while processing secret event: " + str(e))
    
    # Checking the kubernetes secret
    api_instance = client.CoreV1Api()
    watch_kubernetes_secret(api_instance, secret_namespace, secret_name, timeout, secret_event_callback)

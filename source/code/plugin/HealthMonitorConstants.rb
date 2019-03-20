#!/usr/local/bin/ruby
# frozen_string_literal: true

class HealthMonitorConstants
    NODE_CPU_MONITOR_ID = "node_cpu_utilization_percentage"
    NODE_MEMORY_MONITOR_ID = "node_memory_utilization_percentage"
    NODE_KUBELET_HEALTH_MONITOR_ID = "kubelet_running"
    NODE_CONDITION_MONITOR_ID = "node_condition"
    NODE_CONTAINER_RUNTIME_MONITOR_ID = "container_manager_runtime_running"
    WORKLOAD_CPU_OVERSUBSCRIBED_MONITOR_ID = "is_oversubscribed_cpu"
    WORKLOAD_MEMORY_OVERSUBSCRIBED_MONITOR_ID = "is_oversubscribed_memory"
    WORKLOAD_PODS_READY_PERCENTAGE_MONITOR_ID = "pods_ready_percentage"
    WORKLOAD_CONTAINER_CPU_PERCENTAGE_MONITOR_ID = "container_cpu_utilization_percentage"
    WORKLOAD_CONTAINER_MEMORY_PERCENTAGE_MONITOR_ID = "container_memory_utilization_percentage"
    MANAGEDINFRA_KUBEAPI_AVAILABLE_MONITOR_ID = "kube_api_up"
    MANAGEDINFRA_PODS_READY_PERCENTAGE_MONITOR_ID = "pods_ready_percentage"
    POD_STATUS = "pod_status"
    DEFAULT_PASS_PERCENTAGE = 80.0
    DEFAULT_FAIL_PERCENTAGE = 90.0
    DEFAULT_MONITOR_TIMEOUT = 240 #4 hours
    DEFAULT_SAMPLES_BEFORE_NOTIFICATION = 3
end
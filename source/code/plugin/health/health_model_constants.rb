module HealthModel
    class MonitorState
        CRITICAL = "fail"
        ERROR = "fail"
        WARNING = "warn"
        NONE = "none"
        HEALTHY = "pass"
    end

    class AggregationAlgorithm
        WORSTOF = "worstOf"
        PERCENTAGE = "percentage"
    end

    class MonitorId
        CLUSTER = 'cluster';
        ALL_NODES = 'all_nodes';
        K8S_INFRASTRUCTURE = 'k8s_infrastructure'


        NODE = 'node';
        AGENT_NODE_POOL = 'agent_node_pool'
        MASTER_NODE_POOL = 'master_node_pool'
        ALL_AGENT_NODE_POOLS = 'all_agent_node_pools'
        ALL_NODE_POOLS = 'all_node_pools';

        WORKLOAD = 'workload';
        MANAGED_INFRA = 'managed_infra'
        CAPACITY = 'capacity';

        POD_AGGREGATOR = 'pod_aggregator';
        SYSTEM_POD_AGGREGATOR = 'system_pod_aggregator'
        NAMESPACE = 'namespace';
        NAMESPACES = 'namespaces';
    end

    class HealthMonitorRecordFields
        CLUSTER_ID = "ClusterId"
        MONITOR_ID = "MonitorId"
        MONITOR_INSTANCE_ID = "MonitorInstanceId"
        MONITOR_LABELS = "MonitorLabels"
        DETAILS = "Details"
        MONITOR_CONFIG = "MonitorConfig"
        OLD_STATE = "OldState"
        NEW_STATE = "NewState"
        AGENT_COLLECTION_TIME = "AgentCollectionTime"
        TIME_FIRST_OBSERVED = "TimeFirstObserved"
        NODE_NAME = "NodeName"
        POD_AGGREGATOR = "PodAggregator"
        NAMESPACE = "Namespace"
        CONTAINER_ID = "ContainerID"
    end

    class HealthAspect
        NODES = "Nodes"
        KUBERNETES_INFRASTRUCTURE = "Kubernetes infrastructure"
        WORKLOAD = "Workload"
    end
end
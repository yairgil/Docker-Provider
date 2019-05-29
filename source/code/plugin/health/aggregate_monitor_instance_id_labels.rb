module HealthModel
    class AggregateMonitorInstanceIdLabels
        @@id_labels_mapping = {
            MonitorId::SYSTEM_POD_AGGREGATOR => ["container.azm.ms/namespace", "container.azm.ms/pod-aggregator"],
            MonitorId::POD_AGGREGATOR => ["container.azm.ms/namespace", "container.azm.ms/pod-aggregator"],
            MonitorId::NODE => ["agentpool", "kubernetes.io/role", "kubernetes.io/hostname"],
            MonitorId::NAMESPACE => ["container.azm.ms/namespace"],
            MonitorId::AGENT_NODE_POOL => ["agentpool"],
            # MonitorId::ALL_AGENT_NODE_POOLS => [],
            # MonitorId::ALL_NODE_POOLS => [],
            # MonitorId::ALL_NODES => [],
            # MonitorId::K8S_INFRASTRUCTURE => [],
            # MonitorId::CLUSTER => [],
            # MonitorId::WORKLOAD => []
        }

        def self.get_labels_for(monitor_id)
            if @@id_labels_mapping.key?(monitor_id)
                return @@id_labels_mapping[monitor_id]
            else
                return []
            end

        end
    end
end
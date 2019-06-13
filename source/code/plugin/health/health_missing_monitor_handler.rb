module HealthModel
    class HealthMissingMonitorHandler

        attr_accessor :last_sent_monitors, :unknown_state_candidates

        def initialize(last_sent_monitors, unknown_state_candidates)
            @last_sent_monitors = {}
            @unknown_state_candidates = {}
            @node_inventory = {}
            @workload_inventory ={}
        end

        def detect_missing_signals(received_records)
            nodes = get_node_inventory(received_records)
            workloads = get_workload_inventory(received_records)

            received_records.each{|record|
                monitor_id = record[HealthMonitorRecordFields::MONITOR_ID]
                case monitor_id
                when HealthMonitorConstants::NODE_CPU_MONITOR_ID, HealthMonitorConstants::NODE_MEMORY_MONITOR_ID
                    # node monitor processing
                    # check if present in last_sent_monitors
                    # if not present
                when HealthMonitorConstants::USER_WORKLOAD_PODS_READY_MONITOR_ID, HealthMonitorConstants::SYSTEM_WORKLOAD_PODS_READY_MONITOR_ID
                    # pods ready percentage processing
                when HealthMonitorConstants::KUBE_API_STATUS
                    # kube api status
                end
            }
        end

        def get_node_inventory(received_records)
            @node_inventory = []
            node_records = received_records.select {|record| record[HealthMonitorRecordFields::MONITOR_ID] == HealthMonitorConstants::NODE_CONDITION_MONITOR_ID}
            node_records.each{|node_record|
                node_name = JSON.parse(node_record[HealthMonitorRecordFields::MONITOR_LABELS])['kubernetes.io/hostname']
                @node_inventory.push(node_name) if node_name
            }
        end

        def get_workload_inventory(received_records)
            @workload_inventory = []
            workload_records = received_records.select {|record|
                (record[HealthMonitorRecordFields::MONITOR_ID] == HealthMonitorConstants::USER_WORKLOAD_PODS_READY_MONITOR_ID ||
                    record[HealthMonitorRecordFields::MONITOR_ID] == HealthMonitorConstants::SYSTEM_WORKLOAD_PODS_READY_MONITOR_ID
                )
            }
            workload_records.each{|workload_record|
                workload_name = JSON.parse(workload_record[HealthMonitorRecordFields::MONITOR_LABELS])['container.azm.ms/workload-name']
                @workload_inventory.push(workload_name)
            }
        end


    end
end
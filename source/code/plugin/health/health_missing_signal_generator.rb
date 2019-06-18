module HealthModel
    class HealthMissingSignalGenerator
        attr_accessor :last_received_records, :current_received_records
        attr_reader :missing_signals

        def initialize()
            @last_received_records = {}
        end

        def get_missing_signals(cluster_id, health_monitor_records, health_k8s_inventory, provider)
            missing_monitor_ids = []
            nodes = health_k8s_inventory.get_nodes
            workload_names = health_k8s_inventory.get_workload_names
            missing_signals_map = {}
            missing_signals = []
            health_monitor_records_map = {}
            health_monitor_records.map{
                |monitor| health_monitor_records_map[monitor.monitor_instance_id] = monitor
            }

            node_signals_hash = {}
            nodes.each{|node|
                node_signals_hash[node] = [HealthMonitorConstants::NODE_CPU_MONITOR_ID, HealthMonitorConstants::NODE_MEMORY_MONITOR_ID, HealthMonitorConstants::NODE_CONDITION_MONITOR_ID]
            }

            @last_received_records.each{|monitor_instance_id, monitor|
                if !health_monitor_records_map.key?(monitor_instance_id)
                    if HealthMonitorUtils.is_node_monitor(monitor.monitor_id)
                        node_name = monitor.labels['kubernetes.io/hostname']
                        new_monitor = HealthMonitorRecord.new(
                            monitor.monitor_id,
                            monitor.monitor_instance_id,
                            Time.now.utc.iso8601,
                            monitor.state,
                            monitor.labels,
                            monitor.config,
                            monitor.details
                        )
                        if !node_name.nil? && nodes.include?(node_name)
                            new_monitor.state = HealthMonitorStates::UNKNOWN
                        elsif !node_name.nil? && !nodes.include?(node_name)
                            new_monitor.state = HealthMonitorStates::NONE
                        end
                        missing_signals_map[monitor_instance_id] = new_monitor
                    elsif HealthMonitorUtils.is_pods_ready_monitor(monitor.monitor_id)
                        lookup = "#{monitor.labels['container.azm.ms/namespace']}~~#{monitor.labels['container.azm.ms/workload-name']}"
                        new_monitor = HealthMonitorRecord.new(
                            monitor.monitor_id,
                            monitor.monitor_instance_id,
                            Time.now.utc.iso8601,
                            monitor.state,
                            monitor.labels,
                            monitor.config,
                            monitor.details
                        )
                        if !lookup.nil? && workload_names.include?(lookup)
                            new_monitor.state = HealthMonitorStates::UNKNOWN
                        elsif !lookup.nil? && !workload_names.include?(lookup)
                            new_monitor.state = HealthMonitorStates::NONE
                        end
                        missing_signals_map[monitor_instance_id] = new_monitor
                    end
                end
            }

            # remove signals from the list of expected signals if we see them in the list of current signals
            health_monitor_records.each{|health_monitor_record|
                if HealthMonitorUtils.is_node_monitor(health_monitor_record.monitor_id)
                    node_name = health_monitor_record.labels['kubernetes.io/hostname']
                    if node_signals_hash.key?(node_name)
                        signals = node_signals_hash[node_name]
                        signals.delete(health_monitor_record.monitor_id)
                        if signals.size == 0
                            node_signals_hash.delete(node_name)
                        end
                    end
                end
            }

            # if the hash is not empty, means we have missing signals
            if node_signals_hash.size > 0
                # these signals were not sent previously
                # these signals need to be assigned an unknown state
                node_signals_hash.each{|node, monitor_ids|
                    monitor_ids.each{|monitor_id|
                        monitor_instance_id = HealthMonitorUtils.get_monitor_instance_id(monitor_id, [cluster_id, node])
                        new_monitor = HealthMonitorRecord.new(
                            monitor_id,
                            monitor_instance_id,
                            Time.now.utc.iso8601,
                            HealthMonitorStates::UNKNOWN,
                            provider.get_node_labels(node),
                            {},
                            {"timestamp" => Time.now.utc.iso8601, "state" => HealthMonitorStates::UNKNOWN, "details" => "no signal received from node #{node}"}
                        )
                        missing_signals_map[monitor_instance_id] = new_monitor
                    }
                }
            end

            missing_signals_map.each{|k,v|
                missing_signals.push(v)
            }

            return missing_signals
        end

        def update_last_received_records(last_received_records)
            last_received_records_map = {}
            last_received_records.map {|record| last_received_records_map[record.monitor_instance_id] = record }
            @last_received_records = last_received_records_map
        end
    end

end
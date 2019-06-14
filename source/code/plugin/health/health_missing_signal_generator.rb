module HealthModel
    class HealthMissingSignalGenerator
        attr_accessor :last_received_records, :current_received_records
        attr_reader :missing_signals

        def initialize()
            @last_received_records = {}
        end

        def get_missing_signals(health_monitor_records, health_k8s_inventory)
            missing_monitor_ids = []
            nodes = health_k8s_inventory.get_nodes
            workload_names = health_k8s_inventory.get_workload_names
            missing_signals_map = {}
            missing_signals = []
            health_monitor_records_map = {}
            health_monitor_records.map{
                |monitor| health_monitor_records_map[monitor.monitor_instance_id] = monitor
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
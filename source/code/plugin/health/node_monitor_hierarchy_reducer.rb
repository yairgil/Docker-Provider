# frozen_string_literal: true

module HealthModel
  class NodeMonitorHierarchyReducer
    def initialize
    end

    # Finalizes the Node Hierarchy. This removes node pools and node pool set from the hierarchy if they are not present.
    def finalize(monitor_set)
      monitors_to_reduce = [MonitorId::ALL_AGENT_NODE_POOLS, MonitorId::ALL_NODES]
      # for the above monitors, which are constant per cluster, the monitor_id and monitor_instance_id are the same
      monitors_to_reduce.each do |monitor_to_reduce|
        monitor = monitor_set.get_monitor(monitor_to_reduce)
        if !monitor.nil?
            puts "Before Deleting #{monitor_set.get_size}"
            if monitor.is_aggregate_monitor && monitor.get_member_monitors.size == 1
                #copy the children of member monitor as children of parent
                member_monitor_instance_id = monitor.get_member_monitors[0] #gets the only member monitor instance id
                member_monitor = monitor_set.get_monitor(member_monitor_instance_id)
                member_monitor.get_member_monitors.each{|grandchild_monitor|
                    monitor.add_member_monitor(grandchild_monitor)
                }
                monitor.remove_member_monitor(member_monitor_instance_id)
                # delete the member monitor from the monitor_set
                monitor_set.delete(member_monitor_instance_id)
                puts "After Deleting #{monitor_set.get_size}"
            end
        end
      end
    end
  end
end

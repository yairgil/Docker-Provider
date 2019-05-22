module HealthModel
    class MonitorFactory

        def initialize

        end

        def create_unit_monitor(monitor_state_transition)
            return UnitMonitor.new(monitor_state_transition.monitor_id,
                monitor_state_transition.monitor_instance_id,
                monitor_state_transition.old_state,
                monitor_state_transition.new_state,
                monitor_state_transition.transition_date_time,
                monitor_state_transition.labels,
                monitor_state_transition.config,
                monitor_state_transition.details)
        end

        def create_aggregate_monitor(monitor_id, monitor_instance_id, labels, aggregation_algorithm, aggregation_algorithm_params, child_monitor)
            return AggregateMonitor.new(monitor_id,
                monitor_instance_id,
                child_monitor.old_state,
                child_monitor.new_state,
                child_monitor.transition_time,
                aggregation_algorithm,
                aggregation_algorithm_params,
                labels)
        end
    end
end
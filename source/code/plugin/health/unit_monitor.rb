require_relative 'health_model_constants'
require 'json'

module HealthModel
    class UnitMonitor

        attr_accessor :monitor_id, :monitor_instance_id, :old_state, :new_state, :transition_time, :labels, :config, :details, :is_aggregate_monitor

        # constructor
        def initialize(monitor_id, monitor_instance_id, old_state, new_state, transition_time, labels, config, details)
            @monitor_id = monitor_id
            @monitor_instance_id = monitor_instance_id
            @old_state = old_state
            @new_state = new_state
            @transition_time = transition_time
            @labels = JSON.parse(labels)
            @config = config
            @details = details
            @is_aggregate_monitor = false
        end

        def get_member_monitors
            return nil
        end

    end
end
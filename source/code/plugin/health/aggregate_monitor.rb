# frozen_string_literal: true

require_relative 'health_model_constants'
require 'json'

module HealthModel
  class AggregateMonitor
    attr_accessor :monitor_id, :monitor_instance_id, :state, :transition_date_time, :aggregation_algorithm, :aggregation_algorithm_params, :labels, :is_aggregate_monitor, :details
    attr_reader :member_monitors

    # constructor
    def initialize(
      monitor_id,
      monitor_instance_id,
      state,
      transition_date_time,
      aggregation_algorithm,
      aggregation_algorithm_params,
      labels
    )
      @monitor_id = monitor_id
      @monitor_instance_id = monitor_instance_id
      @state = state
      @transition_date_time = transition_date_time
      @aggregation_algorithm = aggregation_algorithm || AggregationAlgorithm::WORSTOF
      @aggregation_algorithm_params = aggregation_algorithm_params
      @labels = labels
      @member_monitors = {}
      @is_aggregate_monitor = true
    end

    # adds a member monitor as a child
    def add_member_monitor(member_monitor_instance_id)
      unless @member_monitors.key?(member_monitor_instance_id)
        @member_monitors[member_monitor_instance_id] = true
      end
    end

    #removes a member monitor
    def remove_member_monitor(member_monitor_instance_id)
        if @member_monitors.key?(member_monitor_instance_id)
            @member_monitors.delete(member_monitor_instance_id)
        end
    end

    # return the member monitors as an array
    def get_member_monitors
      @member_monitors.map(&:first)
    end

    # calculates the state of the aggregate monitor based on aggregation algorithm and child monitor states
    def calculate_state(monitor_set)
        case @aggregation_algorithm
        when AggregationAlgorithm::WORSTOF
            @state = calculate_worst_of_state(monitor_set)
        when AggregationAlgorithm::PERCENTAGE
            @state = calculate_percentage_state(monitor_set)
        end
    end

    def calculate_details(monitor_set)
        @details = {}
        @details['details'] = {}
        @details['state'] = state
        @details['timestamp'] = transition_date_time
        ids = []
        member_monitor_instance_ids = get_member_monitors
        member_monitor_instance_ids.each{|member_monitor_id|
            member_monitor = monitor_set.get_monitor(member_monitor_id)
            member_state = member_monitor.state
            if @details['details'].key?(member_state)
                ids = @details['details'][member_state]
                if !ids.include?(member_monitor.monitor_instance_id)
                    ids.push(member_monitor.monitor_instance_id)
                end
                @details['details'][member_state] = ids
            else
                @details['details'][member_state] = [member_monitor.monitor_instance_id]
            end
        }
    end

    # calculates the worst of state, given the member monitors
    def calculate_worst_of_state(monitor_set)

        member_state_counts = map_member_monitor_states(monitor_set)

        if member_state_counts.length === 0
            return MonitorState::NONE
        end

        if member_state_counts.key?(MonitorState::CRITICAL) && member_state_counts[MonitorState::CRITICAL] > 0
            return MonitorState::CRITICAL
        end
        if member_state_counts.key?(MonitorState::ERROR) && member_state_counts[MonitorState::ERROR] > 0
            return MonitorState::ERROR
        end
        if member_state_counts.key?(MonitorState::WARNING) &&  member_state_counts[MonitorState::WARNING] > 0
            return MonitorState::WARNING
        end

        if member_state_counts.key?(MonitorState::NONE) && member_state_counts[MonitorState::NONE] > 0
            return MonitorState::NONE
        end

        return MonitorState::HEALTHY
    end

    # calculates a percentage state, given the aggregation algorithm parameters
    def calculate_percentage_state

    end

    # maps states of member monitors to counts
    def map_member_monitor_states(monitor_set)
        member_monitor_instance_ids = get_member_monitors
        if member_monitor_instance_ids.nil? || member_monitor_instance_ids.size == 0
            return {}
        end

        state_counts = {}

        member_monitor_instance_ids.each {|monitor_instance_id|

            member_monitor = monitor_set.get_monitor(monitor_instance_id)
            monitor_state = member_monitor.state

            if !state_counts.key?(monitor_state)
                state_counts[monitor_state] = 1
            else
                count = state_counts[monitor_state]
                state_counts[monitor_state] = count+1
            end
        }

        return state_counts;
    end
  end
end

require_relative 'health_model_constants'
require 'time'

module HealthModel
    class HealthModelBuilder
        attr_accessor :state_transition_processor, :state_finalizers, :monitor_set
        attr_reader :last_sent_monitors

        def initialize(state_transition_processor, state_finalizers, monitor_set)
            @state_transition_processor = state_transition_processor
            @state_finalizers = state_finalizers
            @monitor_set = monitor_set
            @last_sent_monitors = {}
        end

        def process_state_transitions(state_transitions)
            state_transitions.each{|transition|
                @state_transition_processor.process_state_transition(transition, @monitor_set)
            }
        end

        def finalize_model
            if !@state_finalizers.is_a?(Array)
                raise 'state finalizers should be an array'
            end

            if @state_finalizers.length == 0
                raise '@state_finalizers length should not be zero or empty'
            end

            @state_finalizers.each{|finalizer|
                finalizer.finalize(@monitor_set)
            }

            # return only those monitors whose state has changed, ALWAYS including the cluster level monitor
            monitors_map = get_changed_monitors

            # monitors_map.each{|key, value|
            #     puts "#{key} ==> #{value.state}"
            # }
            # puts "*****************************************************"

            update_last_sent_monitors
            clear_monitors
            return monitors_map
        end

        private
        def get_changed_monitors
            changed_monitors = {}
            # always send cluster monitor as a 'heartbeat'
            top_level_monitor = @monitor_set.get_monitor(MonitorId::CLUSTER)
            if top_level_monitor.nil?
                top_level_monitor = AggregateMonitor.new(MonitorId::CLUSTER, MonitorId::CLUSTER, @last_sent_monitors[MonitorId::CLUSTER].old_state, @last_sent_monitors[MonitorId::CLUSTER].new_state, @last_sent_monitors[MonitorId::CLUSTER].transition_time, AggregationAlgorithm::WORSTOF, nil, {})
            end
            changed_monitors[MonitorId::CLUSTER] = top_level_monitor

            @monitor_set.get_map.each{|monitor_instance_id, monitor|
                if @last_sent_monitors.key?(monitor_instance_id)
                    last_sent_monitor_state = @last_sent_monitors[monitor_instance_id].new_state
                    if last_sent_monitor_state.downcase != monitor.new_state.downcase
                        changed_monitors[monitor_instance_id] = monitor
                    end
                else
                    changed_monitors[monitor_instance_id] = monitor
                end
            }
            return changed_monitors
        end

        def update_last_sent_monitors
            @monitor_set.get_map.map{|instance_id, monitor|
                @last_sent_monitors[instance_id] = monitor
            }
        end

        def clear_monitors
            @monitor_set = MonitorSet.new
        end

    end
end
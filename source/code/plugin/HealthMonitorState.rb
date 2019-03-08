#!/usr/local/bin/ruby
# frozen_string_literal: true

require_relative 'HealthEventsConstants'

HealthMonitorRecord = Struct.new(:timestamp, :state, :details) do
end

HealthMonitorInstanceState = Struct.new(:prev_sent_record_time, :prev_sent_record_status, :prev_records) do
end

class HealthMonitorState
    @@instanceStates = {} #hash of monitor_instance_id --> health monitor instance state
    @@firstMonitorRecordSent = {}
    HEALTH_MONITOR_STATE = {"PASS" => "Pass", "FAIL" => "Fail", "WARNING" => "Warning"}

    class << self
        def updateHealthMonitorState(log, monitor_instance_id, health_monitor_record, config)
            log.debug "updateHealthMonitorState"
            samples_to_keep = 1
            if config.nil? || config['SamplesBeforeNotification'].nil?
                samples_to_keep = HealthEventsConstants::DEFAULT_SAMPLES_BEFORE_NOTIFICATION
            elsif !config.nil? && config['NotifyInstantly']
                samples_to_keep = 1
            else
                samples_to_keep = config['SamplesBeforeNotification'].to_i
            end

            if @@instanceStates.key?(monitor_instance_id)
                health_monitor_instance_state = @@instanceStates[monitor_instance_id]
                health_monitor_records = health_monitor_instance_state.prev_records #This should be an array

                if health_monitor_records.size == samples_to_keep
                    health_monitor_records.delete_at(0)
                end
                health_monitor_records.push(health_monitor_record)
                health_monitor_instance_state.prev_records = health_monitor_records
                @@instanceStates[monitor_instance_id] = health_monitor_instance_state
            else
                health_monitor_instance_state = HealthMonitorInstanceState.new(health_monitor_record.timestamp, health_monitor_record.state, [health_monitor_record])
                @@instanceStates[monitor_instance_id] = health_monitor_instance_state
            end
            log.debug "Health Records Count: #{health_monitor_instance_state.prev_records.size}"
        end

        def getHealthMonitorState(monitor_instance_id)
            return @@instanceStates[monitor_instance_id]
        end

        def setHealthMonitorState(monitor_instance_id, health_monitor_instance_state)
            @@instanceStates[monitor_instance_id] = health_monitor_instance_state
        end

        def getHealthMonitorStatesHash
            return @@instanceStates
        end

        def computeHealthMonitorState(log, monitor_id, value, config)
            log.debug "computeHealthMonitorState"
            #log.info "id: #{monitor_id} value: #{value} config: #{config}"
            case monitor_id
            when HealthEventsConstants::WORKLOAD_CONTAINER_CPU_PERCENTAGE_MONITOR_ID, HealthEventsConstants::WORKLOAD_CONTAINER_MEMORY_PERCENTAGE_MONITOR_ID, HealthEventsConstants::NODE_CPU_MONITOR_ID, HealthEventsConstants::NODE_MEMORY_MONITOR_ID
                return getStateForRangeMonitor(log, value, config)
            when HealthEventsConstants::MANAGEDINFRA_PODS_READY_PERCENTAGE_MONITOR_ID, HealthEventsConstants::WORKLOAD_PODS_READY_PERCENTAGE_MONITOR_ID
                getStateForInfraPodsReadyPercentage(log, value, config)
            end
        end

        def getStateForRangeMonitor(log, value, config)
            log.debug "getStateForRangeMonitor"
            pass_percentage = 0.0
            (config.nil? || config['PassPercentage'].nil?) ? pass_percentage = HealthEventsConstants::DEFAULT_PASS_PERCENTAGE : pass_percentage = config['PassPercentage'].to_f
            (config.nil? || config['FailPercentage'].nil?) ? fail_percentage = HealthEventsConstants::DEFAULT_FAIL_PERCENTAGE : fail_percentage = config['FailPercentage'].to_f
            #log.info "Pass: #{pass_percentage} Fail: #{fail_percentage}"
            if value.to_f < pass_percentage.to_f
                return HEALTH_MONITOR_STATE['PASS']
            elsif value.to_f > fail_percentage.to_f
                return HEALTH_MONITOR_STATE['FAIL']
            else
                return HEALTH_MONITOR_STATE['WARNING']
            end
        end

        def getStateForInfraPodsReadyPercentage(log, value, config)
            log.debug "getStateForInfraPodsReadyPercentage"
            log.debug "getStateForInfraPodsReadyPercentage #{config}"
            (config.nil? || config['PassPercentage'].nil?) ? pass_percentage = HealthEventsConstants::DEFAULT_PASS_PERCENTAGE : pass_percentage = config['PassPercentage'].to_f
            (config.nil? || config['FailPercentage'].nil?) ? fail_percentage = HealthEventsConstants::DEFAULT_FAIL_PERCENTAGE : fail_percentage = config['FailPercentage'].to_f
            log.info " getStateForInfraPodsReadyPercentage Pass: #{pass_percentage} Fail: #{fail_percentage}"
            if value.to_f < pass_percentage.to_f
                return HEALTH_MONITOR_STATE['FAIL']
            else
                return HEALTH_MONITOR_STATE['PASS']
            end
        end

        def getStateForWorkloadPodsReadyPercentage(log, value, config)
            log.debug "getStateForWorkloadPodsReadyPercentage"
            pass_percentage = 0.0
            (config.nil? || config['PassPercentage'].nil?) ? pass_percentage = HealthEventsConstants::DEFAULT_PASS_PERCENTAGE : pass_percentage = config['PassPercentage'].to_f
            (config.nil? || config['FailPercentage'].nil?) ? fail_percentage = HealthEventsConstants::DEFAULT_FAIL_PERCENTAGE : fail_percentage = config['FailPercentage'].to_f
            log.info "getStateForWorkloadPodsReadyPercentage Pass: #{pass_percentage} Fail: #{fail_percentage}"
            if value.to_f > fail_percentage.to_f && value.to_f < pass_percentage.to_f
                return HEALTH_MONITOR_STATE['WARNING']
            elsif value.to_f < fail_percentage.to_f
                return HEALTH_MONITOR_STATE['FAIL']
            elsif value.to_f == pass_percentage.to_f
                return HEALTH_MONITOR_STATE['PASS']
            end
        end
    end
end


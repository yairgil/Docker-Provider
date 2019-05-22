#!/usr/local/bin/ruby
# frozen_string_literal: true

require_relative 'HealthMonitorConstants'

HealthMonitorInstanceState = Struct.new(:prev_sent_record_time, :old_state, :new_state, :state_change_time, :prev_records) do
end

class HealthMonitorState
    @@instanceStates = {} #hash of monitor_instance_id --> health monitor instance state
    @@firstMonitorRecordSent = {}
    #FIXME: use lookup for health_monitor_constants.rb from health folder
    HEALTH_MONITOR_STATE = {"PASS" => "pass", "FAIL" => "fail", "WARNING" => "warn", "NONE" => "none"}

    class << self
        #set new_state to be the latest ONLY if the state change is consistent for monitors that are not configured to be notified instantly, i.e. For NotifyInstantly Monitors, set new state to be the latest
        # record state. For others, set it to be none, if there is no state information present in the lookup table
        def updateHealthMonitorState(log, monitor_instance_id, health_monitor_record, config)
            #log.debug "updateHealthMonitorState"
            samples_to_keep = 1
            if !config.nil? && config['NotifyInstantly'] == true
                samples_to_keep = 1
            elsif !config.nil? && !config['SamplesBeforeNotification'].nil?
                samples_to_keep = config['SamplesBeforeNotification'].to_i
            else
                samples_to_keep = HealthMonitorConstants::DEFAULT_SAMPLES_BEFORE_NOTIFICATION
            end

            #log.debug "Monitor Instance Id #{monitor_instance_id} samples_to_keep #{samples_to_keep}"

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
                # if samples_to_keep == 1, then set new state to be the health_monitor_record state, else set it as none
                old_state = HEALTH_MONITOR_STATE["NONE"]
                new_state = HEALTH_MONITOR_STATE["NONE"]
                if samples_to_keep == 1
                    new_state = health_monitor_record["state"]
                end
                health_monitor_instance_state = HealthMonitorInstanceState.new(health_monitor_record["timestamp"], old_state, new_state, health_monitor_record["timestamp"], [health_monitor_record])
                @@instanceStates[monitor_instance_id] = health_monitor_instance_state
            end
            #log.debug "Health Records Count: #{health_monitor_instance_state.prev_records.size}"
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
            #log.debug "computeHealthMonitorState"
            #log.info "id: #{monitor_id} value: #{value} config: #{config}"
            case monitor_id
            when HealthMonitorConstants::WORKLOAD_CONTAINER_CPU_PERCENTAGE_MONITOR_ID, HealthMonitorConstants::WORKLOAD_CONTAINER_MEMORY_PERCENTAGE_MONITOR_ID, HealthMonitorConstants::NODE_CPU_MONITOR_ID, HealthMonitorConstants::NODE_MEMORY_MONITOR_ID
                return getStateForRangeMonitor(log, value, config)
            when HealthMonitorConstants::MANAGEDINFRA_PODS_READY_PERCENTAGE_MONITOR_ID, HealthMonitorConstants::WORKLOAD_PODS_READY_PERCENTAGE_MONITOR_ID
                getStateForInfraPodsReadyPercentage(log, value, config)
            end
        end

        def getStateForRangeMonitor(log, value, config)
            #log.debug "getStateForRangeMonitor"
            pass_percentage = 0.0
            (config.nil? || config['PassPercentage'].nil?) ? pass_percentage = HealthMonitorConstants::DEFAULT_PASS_PERCENTAGE : pass_percentage = config['PassPercentage'].to_f
            (config.nil? || config['FailPercentage'].nil?) ? fail_percentage = HealthMonitorConstants::DEFAULT_FAIL_PERCENTAGE : fail_percentage = config['FailPercentage'].to_f
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
            # log.debug "getStateForInfraPodsReadyPercentage"
            # log.debug "getStateForInfraPodsReadyPercentage #{config}"
            (config.nil? || config['PassPercentage'].nil?) ? pass_percentage = HealthMonitorConstants::DEFAULT_PASS_PERCENTAGE : pass_percentage = config['PassPercentage'].to_f
            (config.nil? || config['FailPercentage'].nil?) ? fail_percentage = HealthMonitorConstants::DEFAULT_FAIL_PERCENTAGE : fail_percentage = config['FailPercentage'].to_f
            # log.info " getStateForInfraPodsReadyPercentage Pass: #{pass_percentage} Fail: #{fail_percentage}"
            if value.to_f < pass_percentage.to_f
                return HEALTH_MONITOR_STATE['FAIL']
            else
                return HEALTH_MONITOR_STATE['PASS']
            end
        end

        def getStateForWorkloadPodsReadyPercentage(log, value, config)
            # log.debug "getStateForWorkloadPodsReadyPercentage"
            pass_percentage = 0.0
            (config.nil? || config['PassPercentage'].nil?) ? pass_percentage = HealthMonitorConstants::DEFAULT_PASS_PERCENTAGE : pass_percentage = config['PassPercentage'].to_f
            (config.nil? || config['FailPercentage'].nil?) ? fail_percentage = HealthMonitorConstants::DEFAULT_FAIL_PERCENTAGE : fail_percentage = config['FailPercentage'].to_f
            #log.info "getStateForWorkloadPodsReadyPercentage Pass: #{pass_percentage} Fail: #{fail_percentage}"
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


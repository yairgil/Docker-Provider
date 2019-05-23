#!/usr/local/bin/ruby
# frozen_string_literal: true

require 'time'
require 'json'
require_relative 'KubernetesApiClient'
require_relative 'health/health_model_constants'

class HealthMonitorSignalReducer

    @@firstMonitorRecordSent = {}
    class << self
        def reduceSignal(log, monitor_id, monitor_instance_id, monitor_config, key: nil, node_name: nil)

            health_monitor_instance_state = HealthMonitorState.getHealthMonitorState(monitor_instance_id)
            health_monitor_records = health_monitor_instance_state.prev_records
            new_state = health_monitor_instance_state.new_state
            prev_sent_time = health_monitor_instance_state.prev_sent_record_time
            time_first_observed = health_monitor_instance_state.state_change_time
            monitor_config['MonitorTimeOut'].nil? ? monitor_timeout = HealthMonitorConstants::DEFAULT_MONITOR_TIMEOUT : monitor_timeout = monitor_config['MonitorTimeOut'] #minutes
            #log.debug monitor_timeout

            # Notify Instantly sends a signal immediately on a state change
            if (!monitor_config['NotifyInstantly'].nil? && monitor_config['NotifyInstantly'] == true)
                latest_record = health_monitor_records[health_monitor_records.size-1] #since we push new records to the end, and remove oldest records from the beginning
                latest_record_state = latest_record["state"]
                latest_record_time = latest_record["timestamp"] #string representation of time
                #log.debug "Latest Record #{latest_record}"
                if latest_record_state.downcase == new_state.downcase && @@firstMonitorRecordSent.key?(monitor_instance_id) #no state change
                    #log.debug "latest_record_state.to_s.downcase == prev_sent_status.to_s.state"
                    time_elapsed = (Time.parse(latest_record_time) - Time.parse(prev_sent_time)) / 60
                    #log.debug "time elapsed #{time_elapsed}"
                    if time_elapsed > monitor_timeout # minutes
                        # update record for last sent record time
                        health_monitor_instance_state.old_state = health_monitor_instance_state.new_state
                        health_monitor_instance_state.new_state = latest_record_state
                        health_monitor_instance_state.prev_sent_record_time = latest_record_time
                        #log.debug "After Updating Monitor State #{health_monitor_instance_state}"
                        HealthMonitorState.setHealthMonitorState(monitor_instance_id, health_monitor_instance_state)
                        return formatRecord(log, monitor_id, monitor_instance_id, health_monitor_instance_state, monitor_config, key: key, node_name: node_name)
                    else
                        #log.debug "Monitor timeout not reached #{time_elapsed}"
                        #log.debug "Timeout not reached for #{monitor_id}"
                        return nil# dont send anything
                    end
                else
                    health_monitor_instance_state.old_state = health_monitor_instance_state.new_state #initially old = new, so when state change occurs, assign old to be new, and set new to be the latest record state
                    health_monitor_instance_state.new_state = latest_record_state
                    health_monitor_instance_state.state_change_time = latest_record_time
                    health_monitor_instance_state.prev_sent_record_time = latest_record_time
                    HealthMonitorState.setHealthMonitorState(monitor_instance_id, health_monitor_instance_state)
                    return formatRecord(log, monitor_id, monitor_instance_id, health_monitor_instance_state, monitor_config, node_name: node_name)
                end
            end

            #FIXME: if record count = 1, then send it, if it is greater than 1 and less than SamplesBeforeNotification, NO-OP. If equal to SamplesBeforeNotification, then check for consistency in state change
            if health_monitor_instance_state.prev_records.size == 1
                #log.debug "Only One Record"
                return formatRecord(log, monitor_id, monitor_instance_id, health_monitor_instance_state, monitor_config, key: key, node_name: node_name)
            elsif health_monitor_instance_state.prev_records.size < monitor_config["SamplesBeforeNotification"].to_i
                log.debug "Prev records size < SamplesBeforeNotification for #{monitor_instance_id}"
                return nil
            else
                first_record = health_monitor_records[0]
                latest_record = health_monitor_records[health_monitor_records.size-1] #since we push new records to the end, and remove oldest records from the beginning
                latest_record_state = latest_record["state"]
                latest_record_time = latest_record["timestamp"] #string representation of time
                #log.debug "Latest Record #{latest_record}"
                if latest_record_state.downcase == new_state.downcase # No state change
                    #log.debug "latest_record_state.to_s.downcase == prev_sent_status.to_s.state"
                    time_elapsed = (Time.parse(latest_record_time) - Time.parse(prev_sent_time)) / 60 #check if more than monitor timeout for signal
                    #log.debug "time elapsed #{time_elapsed}"
                    if time_elapsed > monitor_timeout # minutes
                        # update record
                        health_monitor_instance_state.old_state = health_monitor_instance_state.new_state
                        health_monitor_instance_state.new_state = latest_record_state
                        health_monitor_instance_state.prev_sent_record_time = latest_record_time
                        #log.debug "After Updating Monitor State #{health_monitor_instance_state}"
                        HealthMonitorState.setHealthMonitorState(monitor_instance_id, health_monitor_instance_state)
                        return formatRecord(log, monitor_id, monitor_instance_id, health_monitor_instance_state, monitor_config, key: key, node_name: node_name)
                    else
                        #log.debug "Monitor timeout not reached #{time_elapsed}"
                        #log.debug "Timeout not reached for #{monitor_id}"
                        return nil# dont send anything
                    end
                else # state change from previous sent state to latest record state
                    #check state of last n records to see if they are all in the same state
                    if (isStateChangeConsistent(log, health_monitor_records))
                        health_monitor_instance_state.old_state = health_monitor_instance_state.new_state
                        health_monitor_instance_state.new_state = latest_record_state
                        health_monitor_instance_state.prev_sent_record_time = latest_record_time
                        health_monitor_instance_state.state_change_time = first_record["timestamp"]
                        HealthMonitorState.setHealthMonitorState(monitor_instance_id, health_monitor_instance_state)
                        return formatRecord(log, monitor_id, monitor_instance_id, health_monitor_instance_state, monitor_config, key: key, node_name: node_name)
                    else
                        log.debug "No consistent state change for monitor #{monitor_id}"
                        return nil
                    end
                end
            end
            log.debug "No new information for monitor #{monitor_id}"
            return nil
        end

        def formatRecord(log, monitor_id, monitor_instance_id, health_monitor_instance_state, monitor_config, key: nil, node_name: nil)
            log.debug "Health Monitor Instance State #{health_monitor_instance_state}"

            labels = HealthMonitorUtils.getClusterLabels
            #log.debug "Labels : #{labels}"

            namespace = health_monitor_instance_state.prev_records[0]['details']['namespace']
            pod_aggregator = health_monitor_instance_state.prev_records[0]['details']['podAggregator']
            pod_aggregator_kind = health_monitor_instance_state.prev_records[0]['details']['podAggregatorKind']

            monitor_labels = HealthMonitorUtils.getMonitorLabels(log, monitor_id, key: key, pod_aggregator: pod_aggregator, node_name: node_name, namespace: namespace, pod_aggregator_kind: pod_aggregator_kind)
            #log.debug "Monitor Labels : #{monitor_labels}"

            if !monitor_labels.nil?
                monitor_labels.keys.each do |key|
                    labels[key] = monitor_labels[key]
                end
            end

            #log.debug "Labels after adding Monitor Labels #{labels}"
            prev_records = health_monitor_instance_state.prev_records
            time_first_observed = health_monitor_instance_state.state_change_time # the oldest collection time
            new_state = health_monitor_instance_state.new_state # this is updated before formatRecord is called
            old_state = health_monitor_instance_state.old_state

            #log.debug "monitor_config  #{monitor_config}"
            if monitor_config.nil?
                monitor_config = ''
            end
            monitor_config = monitor_config
            #log.debug "monitor_config  #{monitor_config}"
            records = []

            details = prev_records #.each do |record|

            time_observed = Time.now.utc.iso8601
            #log.debug "Details: #{details}"
            #log.debug "time_first_observed #{time_first_observed} time_observed #{time_observed} new_state #{new_state} old_state #{old_state}"

            health_monitor_record = {}
            health_monitor_record["ClusterId"] = KubernetesApiClient.getClusterId
            health_monitor_record["ClusterName"] = KubernetesApiClient.getClusterName
            health_monitor_record["MonitorLabels"] = labels.to_json
            health_monitor_record["MonitorId"] = monitor_id
            health_monitor_record["MonitorInstanceId"] = monitor_instance_id
            health_monitor_record["NewState"] = new_state
            health_monitor_record["OldState"] = old_state
            health_monitor_record["Details"] = details
            health_monitor_record["MonitorConfig"] = monitor_config.to_json
            health_monitor_record["AgentCollectionTime"] = Time.now.utc.iso8601
            health_monitor_record["TimeFirstObserved"] = time_first_observed

            #log.debug "HealthMonitor Record #{health_monitor_record}"
            #log.debug "Parsed Health Monitor Record for #{monitor_id}"

            if !@@firstMonitorRecordSent.key?(monitor_instance_id)
                @@firstMonitorRecordSent[monitor_instance_id] = true
            end

            return health_monitor_record
        end

        #FIXME: check for consistency for "SamplesBeforeNotification" records
        def isStateChangeConsistent(log, health_monitor_records)
            if health_monitor_records.nil? || health_monitor_records.size == 0
                return false
            end
            i = 0
            while i < health_monitor_records.size - 1
                #log.debug "Prev: #{health_monitor_records[i].state} Current: #{health_monitor_records[i + 1].state}"
                if health_monitor_records[i]["state"] != health_monitor_records[i + 1]["state"]
                    return false
                end
                i += 1
            end
            return true
        end
    end
end
#!/usr/local/bin/ruby
# frozen_string_literal: true

require 'time'
require 'json'
require_relative 'HealthEventTemplates'

class HealthSignalReducer

    @@firstMonitorRecordSent = {}
    class << self
        def reduceSignal(log, monitor_id, monitor_instance_id, monitor_config, key: nil, controller_name: nil, node_name: nil)
            #log.debug "reduceSignal Key : #{key} controller_name: #{controller_name} node_name #{node_name}"
            #log.debug "monitorConfig #{monitor_config}"

            health_monitor_instance_state = HealthMonitorState.getHealthMonitorState(monitor_instance_id)
            #log.info "Health Monitor Instance state #{health_monitor_instance_state}"
            health_monitor_records = health_monitor_instance_state.prev_records
            prev_sent_status = health_monitor_instance_state.prev_sent_record_status
            prev_sent_time = health_monitor_instance_state.prev_sent_record_time
            monitor_config['MonitorTimeOut'].nil? ? monitor_timeout = HealthEventsConstants::DEFAULT_MONITOR_TIMEOUT : monitor_timeout = monitor_config['MonitorTimeOut'] #minutes
            #log.info monitor_timeout


            if (!monitor_config['NotifyInstantly'].nil? && monitor_config['NotifyInstantly'] == true)
                latest_record = health_monitor_records[health_monitor_records.size-1] #since we push new records to the end, and remove oldest records from the beginning
                latest_record_state = latest_record["state"]
                latest_record_time = latest_record["timestamp"] #string representation of time
                #log.info "Latest Record #{latest_record}"
                if latest_record_state.downcase == prev_sent_status.downcase && @@firstMonitorRecordSent.key?(monitor_id)
                    #log.info "latest_record_state.to_s.downcase == prev_sent_status.to_s.state"
                    time_elapsed = (Time.parse(latest_record_time) - Time.parse(prev_sent_time)) / 60
                    #log.info "time elapsed #{time_elapsed}"
                    if time_elapsed > monitor_timeout # minutes
                        # update record
                        health_monitor_instance_state.prev_sent_record_time = latest_record_time
                        health_monitor_instance_state.prev_sent_record_status = latest_record_state
                        #log.info "After Updating Monitor State #{health_monitor_instance_state}"
                        HealthMonitorState.setHealthMonitorState(monitor_instance_id, health_monitor_instance_state)
                        return formatRecord(log, monitor_id, monitor_instance_id, health_monitor_instance_state, monitor_config, node_name: node_name)
                    else
                        #log.info "Monitor timeout not reached #{time_elapsed}"
                        #log.info "Timeout not reached for #{monitor_id}"
                        return nil# dont send anything
                    end
                else
                    return formatRecord(log, monitor_id, monitor_instance_id, health_monitor_instance_state, monitor_config, node_name: node_name)
                end
            end

            if health_monitor_instance_state.prev_records.size == 1
                #log.info "Only One Record"
                return formatRecord(log, monitor_id, monitor_instance_id, health_monitor_instance_state, monitor_config, key: key, controller_name: controller_name, node_name: node_name)
            else
                latest_record = health_monitor_records[health_monitor_records.size-1] #since we push new records to the end, and remove oldest records from the beginning
                latest_record_state = latest_record["state"]
                latest_record_time = latest_record["timestamp"] #string representation of time
                #log.info "Latest Record #{latest_record}"
                if latest_record_state.downcase == prev_sent_status.downcase
                    #log.info "latest_record_state.to_s.downcase == prev_sent_status.to_s.state"
                    time_elapsed = (Time.parse(latest_record_time) - Time.parse(prev_sent_time)) / 60
                    #log.info "time elapsed #{time_elapsed}"
                    if time_elapsed > monitor_timeout # minutes
                        # update record
                        health_monitor_instance_state.prev_sent_record_time = latest_record_time
                        health_monitor_instance_state.prev_sent_record_status = latest_record_state
                        #log.info "After Updating Monitor State #{health_monitor_instance_state}"
                        HealthMonitorState.setHealthMonitorState(monitor_instance_id, health_monitor_instance_state)
                        return formatRecord(log, monitor_id, monitor_instance_id, health_monitor_instance_state, monitor_config, key: key, controller_name: controller_name)
                    else
                        #log.info "Monitor timeout not reached #{time_elapsed}"
                        #log.info "Timeout not reached for #{monitor_id}"
                        return nil# dont send anything
                    end
                else # state change from previous sent state to latest record state
                    #check state of last n records to see if they are all in the same state
                    if (isStateChangeConsistent(log, health_monitor_records))
                        return formatRecord(log, monitor_id, monitor_instance_id, health_monitor_instance_state, monitor_config, key: key, controller_name: controller_name, node_name: node_name)
                    else
                        log.debug "No consistent state change for monitor #{monitor_id}"
                        return nil
                    end
                end
            end
            log.debug "No new information for monitor #{monitor_id}"
            return nil
        end

        def formatRecord(log, monitor_id, monitor_instance_id, health_monitor_instance_state, monitor_config, key: nil, controller_name: nil, node_name: nil)
            #log.debug "formatRecord key:#{key} controller_name: #{controller_name} node_name #{node_name}"

            #log.debug "Health Monitor Instance State #{health_monitor_instance_state}"

            labels = HealthEventUtils.getClusterLabels
            #log.info "Labels : #{labels}"

            monitor_labels = HealthEventUtils.getMonitorLabels(log, monitor_id, key, controller_name, node_name)
            #log.info "Monitor Labels : #{monitor_labels}"

            if !monitor_labels.nil?
                monitor_labels.keys.each do |key|
                    labels[key] = monitor_labels[key]
                end
            end

            #log.debug "Labels #{labels.to_json.to_s}"
            prev_records = health_monitor_instance_state.prev_records
            collection_time = prev_records[0]["timestamp"] # the oldest collection time
            new_state = health_monitor_instance_state.prev_records[0]["state"]
            old_state = health_monitor_instance_state.prev_sent_record_status

            #log.debug "monitor_config  #{monitor_config}"
            if monitor_config.nil?
                monitor_config = ''
            end
            monitor_config = monitor_config
            #log.debug "monitor_config  #{monitor_config}"
            records = []

             details = prev_records #.each do |record|

            #     hash_record =  { "timestamp" => record.timestamp, "state" => record.state, "details" => record.details}
            #     #log.debug "Hash from Struct #{hash_record}"
            #     #log.debug "monitor_config #{monitor_config}"
            #     records.push(hash_record.to_json.to_s)
            # end
            # details = "[#{records.join(',')}]"
            time_observed = Time.now.utc.iso8601
            #log.debug "Details: #{details}"
            #log.debug "collection_time #{collection_time} time_observed #{time_observed} new_state #{new_state} old_state #{old_state}"

            # health_monitor_record = HealthEventTemplates::HealthRecordTemplate % {
            #     labels: labels,
            #     monitor_id: monitor_id,
            #     monitor_instance_id: monitor_instance_id,
            #     new_state: new_state,
            #     old_state: old_state,
            #     monitor_details: details,
            #     collection_time: collection_time,
            #     time_observed: time_observed,
            #     monitor_config: monitor_config
            # }
            # HealthRecordTemplate = '{
            #     "Labels": %{labels},
            #     "MonitorId": "%{monitor_id}",
            #     "MonitorInstanceId": "%{monitor_instance_id}",
            #     "NewState": "%{new_state}",
            #     "OldState": "%{old_state}",
            #     "Details": %{monitor_details},
            #     "MonitorConfig": %{monitor_config},
            #     "CollectionTime": "%{collection_time}",
            #     "TimeObserved": "%{time_observed}"
            # }'
            health_monitor_record = {}
            health_monitor_record["MonitorLabels"] = labels.to_json
            health_monitor_record["MonitorId"] = monitor_id
            health_monitor_record["MonitorInstanceId"] = monitor_instance_id
            health_monitor_record["NewState"] = new_state
            health_monitor_record["OldState"] = old_state
            health_monitor_record["Details"] = details
            health_monitor_record["MonitorConfig"] = monitor_config.to_json
            health_monitor_record["CollectionTime"] = collection_time
            health_monitor_record["TimeObserved"] = time_observed


            #log.debug "HealthMonitor Record #{health_monitor_record}"
            #return_val = JSON.parse(health_monitor_record)
            #log.debug "Parsed Health Monitor Record for #{monitor_id}"

            if !@@firstMonitorRecordSent.key?(monitor_id)
                @@firstMonitorRecordSent[monitor_id] = true
            end

            return health_monitor_record
        end

        def isStateChangeConsistent(log, health_monitor_records)
            if health_monitor_records.nil? || health_monitor_records.size == 0
                return false
            end
            i = 0
            while i < health_monitor_records.size - 1
                #log.info "Prev: #{health_monitor_records[i].state} Current: #{health_monitor_records[i + 1].state}"
                if health_monitor_records[i]["state"] != health_monitor_records[i + 1]["state"]
                    return false
                end
                i += 1
            end
            return true
        end
    end
end
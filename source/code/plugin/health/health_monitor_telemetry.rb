require_relative 'health_model_constants'
require_relative '../ApplicationInsightsUtility'

module HealthModel
    class HealthMonitorTelemetry

        attr_reader :monitor_records

        def send
            log = HealthMonitorHelpers.get_log_handle
            log.info "Sending #{@monitor_records.size} state change events"
            @monitor_records.each{|record|
                ApplicationInsightsUtility.sendCustomEvent("HealthMonitorStateChangeEvent", {
                    "MonitorId" => record['monitor_id'],
                    "OldState" => record['old_state'],
                    "NewState" => record['new_state']
                })
            }
            @monitor_records = []
        end

        def add_monitor_to_telemetry(monitor_id, old_state, new_state)
            if @monitor_records.nil? || @monitor_records.empty?
                @monitor_records = []
            end
            record = {
                "monitor_id" => monitor_id,
                "old_state" => old_state,
                "new_state" => new_state
            }
            @monitor_records.push(record)
        end
    end
end
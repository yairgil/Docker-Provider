HealthMonitorRecord = Struct.new(
    :monitor_id,
    :monitor_instance_id,
    :transition_date_time,
    :operational_state,
    :labels,
    :config,
    :details
    ) do
end
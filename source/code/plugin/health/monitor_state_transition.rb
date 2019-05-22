MonitorStateTransition = Struct.new(
    :monitor_id,
    :monitor_instance_id,
    :transition_date_time,
    :old_state,
    :new_state,
    :labels,
    :config,
    :details
    ) do
end
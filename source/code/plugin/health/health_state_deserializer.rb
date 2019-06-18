module HealthModel
    class HealthStateDeserializer

        attr_reader :deserialize_path

        def initialize(path)
            @deserialize_path = path
        end

        def deserialize
            if !File.file?(@deserialize_path)
                return {}
            end

            file = File.read(@deserialize_path) #File.read(@deserialize_path)

            deserialized_state = {}
            if !file.nil? || !file.empty?
                records = JSON.parse(file)

                records.each{|monitor_instance_id, health_monitor_instance_state_hash|
                    state = HealthMonitorInstanceState.new(*health_monitor_instance_state_hash.values_at(*HealthMonitorInstanceState.members))
                    state.prev_sent_record_time = health_monitor_instance_state_hash["prev_sent_record_time"]
                    state.old_state = health_monitor_instance_state_hash["old_state"]
                    state.new_state = health_monitor_instance_state_hash["new_state"]
                    state.state_change_time = health_monitor_instance_state_hash["state_change_time"]
                    state.prev_records = health_monitor_instance_state_hash["prev_records"]
                    state.is_state_change_consistent = health_monitor_instance_state_hash["is_state_change_consistent"] || false
                    state.should_send = health_monitor_instance_state_hash["should_send"]
                    deserialized_state[monitor_instance_id] = state
                }
                return deserialized_state
            end
        end
    end
end
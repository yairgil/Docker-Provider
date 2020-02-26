# frozen_string_literal: true
require 'time'
require 'json'
require_relative 'health_model_constants'

module HealthModel
    class HealthMonitorVersions
        attr_reader :versions_hash, :state
        def initialize
            @versions_hash = {}
        end

        def initialize_versions(state)
            @state = state
            @state.keys.each{|monitor_instance_id|
                @versions_hash[monitor_instance_id] = @state[monitor_instance_id].monitor_version.to_i
            }
        end

        def set_monitor_version(monitor_instance_id, version)
            @versions_hash[monitor_instance_id] = version
        end

        def get_monitor_version(monitor_instance_id)
            return @versions_hash[monitor_instance_id]
        end

        def get_current_monitor_versions
            return @versions_hash
        end

        def get_current_monitor_versions_hash
            hash_array = []
            @versions_hash.sort.to_h.keys.each{|k|
                    hash_array.push({k => @versions_hash[k]})
            }
            json_to_hash = hash_array.to_json
            hash_json = Digest::SHA256.hexdigest(json_to_hash)
            $log.info "HashSize: #{@versions_hash.keys.size}"
            $log.info "HashJSON: #{hash_json}"
            return hash_json
        end

        def remove_none_state_monitors
            @versions_hash.keys.each{|k|
                if @state.key?(k) && @state[k].new_state.downcase == HealthMonitorStates::NONE
                    @versions_hash.delete(k)
                    $log.info "Removed monitor #{k} from versions_hash"
                end
            }
        end
    end
end
require_relative 'health_model_constants'


begin
    if !Gem.win_platform?
        require '/opt/tomlrb'
    else
        require 'tomlrb' #this assumes the tomlrb gem is installed in your dev environment
    end
rescue => e
    ApplicationinsightsUtility.sendExceptionTelemetry(e, {"FeatureArea" => "Health"})
end

class HealthMonitorConfigProvider
    @@DEFAULT_CONFIG_FILE_PATH = '/etc/opt/microsoft/docker-cimprov/health/healthmonitorconfig.toml'
    @@OVERRIDE_CONFIG_FILE_PATH = '/etc/config/settings/agent-settings'
    @@CONFIG_VALUE_TYPE_MAP = {
        'warn_percentage' => [0.0.class,0.class],
        'fail_percentage' => [0.0.class,0.class],
        'enabled' => [true.class,false.class],
        'samples_to_evaluate' => 1.class,
        'pods_not_ready_warn_percentage' => [0.0.class,0.class],
        'pods_not_ready_fail_percentage' => [0.0.class,0.class],
        'failure_node_conditions' => ''.class
    }

    attr_reader :merged_configuration

    def initialize(default_config_file_path = nil, override_config_file_path = nil)
        @log = HealthMonitorHelpers.get_log_handle
        if default_config_file_path.nil?
            default_config_file_path = @@DEFAULT_CONFIG_FILE_PATH
        end

        if File.exist?(default_config_file_path)
            default_config = Tomlrb.load_file(default_config_file_path)['agent_settings']['health_model']['configurations']
            # the merging will modify the default configuration. So create a copy. Marshal is the only deep copy technique that works for nested hashes
            default_config_copy = deep_copy(default_config)
        else
            raise "File not Found #{default_config_file_path}"
        end

        if override_config_file_path.nil?
            override_config_file_path = @@OVERRIDE_CONFIG_FILE_PATH
        end

        if File.exist?(override_config_file_path)
            override_config = Tomlrb.load_file(override_config_file_path)['agent_settings']['health_model']['configurations']
        else
            override_config = {}
        end

        unless override_config.nil?
            @merged_configuration = override_config.merge!(default_config){|key, o, d|
                if o.is_a?(Hash) && d.is_a?(Hash)
                    d.merge!(o) {|k, original, override | # if both default and override have the same keys, take the override value
                    override
                    }
                end
            }
        end

        @merged_configuration.keys.each{|monitor_id|
            # check for validity of each configuration. If any of them is not valid, take the default configuration.
            # Only type checking is done. Logical Validation is skipped e.g. warn_percentage > fail_percentage
            if !configuration_valid?(@merged_configuration[monitor_id], monitor_id)
                #TODO: send telemetry for config parsing error
                @log.info "Invalid Config specified for #{monitor_id}. Applying default config"
                @merged_configuration[monitor_id] = default_config_copy[monitor_id]
            end
        }
    end

    def get_all_configurations
        return @merged_configuration
    end

    def get_monitor_configuration(monitor_id, monitor_labels)
        if !@merged_configuration.key?(monitor_id)
            #TODO: Should this be sent to AppInsights?
            raise "Invalid MonitorId #{monitor_id} specified"
        end

        #get the configuration for the monitor_id
        monitor_configuration = @merged_configuration[monitor_id]

        # if the configuration has no 'labels' key, return the config. there are no label overrides present for the monitor
        if !monitor_configuration.key?('labels')
            return monitor_configuration
        end

        monitor_labels_array = monitor_labels.map{|k,v|
                "#{k}=#{v}"
        }

        monitor_configuration['labels'].each{|match_labels,label_configuration|
            match_labels_array = match_labels.split(',')
            delta = match_labels_array - monitor_labels_array #[this basically removes intersecting elements from the match_labels_array that are in monitor_labels_array]
            if delta.empty? #if empty, it means all the match_labels are present in the monitor_labels_array
                #TODO: merge missing configurations with default configurations before returning
                return monitor_configuration['labels'][match_labels]
            else
                # all the labels do not match. Continue looking at other labels for matches with the monitor labels
                next
            end
        }

        #if none of them match, then return the default configuration for the monitor
        return monitor_configuration
    end

    private
    def configuration_valid?(configuration, monitor_id)
        configuration.keys.each {|key|
            case key
            when 'labels'
                configuration['labels'].keys.each {|label_overrides|
                    if !configuration_valid?(configuration['labels'][label_overrides], label_overrides)
                        return false
                    end
                }
            when 'samples_to_evaluate'
            when 'failure_node_conditions'
                config_value_type = configuration[key].class
                if @@CONFIG_VALUE_TYPE_MAP[key] != config_value_type
                    @log.info "Config Value Type mismatch for #{key} Expected: #{@@CONFIG_VALUE_TYPE_MAP[key]} Actual: #{config_value_type}"
                    puts "Config Value Type mismatch for #{key} Expected: #{@@CONFIG_VALUE_TYPE_MAP[key]} Actual: #{config_value_type}"
                    return false
                end
            else
                config_value_type = configuration[key].class
                if !@@CONFIG_VALUE_TYPE_MAP[key].include?(config_value_type)
                    @log.info "Config Value Type mismatch for #{key} Expected: #{@@CONFIG_VALUE_TYPE_MAP[key]} Actual: #{config_value_type}"
                    puts "Config Value Type mismatch for #{key} Expected: #{@@CONFIG_VALUE_TYPE_MAP[key]} Actual: #{config_value_type}"
                    return false
                end
            end
        }
        return true
    end

    def deep_copy(o)
        Marshal.load(Marshal.dump(o))
    end
end
module HealthModel
    # static class that provides a bunch of utility methods
    class HealthMonitorUtils

        @@node_inventory = []

        class << self
            # compute the percentage state given a value and a monitor configuration
            def compute_percentage_state(value, config)
                (config.nil? || config['WarnThresholdPercentage'].nil?) ? warn_percentage = nil : config['WarnThresholdPercentage'].to_f
                fail_percentage = config['FailThresholdPercentage'].to_f

                if value > fail_percentage
                    return HealthMonitorState::FAIL
                elsif !warn_percentage.nil? && value > warn_percentage
                    return HealthMonitorState::WARNING
                else
                    return HealthMonitorStatePASS
                end
            end
        end
    end
end
require_relative '../test_helpers'
# consider doing this in test_helpers.rb so that this code is common
Dir[File.join(File.expand_path(File.dirname(__FILE__)), "../../../../source/code/plugin/health/*.rb")].reject{|f| f.include?('health_monitor_utils')}.each { |file| require file }
include HealthModel

#TODO: Unit Tests
describe "HealthMonitorConfigProvider spec " do
    it "computes the config correctly given default and overrides" do
        #arrange

        provider = HealthMonitorConfigProvider.new(File.join(__dir__, '../../../../installer/conf/healthmonitorconfig.toml'), File.join(__dir__,'config','override_config.toml') )
        #act
        puts JSON.pretty_generate(provider.get_all_configurations)

        # monitor_config = provider.get_monitor_configuration('node_memory_utilization', {'kubernetes.io/hostname' => 'node1','agentpool' => 'pool2'})
        # puts monitor_config

        monitor_config = provider.get_monitor_configuration('node_memory_utilization', {'kubernetes.io/hostname' => 'node2','agentpool' => 'pool2'})
        puts monitor_config
    end

    it "returns default config when the override path is not present" do
    #     #arrange
    #     provider = HealthMonitorConfigProvider.new(File.join(__dir__, '../../../../installer/conf/healthmonitorconfig.toml'), File.join(__dir__,'config','override_config_1.toml') )
    #     #act
    #     puts JSON.pretty_generate(provider.get_all_configurations)
    #     puts ''
    end

    it "throws when the default config and override config is not present" do
    end

    it "returns default config for an object when override config is invalid" do
    end

    it "returns default monitor config when there is no labels match" do

    end

    it "returns the overridden monitor config when there is a labels match" do

    end



end
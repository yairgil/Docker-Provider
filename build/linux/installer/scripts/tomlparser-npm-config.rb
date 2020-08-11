#!/usr/local/bin/ruby

#this should be require relative in Linux and require in windows, since it is a gem install on windows
@os_type = ENV["OS_TYPE"]
if !@os_type.nil? && !@os_type.empty? && @os_type.strip.casecmp("windows") == 0
  require "tomlrb"
else
  require_relative "tomlrb"
end

require_relative "ConfigParseErrorLogger"

@configMapMountPath = "/etc/config/settings/integrations"
@configSchemaVersion = ""
@collect_basic_npm_metrics = false
@collect_advanced_npm_metrics = false
@npm_node_url="http://$NODE_IP:10091/node-metrics"
@npm_cluster_url="http://npm-metrics-cluster-service.kube-system:9000/cluster-metrics"
@npm_basic_drop_metrics_cluster = "npm_ipset_counts"

# Use parser to parse the configmap toml file to a ruby structure
def parseConfigMap
  begin
    # Check to see if config map is created
    if (File.file?(@configMapMountPath))
      puts "config::configmap container-azm-ms-agentconfig for npm metrics found, parsing values"
      parsedConfig = Tomlrb.load_file(@configMapMountPath, symbolize_keys: true)
      puts "config::Successfully parsed mounted config map for npm metrics"
      return parsedConfig
    else
      puts "config::configmap container-azm-ms-agentconfig for npm metrics not mounted, using defaults"
      return nil
    end
  rescue => errorStr
    ConfigParseErrorLogger.logError("Exception while parsing config map for npm metrics: #{errorStr}, using defaults, please check config map for errors")
    return nil
  end
end

# Use the ruby structure created after config parsing to set the right values to be used as environment variables
def populateSettingValuesFromConfigMap(parsedConfig)
  begin
    if !parsedConfig.nil? && !parsedConfig[:integrations].nil? && !parsedConfig[:integrations][:azure_network_policy_manager].nil? && !parsedConfig[:integrations][:azure_network_policy_manager][:collect_advanced_metrics].nil?
        advanced_npm_metrics = parsedConfig[:integrations][:azure_network_policy_manager][:collect_advanced_metrics].to_s
        puts "got:integrations.azure_network_policy_manager.collect_advanced_metrics='#{advanced_npm_metrics}'"
        if !advanced_npm_metrics.nil? && advanced_npm_metrics.strip.casecmp("true") == 0
            @collect_advanced_npm_metrics = true
        else
            @collect_advanced_npm_metrics = false
        end
        puts "set:integrations.azure_network_policy_manager.collect_advanced_metrics=#{@collect_advanced_npm_metrics}"
    end
  rescue => errorStr
    puts "config::error:Exception while reading config settings for npm advanced setting - #{errorStr}, using defaults"
    @collect_advanced_npm_metrics = false
  end
  begin
    if !parsedConfig.nil? && !parsedConfig[:integrations].nil? && !parsedConfig[:integrations][:azure_network_policy_manager].nil? && !parsedConfig[:integrations][:azure_network_policy_manager][:collect_basic_metrics].nil?
        basic_npm_metrics = parsedConfig[:integrations][:azure_network_policy_manager][:collect_basic_metrics].to_s
        puts "got:integrations.azure_network_policy_manager.collect_basic_metrics='#{basic_npm_metrics}'"
        if !basic_npm_metrics.nil? && basic_npm_metrics.strip.casecmp("true") == 0
            @collect_basic_npm_metrics = true
        else
            @collect_basic_npm_metrics = false
        end
        puts "set:integrations.azure_network_policy_manager.collect_basic_metrics=#{@collect_basic_npm_metrics}"
    end
  rescue => errorStr
    puts "config::error:Exception while reading config settings for npm basic setting - #{errorStr}, using defaults"
    @collect_basic_npm_metrics = false
  end
end

@configSchemaVersion = ENV["AZMON_AGENT_CFG_SCHEMA_VERSION"]
puts "****************Start Config Processing********************"
if !@configSchemaVersion.nil? && !@configSchemaVersion.empty? && @configSchemaVersion.strip.casecmp("v1") == 0 #note v1 is the only supported schema version , so hardcoding it
  configMapSettings = parseConfigMap
  if !configMapSettings.nil?
    populateSettingValuesFromConfigMap(configMapSettings)
  end
else
  if (File.file?(@configMapMountPath))
    ConfigParseErrorLogger.logError("config::unsupported/missing config schema version - '#{@configSchemaVersion}' , using defaults, please use supported schema version")
  end
  @collect_basic_npm_metrics = false
  @collect_advanced_npm_metrics = false
end

# Write the settings to file, so that they can be set as environment variables
file = File.open("integration_npm_config_env_var", "w")

if !file.nil?
  if @collect_advanced_npm_metrics == true 
    file.write("export TELEMETRY_NPM_INTEGRATION_METRICS_ADVANCED=1\n")
    file.write("export AZMON_INTEGRATION_NPM_METRICS_URL_LIST_NODE=#{@npm_node_url}\n")
    file.write("export AZMON_INTEGRATION_NPM_METRICS_URL_LIST_CLUSTER=#{@npm_cluster_url}\n")
    file.write("export AZMON_INTEGRATION_NPM_METRICS_DROP_LIST_CLUSTER=\n")
  elsif @collect_basic_npm_metrics == true
    file.write("export TELEMETRY_NPM_INTEGRATION_METRICS_BASIC=1\n")
    file.write("export AZMON_INTEGRATION_NPM_METRICS_URL_LIST_NODE=#{@npm_node_url}\n")
    file.write("export AZMON_INTEGRATION_NPM_METRICS_URL_LIST_CLUSTER=#{@npm_cluster_url}\n")
    file.write("export AZMON_INTEGRATION_NPM_METRICS_DROP_LIST_CLUSTER=#{@npm_basic_drop_metrics_cluster}\n")
  else
    file.write("export AZMON_INTEGRATION_NPM_METRICS_URL_LIST_NODE=\n")
    file.write("export AZMON_INTEGRATION_NPM_METRICS_URL_LIST_CLUSTER=\n")
    file.write("export AZMON_INTEGRATION_NPM_METRICS_DROP_LIST_CLUSTER=\n")
  end
  # Close file after writing all environment variables
  file.close
else
  puts "Exception while opening file for writing config environment variables"
  puts "****************End Config Processing********************"
end
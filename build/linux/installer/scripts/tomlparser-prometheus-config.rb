#!/usr/local/bin/ruby
# frozen_string_literal: true

require_relative "tomlrb"
require_relative "ConfigParseErrorLogger"
require_relative "microsoft/omsagent/plugin/constants"

@configMapMountPath = "/etc/config/settings/prometheus-config"
@collectorConfigPath = "/opt/otelcollector/otelcollector-config.yml"
@configVersion = ""
@configSchemaVersion = ""

# Setting default values which will be used in case they are not set in the configmap or if configmap doesnt exist
@prometheusConfig = ""

# Use parser to parse the configmap toml file to a ruby structure
def parseConfigMap
  begin
    # Check to see if config map is created
    puts "config::configmap container-azm-ms-agentconfig for prometheus-config file: #{@configMapMountPath}"
    if (File.file?(@configMapMountPath))
      puts "config::configmap container-azm-ms-agentconfig for prometheus config mounted, parsing values"
      config = File.read(@configMapMountPath)
      puts "config::Successfully parsed mounted config map"
      return config
    else
      puts "config::configmap container-azm-ms-agentconfig for prometheus config not mounted, using defaults"
      return ""
    end
  rescue => errorStr
    ConfigParseErrorLogger.logError("Exception while parsing config map for prometheus config settings: #{errorStr}, using defaults, please check config map for errors")
    return ""
  end
end

# Get the prometheus config and indent correctly for otelcollector config
def populateSettingValuesFromConfigMap(configString)
  begin
    @prometheusConfig = configString.gsub(/\R+/, "\n        ")
    puts "config::Using config map setting for prometheus config"
  rescue => errorStr
    ConfigParseErrorLogger.logError("Exception while reading prometheus config - #{errorStr}, using defaults, please check config map for errors")
  end
end

@configSchemaVersion = ENV["AZMON_AGENT_CFG_SCHEMA_VERSION"]
puts "****************Start Prometheus Config Processing********************"
if !@configSchemaVersion.nil? && !@configSchemaVersion.empty? && @configSchemaVersion.strip.casecmp("v1") == 0 #note v1 is the only supported schema version, so hardcoding it
  prometheusConfigString = parseConfigMap
  if prometheusConfigString != ""
    populateSettingValuesFromConfigMap(prometheusConfigString)
  end
else
  if (File.file?(@configMapMountPath))
    ConfigParseErrorLogger.logError("config::unsupported/missing config schema version - '#{@configSchemaVersion}' , using defaults, please use supported schema version")
  end
end

begin
  puts "config::Starting to substitute the placeholders in collector.yml"
  #Replace the placeholder config values with values from custom config
  text = File.read(@collectorConfigPath)
  new_contents = text.gsub("$AZMON_PROMETHEUS_CONFIG", @prometheusConfig)
  File.open(@collectorConfigPath, "w") { |file| file.puts new_contents }
rescue => errorStr
  ConfigParseErrorLogger.logError("Exception while substituing placeholders for prometheus config - #{errorStr}")
end

# Write the settings to file, so that they can be set as environment variables
file = File.open("config_prometheus_config_env_var", "w")

if !file.nil?
  # Just write the original without the spacing needed for the otelcollector config
  file.write("export AZMON_PROMETHEUS_CONFIG=`cat #{@configMapMountPath}`\n")
  file.close
  puts "****************End Prometheus Config Processing********************"
else
  puts "Exception while opening file for writing prometheus config environment variables"
  puts "****************End Prometheus Config Processing********************"
end

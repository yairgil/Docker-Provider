#!/usr/local/bin/ruby
# frozen_string_literal: true

require_relative "tomlrb"
require_relative "ConfigParseErrorLogger"
require_relative "microsoft/omsagent/plugin/constants"

@configMapMountPath = "/etc/config/settings/prometheus/prometheus-config"
@collectorConfigTemplatePath = "/opt/otelcollector/otelcollector-config-template.yml"
@collectorConfigPath = "/opt/otelcollector/otelcollector-config.yml"
@configVersion = ""
@configSchemaVersion = ""

# Setting default values which will be used in case they are not set in the configmap or if configmap doesnt exist
@indentedConfig = ""
@useDefaultConfig = true

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
    # Indent for the otelcollector config
    @indentedConfig = configString.gsub(/\R+/, "\n        ")
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
  #Replace the placeholder value in the otelcollector with values from custom config
  text = File.read(@collectorConfigTemplatePath)
  new_contents = text.gsub("$AZMON_PROMETHEUS_CONFIG", @indentedConfig)
  File.open(@collectorConfigPath, "w") { |file| file.puts new_contents }
  @useDefaultConfig = false
rescue => errorStr
  ConfigParseErrorLogger.logError("Exception while substituing placeholders for prometheus config - #{errorStr}")
end

# Write the settings to file, so that they can be set as environment variables
file = File.open("config_prometheusconfig_env_var", "w")

if !file.nil?
  file.write("export AZMON_USE_DEFAULT_PROMETHEUS_CONFIG=#{@configExists}\n")
  # Close file after writing all metric collection setting environment variables
  file.close
  puts "****************End Prometheus Config Processing********************"
else
  puts "Exception while opening file for writing prometheus config environment variables"
  puts "****************End Prometheus Config Processing********************"
end

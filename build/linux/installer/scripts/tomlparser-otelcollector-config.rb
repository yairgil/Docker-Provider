#!/usr/local/bin/ruby
# frozen_string_literal: true

require_relative "tomlrb"
require_relative "ConfigParseErrorLogger"
require_relative "microsoft/omsagent/plugin/constants"

@configMapMountPath = "/etc/config/settings/otel-collector-settings"
@configVersion = ""
@configSchemaVersion = ""

# Setting default values which will be used in case they are not set in the configmap or if configmap doesnt exist
@collectPrometheusMetrics = false

# Use parser to parse the configmap toml file to a ruby structure
def parseConfigMap
  begin
    # Check to see if config map is created
    puts "config::configmap container-azm-ms-agentconfig for otel collector file: #{@configMapMountPath}"
    if (File.file?(@configMapMountPath))
      puts "config::configmap container-azm-ms-agentconfig for otel collector settings mounted, parsing values"
      parsedConfig = Tomlrb.load_file(@configMapMountPath, symbolize_keys: true)
      puts "config::Successfully parsed mounted config map"
      return parsedConfig
    else
      puts "config::configmap container-azm-ms-agentconfig for otel collector settings not mounted, using defaults"
      return nil
    end
  rescue => errorStr
    ConfigParseErrorLogger.logError("Exception while parsing config map for otel collector settings: #{errorStr}, using defaults, please check config map for errors")
    return nil
  end
end

# Use the ruby structure created after config parsing to set the right values to be used for otel collector settings
def populateSettingValuesFromConfigMap(parsedConfig)
  # Get if otel collector prometheus scraping is enabled
  begin
    if !parsedConfig.nil? && !parsedConfig[:otel_collector_settings].nil? && !parsedConfig[:otel_collector_settings][:prometheus_collection_settings].nil? && !parsedConfig[:otel_collector_settings][:prometheus_collection_settings][:enabled].nil?
      @collectPrometheusMetrics = parsedConfig[:otel_collector_settings][:prometheus_collection_settings][:enabled]
      puts "config::Using config map setting for otel collector prometheus settings"
    end
  rescue => errorStr
    ConfigParseErrorLogger.logError("Exception while reading config map settings for otelcollector - #{errorStr}, using defaults, please check config map for errors")
  end
end

@configSchemaVersion = ENV["AZMON_AGENT_CFG_SCHEMA_VERSION"]
puts "****************Start OpenTelemetryCollector Settings Processing********************"
if !@configSchemaVersion.nil? && !@configSchemaVersion.empty? && @configSchemaVersion.strip.casecmp("v1") == 0 #note v1 is the only supported schema version, so hardcoding it
  configMapSettings = parseConfigMap
  if !configMapSettings.nil?
    populateSettingValuesFromConfigMap(configMapSettings)
  end
else
  if (File.file?(@configMapMountPath))
    ConfigParseErrorLogger.logError("config::unsupported/missing config schema version - '#{@configSchemaVersion}' , using defaults, please use supported schema version")
  end
end

# Write the settings to file, so that they can be set as environment variables
file = File.open("config_otelcollector_env_var", "w")

if !file.nil?
  file.write("export AZMON_OTELCOLLECTOR_ENABLED=#{@collectPrometheusMetrics}\n")
  # Close file after writing all metric collection setting environment variables
  file.close
  puts "****************End OpenTelemetryCollector Settings Processing********************"
else
  puts "Exception while opening file for writing otelcollector config environment variables"
  puts "****************End OpenTelemetryCollector Settings Processing********************"
end

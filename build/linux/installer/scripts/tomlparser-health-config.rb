#!/usr/local/bin/ruby

#this should be require relative in Linux and require in windows, since it is a gem install on windows
@os_type = ENV["OS_TYPE"]
if !@os_type.nil? && !@os_type.empty? && @os_type.strip.casecmp("windows") == 0
  require "tomlrb"
else
  require_relative "tomlrb"
end

require_relative "ConfigParseErrorLogger"

@configMapMountPath = "/etc/config/settings/agent-settings"
@configSchemaVersion = ""
@enable_health_model = false
@nodesChunkSize = 0
@podsChunkSize = 0
@eventsChunkSize = 0
@deploymentsChunkSize = 0
@hpaChunkSize = 0

def is_number?(value)
  true if Integer(value) rescue false
end

# Use parser to parse the configmap toml file to a ruby structure
def parseConfigMap
  begin
    # Check to see if config map is created
    if (File.file?(@configMapMountPath))
      puts "config::configmap container-azm-ms-agentconfig for agent settings mounted, parsing values"
      parsedConfig = Tomlrb.load_file(@configMapMountPath, symbolize_keys: true)
      puts "config::Successfully parsed mounted config map"
      return parsedConfig
    else
      puts "config::configmap container-azm-ms-agentconfig for agent settings not mounted, using defaults"
      return nil
    end
  rescue => errorStr
    ConfigParseErrorLogger.logError("Exception while parsing config map for agent settings : #{errorStr}, using defaults, please check config map for errors")
    return nil
  end
end

# Use the ruby structure created after config parsing to set the right values to be used as environment variables
def populateSettingValuesFromConfigMap(parsedConfig)
  begin
    if !parsedConfig.nil? && !parsedConfig[:agent_settings].nil?
      if !parsedConfig[:agent_settings][:health_model].nil? && !parsedConfig[:agent_settings][:health_model][:enabled].nil?
        @enable_health_model = parsedConfig[:agent_settings][:health_model][:enabled]
        puts "enable_health_model = #{@enable_health_model}"
      end
      chunk_config = parsedConfig[:agent_settings][:chunk_config]
      if !chunk_config.nil?
        nodesChunkSize = chunk_config[:NODES_CHUNK_SIZE]
        if !nodesChunkSize.nil? && is_number?(nodesChunkSize)
          @nodesChunkSize = nodesChunkSize.to_i
          puts "NODES_CHUNK_SIZE = #{@nodesChunkSize}"
        end
        podsChunkSize = chunk_config[:PODS_CHUNK_SIZE]
        if !podsChunkSize.nil? && is_number?(podsChunkSize)
          @podsChunkSize = podsChunkSize.to_i
          puts "PODS_CHUNK_SIZE = #{@podsChunkSize}"
        end
        eventsChunkSize = chunk_config[:EVENTS_CHUNK_SIZE]
        if !eventsChunkSize.nil? && is_number?(eventsChunkSize)
          @eventsChunkSize = eventsChunkSize.to_i
          puts "EVENTS_CHUNK_SIZE = #{@eventsChunkSize}"
        end
        deploymentsChunkSize = chunk_config[:DEPLOYMENTS_CHUNK_SIZE]
        if !deploymentsChunkSize.nil? && is_number?(deploymentsChunkSize)
          @deploymentsChunkSize = deploymentsChunkSize.to_i
          puts "DEPLOYMENTS_CHUNK_SIZE = #{@deploymentsChunkSize}"
        end
        hpaChunkSize = chunk_config[:HPA_CHUNK_SIZE]
        if !hpaChunkSize.nil? && is_number?(hpaChunkSize)
          @hpaChunkSize = hpaChunkSize.to_i
          puts "HPA_CHUNK_SIZE = #{@hpaChunkSize}"
        end
      end
    end
  rescue => errorStr
    puts "config::error:Exception while reading config settings for health_model enabled setting - #{errorStr}, using defaults"
    @enable_health_model = false
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
  @enable_health_model = false
end

# Write the settings to file, so that they can be set as environment variables
file = File.open("health_config_env_var", "w")

if !file.nil?
  file.write("export AZMON_CLUSTER_ENABLE_HEALTH_MODEL=#{@enable_health_model}\n")
  if @nodesChunkSize > 0
    file.write("export NODES_CHUNK_SIZE=#{@nodesChunkSize}\n")
  end
  if @podsChunkSize > 0
    file.write("export PODS_CHUNK_SIZE=#{@podsChunkSize}\n")
  end
  if @eventsChunkSize > 0
    file.write("export EVENTS_CHUNK_SIZE=#{@eventsChunkSize}\n")
  end
  if @deploymentsChunkSize > 0
    file.write("export DEPLOYMENTS_CHUNK_SIZE=#{@deploymentsChunkSize}\n")
  end
  if @hpaChunkSize > 0
    file.write("export HPA_CHUNK_SIZE=#{@hpaChunkSize}\n")
  end
  # Close file after writing all environment variables
  file.close
else
  puts "Exception while opening file for writing config environment variables"
  puts "****************End Config Processing********************"
end

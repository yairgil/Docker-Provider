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

# 250 Node items (15KB per node) account to approximately 4MB
@nodesChunkSize = 250
# 1000 pods (10KB per pod) account to approximately 10MB
@podsChunkSize = 1000
# 4000 events (1KB per event) account to approximately 4MB
@eventsChunkSize = 4000
# roughly each deployment is 8k
# 500 deployments account to approximately 4MB
@deploymentsChunkSize = 500
# roughly each HPA is 3k
# 2000 HPAs account to approximately 6-7MB
@hpaChunkSize = 2000
# stream batch sizes to avoid large file writes
# too low will consume higher disk iops
@podsEmitStreamBatchSize = 200
@nodesEmitStreamBatchSize = 100

# higher the chunk size rs pod memory consumption higher and lower api latency
# similarly lower the value, helps on the memory consumption but incurrs additional round trip latency
# these needs to be tuned be based on the workload
# nodes
@nodesChunkSizeMin = 100
@nodesChunkSizeMax = 400
# pods
@podsChunkSizeMin = 250
@podsChunkSizeMax = 1500
# events
@eventsChunkSizeMin = 2000
@eventsChunkSizeMax = 10000
# deployments
@deploymentsChunkSizeMin = 500
@deploymentsChunkSizeMax = 1000
# hpa
@hpaChunkSizeMin = 500
@hpaChunkSizeMax = 2000

# emit stream sizes to prevent lower values which costs disk i/o
# max will be upto the chunk size
@podsEmitStreamBatchSizeMin = 50
@nodesEmitStreamBatchSizeMin = 50

# configmap settings related fbit config
@fbitFlushInterval = 0
@fbitTailBufferChunkSize = 0
@fbitTailBufferMaxSize = 0


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
        if !nodesChunkSize.nil? && is_number?(nodesChunkSize) && (@nodesChunkSizeMin..@nodesChunkSizeMax) === nodesChunkSize.to_i
          @nodesChunkSize = nodesChunkSize.to_i
          puts "Using config map value: NODES_CHUNK_SIZE = #{@nodesChunkSize}"
        end

        podsChunkSize = chunk_config[:PODS_CHUNK_SIZE]
        if !podsChunkSize.nil? && is_number?(podsChunkSize) && (@podsChunkSizeMin..@podsChunkSizeMax) === podsChunkSize.to_i
          @podsChunkSize = podsChunkSize.to_i
          puts "Using config map value: PODS_CHUNK_SIZE = #{@podsChunkSize}"
        end

        eventsChunkSize = chunk_config[:EVENTS_CHUNK_SIZE]
        if !eventsChunkSize.nil? && is_number?(eventsChunkSize) && (@eventsChunkSizeMin..@eventsChunkSizeMax) === eventsChunkSize.to_i
          @eventsChunkSize = eventsChunkSize.to_i
          puts "Using config map value: EVENTS_CHUNK_SIZE = #{@eventsChunkSize}"
        end

        deploymentsChunkSize = chunk_config[:DEPLOYMENTS_CHUNK_SIZE]
        if !deploymentsChunkSize.nil? && is_number?(deploymentsChunkSize) && (@deploymentsChunkSizeMin..@deploymentsChunkSizeMax) === deploymentsChunkSize.to_i
          @deploymentsChunkSize = deploymentsChunkSize.to_i
          puts "Using config map value: DEPLOYMENTS_CHUNK_SIZE = #{@deploymentsChunkSize}"
        end

        hpaChunkSize = chunk_config[:HPA_CHUNK_SIZE]
        if !hpaChunkSize.nil? && is_number?(hpaChunkSize) && (@hpaChunkSizeMin..@hpaChunkSizeMax) === hpaChunkSize.to_i
          @hpaChunkSize = hpaChunkSize.to_i
          puts "Using config map value: HPA_CHUNK_SIZE = #{@hpaChunkSize}"
        end

        podsEmitStreamBatchSize = chunk_config[:PODS_EMIT_STREAM_BATCH_SIZE]
        if !podsEmitStreamBatchSize.nil? && is_number?(podsEmitStreamBatchSize) &&
           podsEmitStreamBatchSize.to_i <= @podsChunkSize && podsEmitStreamBatchSize.to_i >= @podsEmitStreamBatchSizeMin
          @podsEmitStreamBatchSize = podsEmitStreamBatchSize.to_i
          puts "Using config map value: PODS_EMIT_STREAM_BATCH_SIZE = #{@podsEmitStreamBatchSize}"
        end
        nodesEmitStreamBatchSize = chunk_config[:NODES_EMIT_STREAM_BATCH_SIZE]
        if !nodesEmitStreamBatchSize.nil? && is_number?(nodesEmitStreamBatchSize) &&
           nodesEmitStreamBatchSize.to_i <= @nodesChunkSize && nodesEmitStreamBatchSize.to_i >= @nodesEmitStreamBatchSizeMin
          @nodesEmitStreamBatchSize = nodesEmitStreamBatchSize.to_i
          puts "Using config map value: NODES_EMIT_STREAM_BATCH_SIZE = #{@nodesEmitStreamBatchSize}"
        end
      end
      # fbit config settings
      fbit_config = parsedConfig[:agent_settings][:fbit_config]
      if !fbit_config.nil?
        fbitFlushInterval = fbit_config[:FBIT_SERVICE_FLUSH_INTERVAL]
        if !fbitFlushInterval.nil? && is_number?(fbitFlushInterval) && fbitFlushInterval.to_i > 0
          @fbitFlushInterval = fbitFlushInterval.to_i
          puts "Using config map value: FBIT_SERVICE_FLUSH_INTERVAL = #{@fbitFlushInterval}"
        end

        fbitTailBufferChunkSize = fbit_config[:FBIT_TAIL_BUFFER_CHUNK_SIZE]
        if !fbitTailBufferChunkSize.nil? && is_number?(fbitTailBufferChunkSize) && fbitTailBufferChunkSize.to_i > 0
          @fbitTailBufferChunkSize = fbitTailBufferChunkSize.to_i
          puts "Using config map value: FBIT_TAIL_BUFFER_CHUNK_SIZE = #{@fbitTailBufferChunkSize}"
        end

        fbitTailBufferMaxSize = fbit_config[:FBIT_TAIL_BUFFER_MAX_SIZE]
        if !fbitTailBufferMaxSize.nil? && is_number?(fbitTailBufferMaxSize) && fbitTailBufferMaxSize.to_i > 0 
          if fbitTailBufferMaxSize.to_i >= @fbitTailBufferChunkSize
            @fbitTailBufferMaxSize = fbitTailBufferMaxSize.to_i
            puts "Using config map value: FBIT_TAIL_BUFFER_MAX_SIZE = #{@fbitTailBufferMaxSize}"
          else
            @fbitTailBufferMaxSize = @fbitTailBufferChunkSize
            puts "config::warn: FBIT_TAIL_BUFFER_MAX_SIZE should be greater or equal to value of FBIT_TAIL_BUFFER_CHUNK_SIZE. Using FBIT_TAIL_BUFFER_MAX_SIZE = #{@fbitTailBufferMaxSize} since provodided config value not valid"
          end
        end
         # in scenario - FBIT_TAIL_BUFFER_MAX_SIZE provided but not FBIT_TAIL_BUFFER_CHUNK_SIZE
        if @fbitTailBufferMaxSize > 0 && @fbitTailBufferChunkSize == 0
          @fbitTailBufferChunkSize = @fbitTailBufferMaxSize
        end 
      end
    end
  rescue => errorStr
    puts "config::error:Exception while reading config settings for agent configuration setting - #{errorStr}, using defaults"
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
file = File.open("agent_config_env_var", "w")

if !file.nil?
  file.write("export AZMON_CLUSTER_ENABLE_HEALTH_MODEL=#{@enable_health_model}\n")
  file.write("export NODES_CHUNK_SIZE=#{@nodesChunkSize}\n")
  file.write("export PODS_CHUNK_SIZE=#{@podsChunkSize}\n")
  file.write("export EVENTS_CHUNK_SIZE=#{@eventsChunkSize}\n")
  file.write("export DEPLOYMENTS_CHUNK_SIZE=#{@deploymentsChunkSize}\n")
  file.write("export HPA_CHUNK_SIZE=#{@hpaChunkSize}\n")
  file.write("export PODS_EMIT_STREAM_BATCH_SIZE=#{@podsEmitStreamBatchSize}\n")
  file.write("export NODES_EMIT_STREAM_BATCH_SIZE=#{@nodesEmitStreamBatchSize}\n")
  # fbit settings
  if @fbitFlushInterval > 0
    file.write("export FBIT_SERVICE_FLUSH_INTERVAL=#{@fbitFlushInterval}\n")
  end
  if @fbitTailBufferChunkSize > 0
    file.write("export FBIT_TAIL_BUFFER_CHUNK_SIZE=#{@fbitTailBufferChunkSize}\n")
  end
  if @fbitTailBufferMaxSize > 0
    file.write("export FBIT_TAIL_BUFFER_MAX_SIZE=#{@fbitTailBufferMaxSize}\n")
  end 
  # Close file after writing all environment variables
  file.close
else
  puts "Exception while opening file for writing config environment variables"
  puts "****************End Config Processing********************"
end

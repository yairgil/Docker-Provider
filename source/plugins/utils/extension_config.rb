require "socket"
require "msgpack"
require "securerandom"
require_relative "omslog"
require_relative "constants"

class ExtensionConfigCache
    
  def initialize
    @cache = {}
    @cache_lock = Mutex.new  
  end
  
  def get_output_stream_id(datatypeId)
    @cache_lock.synchronize {
      if @cache.has_key?(datatypeId)
        return @cache[datatypeId]
      else
        @cache = get_extension_config()
        return @cache[datatypeId]
      end
    }
  end

  private 
  def get_extension_config()
    extConfig = Hash.new
    $log.info("ExtenionConfig:get_extension_config start ...")
    begin
      clientSocket = UNIXSocket.open(Constants::ONEAGENT_FLUENT_SOCKET_NAME)
      requestId = SecureRandom.uuid.to_s
      requestBodyJSON = { "Request" => "AgentTaggedData", "RequestId" => requestId, "Tag" => Constants::CI_EXTENSION_NAME, "Version" => Constants::CI_EXTENSION_VERSION }.to_json
      $log.info("sending request with request body: #{requestBodyJSON}")
      requestBodyMsgPack = requestBodyJSON.to_msgpack
      clientSocket.write(requestBodyMsgPack)
      clientSocket.flush
      $log.info("reading the response from fluent socket: #{Constants::ONEAGENT_FLUENT_SOCKET_NAME}")
      resp = clientSocket.recv(Constants::CI_EXTENSION_CONFIG_MAX_BYTES)
      if !resp.nil? && !resp.empty?
        respJSON = JSON.parse(resp)
        taggedData = respJSON["TaggedData"]
        if !taggedData.nil? && !taggedData.empty?
          taggedAgentData = JSON.parse(taggedData)
          extensionConfigurations = taggedAgentData["extensionConfigurations"]
          extensionConfigurations.each do |extensionConfig|
            outputstreams = extensionConfig["outputStreams"]
            if !outputstreams.nil? && !outputstreams.empty? &&
               !outputstreams.keys.nil? && !outputstreams.keys.empty? &&
               outputstreams.keys.length > 0 && !outputstreams.keys[0].empty?
              datatypeId = outputstreams.keys[0]
              streamId = outputstreams[datatypeId]
              if !streamId.nil? && !streamId.empty?
                extConfig[datatypeId] = streamId
              else
                $log.warn("streamId is nil or empty for datatype: #{datatypeId}")
              end
              $log.info("datatype id: #{datatypeId} and streamid: #{streamId}")
            end
          end
        end
      end
    rescue => error
      $log.warn("get_extension_config failed: #{error}")
    ensure
      clientSocket.close unless clientSocket.nil?
    end
    $log.info("ExtenionConfig:get_extension_config complete ...")
    return extConfig
  end
end
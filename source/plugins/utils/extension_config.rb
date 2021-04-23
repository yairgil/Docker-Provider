require "socket"
require "msgpack"
require "securerandom"
require "singleton"
require_relative "omslog"
require_relative "constants"
require_relative "ApplicationInsightsUtility"


class ExtensionConfig
  include Singleton

  def initialize
    @cache = {}
    @cache_lock = Mutex.new 
    $log.info("ExtensionConfig::initialize complete")
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
    $log.info("ExtensionConfig::get_extension_config start ...")
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
          if !extensionConfigurations.nil? && !extensionConfigurations.empty?
            extensionConfigurations.each do |extensionConfig|
              outputStreams = extensionConfig["outputStreams"]
              if !outputStreams.nil? && !outputStreams.empty? 
                outputStreams.each do |datatypeId, streamId|
                  $log.info("datatypeId: #{datatypeId}, streamId: #{streamId}")
                  extConfig[datatypeId] = streamId
                end
              else
                $log.info("received outputStreams is either nil or empty")
              end                    
            end
          else
            $log.info("received extensionConfigurations from fluentsocket is either nil or empty")  
          end 
        end
      end
    rescue => errorStr
      $log.warn("ExtensionConfig::get_extension_config failed: #{error}")
      ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
    ensure
      clientSocket.close unless clientSocket.nil?
    end
    $log.info("ExtensionConfig::get_extension_config complete ...")
    return extConfig
  end
end
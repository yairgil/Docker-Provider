require "socket"
require "msgpack"
require "securerandom"
require "singleton"
require_relative "omslog"
require_relative "constants"

class ExtensionConfig
  include Singleton

  def initialize
    $log.info("ExtenionConfig:initialize start ...")
    @extensionConfig = getExtensionConfig()
    $log.info("ExtenionConfig:initialize complete.")
  end

  def getExtensionConfig()
    extConfig = Hash.new
    $log.info("ExtenionConfig:getExtensionConfig start ...")
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
      $log.warn("getExtensionConfig failed: #{error}")
    ensure
      clientSocket.close unless clientSocket.nil?
    end
    $log.info("ExtenionConfig:getExtensionConfig complete ...")
    return extConfig
  end

  def getOutputStreamId(datatype_name)
    if @extensionConfig.empty? || @extensionConfig[datatype_name].nil? ||  @extensionConfig[datatype_name].empty? 
        @extensionConfig = getExtensionConfig()
    end 
    return @extensionConfig[datatype_name]   
  end
end  # ExtensionConfig

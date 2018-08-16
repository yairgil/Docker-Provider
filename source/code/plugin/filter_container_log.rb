# frozen_string_literal: true

require 'fluent/filter'
require 'json'
require 'logger'
module Fluent
  require 'logger'
  class PassThruFilter < Filter
    Fluent::Plugin.register_filter('filter_container_log', self)

    def configure(conf)
      super
      if @enable_log
				@log = Logger.new(@log_path, 'weekly')
				@log.debug {'Starting filter_container plugin'}
			end
    end

    def start
      super
      @hostname = OMS::Common.get_hostname or "Unknown host"
      @imageHash = {}
      @nameHash = {}
      @containerIDFilePath = "/var/opt/microsoft/docker-cimprov/state/ContainerInventory/"
      Dir.new(containerIDFilePath).reject{ |f| File.directory? f }.each do |file|
        fileName = containerIDFilePath + file
        f = File.open(fileName)
        temp = JSON.parse(f.readline)
        @imageHash[file] = temp["Image"]
        @nameHash[file] = temp["ElementName"]
      end
    end

    def shutdown
      super
    end
    
    def filter(tag, time, record)
      containerId = record["Id"]
      unless @imageHash.has_key?(containerId)
        fileName = containerIDFilePath + containerId
        f = File.open(fileName)
        temp = JSON.parse(f.readline)
        imageHash[containerId] = temp["Image"]
        nameHash[containerId] = temp["ElementName"]
      end

      record["Image"] = imageHash[containerId]
      record["Name"] = nameHash[containerId]

      @log.debug "record #{record}'"
      wrapper = {
                 "DataType"=>"CONTAINER_LOG_BLOB",
                 "IPName"=>"Containers",
                 "DataItems"=>[record.each{|k,v| record[k]=v}]
      }
      wrapper
    end
  end
end

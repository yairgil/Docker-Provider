#!/usr/local/bin/ruby
# frozen_string_literal: true

require 'fluent/plugin/input'

module Fluent::Plugin
  class Kube_Event_Input < Input
    Fluent::Plugin.register_input("kube_events", self)
    @@KubeEventsStateFile = "/var/opt/microsoft/docker-cimprov/state/KubeEventQueryState.yaml"

    def initialize
      super
      require "yajl/json_gem"
      require "yajl"
      require "time"
      require "kubeclient"
      require "logger"

      require_relative "KubernetesApiClient"
      require_relative "oms_common"
      require_relative "omslog"
      require_relative "ApplicationInsightsUtility"
      require_relative "extension_utils"

      # refer tomlparser-agent-config for defaults
      # this configurable via configmap
      @EVENTS_CHUNK_SIZE = 0

      # Initializing events count for telemetry
      @eventsCount = 0

      # Initilize enable/disable normal event collection
      @collectAllKubeEvents = false
      @k8sWatchClient = nil
      @watchthread = nil
      @resourceVersion = 0
      @fieldSelector = nil
      @eventQueryState = []
      @events = {}
    end

    config_param :run_interval, :time, :default => 60
    config_param :tag, :string, :default => "oneagent.containerInsights.KUBE_EVENTS_BLOB"

    def configure(conf)
      super
    end

    def start
      if @run_interval
        super
        if !ENV["EVENTS_CHUNK_SIZE"].nil? && !ENV["EVENTS_CHUNK_SIZE"].empty? && ENV["EVENTS_CHUNK_SIZE"].to_i > 0
          @EVENTS_CHUNK_SIZE = ENV["EVENTS_CHUNK_SIZE"].to_i
        else
          # this shouldnt happen just setting default here as safe guard
          $log.warn("in_kube_events::start: setting to default value since got EVENTS_CHUNK_SIZE nil or empty")
          @EVENTS_CHUNK_SIZE = 4000
        end
        $log.info("in_kube_events::start : EVENTS_CHUNK_SIZE  @ #{@EVENTS_CHUNK_SIZE}")
        @finished = false
        @condition = ConditionVariable.new
        @mutex = Mutex.new
        @eventsStateLock = Mutex.new
        @thread = Thread.new(&method(:run_periodic))
        collectAllKubeEventsSetting = ENV["AZMON_CLUSTER_COLLECT_ALL_KUBE_EVENTS"]
        if !collectAllKubeEventsSetting.nil? && !collectAllKubeEventsSetting.empty?
          if collectAllKubeEventsSetting.casecmp("false") == 0
            @collectAllKubeEvents = false
            @fieldSelector = "type!=Normal"
            $log.warn("Normal kube events collection disabled for cluster")
          else
            @collectAllKubeEvents = true
            $log.warn("Normal kube events collection enabled for cluster")
          end
        end
        @watchthread = Thread.new(&method(:watch_events))
      end
    end

    def shutdown
      if @run_interval
        @mutex.synchronize {
          @finished = true
          @condition.signal
        }
        @thread.join
        @watchthread.join
        super
      end
    end

    def enumerate
      begin
        eventList = nil
        currentTime = Time.now
        batchTime = currentTime.utc.iso8601
        @eventsCount = 0
        eventQueryState = []

        if ExtensionUtils.isAADMSIAuthMode()
          $log.info("in_kube_events::enumerate: AAD AUTH MSI MODE")
          if @tag.nil? || !@tag.start_with?(Constants::EXTENSION_OUTPUT_STREAM_ID_TAG_PREFIX)
            @tag = ExtensionUtils.getOutputStreamId(Constants::KUBE_EVENTS_DATA_TYPE)
          end
          $log.info("in_kube_events::enumerate: using kubeevents tag -#{@tag} @ #{Time.now.utc.iso8601}")
        end
        # Initializing continuation token to nil
        continuationToken = nil
        $log.info("in_kube_events::enumerate : Getting events obtained from watch @ #{Time.now.utc.iso8601}")
        eventList = {}
        @eventsStateLock.synchronize do
          eventList["items"] = @events.values.dup
          eventQueryState = @currentEventQueryState
          @events.clear
        end
        $log.info("in_kube_events::enumerate : Getting events obtained from watch @ #{Time.now.utc.iso8601}")

        if (!eventList.nil? && !eventList.empty? && !eventList["items"].nil? && !eventList["items"].empty?)
          eventsCount = eventList["items"].length
          $log.info "in_kube_events::enumerate:Received number of events in eventList is #{eventsCount} @ #{Time.now.utc.iso8601}"
          parse_and_emit_records(eventList, batchTime)
        else
          $log.warn "in_kube_events::enumerate:Received empty eventList"
        end

        # Setting this to nil so that we dont hold memory until GC kicks in
        eventList = nil
        writeEventQueryState(eventQueryState)

        # Flush AppInsights telemetry once all the processing is done, only if the number of events flushed is greater than 0
        if (@eventsCount > 0)
          ApplicationInsightsUtility.sendMetricTelemetry("EventCount", @eventsCount, {})
        end
      rescue => errorStr
        $log.warn "in_kube_events::enumerate:Failed in enumerate: #{errorStr}"
        $log.debug_backtrace(errorStr.backtrace)
        ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
      end
    end # end enumerate

    def parse_and_emit_records(events, batchTime = Time.utc.iso8601)
      currentTime = Time.now
      emitTime = Fluent::Engine.now
      @@istestvar = ENV["ISTEST"]
      begin
        eventStream = Fluent::MultiEventStream.new
        events["items"].each do |items|
          record = {}
          #<BUGBUG> - Not sure if ingestion has the below mapping for this custom type. Fix it as part of fixed type conversion
          record["CollectionTime"] = batchTime #This is the time that is mapped to become TimeGenerated
          eventId = items["metadata"]["uid"] + "/" + items["count"].to_s
          nodeName = items["source"].key?("host") ? items["source"]["host"] : (OMS::Common.get_hostname)
          # For ARO v3 cluster, drop the master and infra node sourced events to ingest
          if KubernetesApiClient.isAROV3Cluster && !nodeName.nil? && !nodeName.empty? &&
             (nodeName.downcase.start_with?("infra-") || nodeName.downcase.start_with?("master-"))
            next
          end

          record["ObjectKind"] = items["involvedObject"]["kind"]
          record["Namespace"] = items["involvedObject"]["namespace"]
          record["Name"] = items["involvedObject"]["name"]
          record["Reason"] = items["reason"]
          record["Message"] = items["message"]
          record["KubeEventType"] = items["type"]
          record["TimeGenerated"] = items["metadata"]["creationTimestamp"]
          record["SourceComponent"] = items["source"]["component"]
          record["FirstSeen"] = items["firstTimestamp"]
          record["LastSeen"] = items["lastTimestamp"]
          record["Count"] = items["count"]
          record["Computer"] = nodeName
          record["ClusterName"] = KubernetesApiClient.getClusterName
          record["ClusterId"] = KubernetesApiClient.getClusterId
          eventStream.add(emitTime, record) if record
          @eventsCount += 1
        end
        router.emit_stream(@tag, eventStream) if eventStream
        if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0)
          $log.info("kubeEventsInventoryEmitStreamSuccess @ #{Time.now.utc.iso8601}")
        end
      rescue => errorStr
        $log.debug_backtrace(errorStr.backtrace)
        ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
      end
    end

    def run_periodic
      @mutex.lock
      done = @finished
      @nextTimeToRun = Time.now
      @waitTimeout = @run_interval
      until done
        @nextTimeToRun = @nextTimeToRun + @run_interval
        @now = Time.now
        if @nextTimeToRun <= @now
          @waitTimeout = 1
          @nextTimeToRun = @now
        else
          @waitTimeout = @nextTimeToRun - @now
        end
        @condition.wait(@mutex, @waitTimeout)
        done = @finished
        @mutex.unlock
        if !done
          begin
            $log.info("in_kube_events::run_periodic.enumerate.start @ #{Time.now.utc.iso8601}")
            enumerate
            $log.info("in_kube_events::run_periodic.enumerate.end @ #{Time.now.utc.iso8601}")
          rescue => errorStr
            $log.warn "in_kube_events::run_periodic: enumerate Failed to retrieve kube events: #{errorStr}"
            ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
          end
        end
        @mutex.lock
      end
      @mutex.unlock
    end

    def getEventQueryState
      eventQueryState = []
      begin
        if File.file?(@@KubeEventsStateFile)
          # Do not read the entire file in one shot as it spikes memory (50+MB) for ~5k events
          File.foreach(@@KubeEventsStateFile) do |line|
            eventQueryState.push(line.chomp) #puts will append newline which needs to be removed
          end
        end
      rescue => errorStr
        $log.warn $log.warn line.dump, error: errorStr.to_s
        $log.debug_backtrace(errorStr.backtrace)
        ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
      end
      return eventQueryState
    end

    def writeEventQueryState(eventQueryState)
      begin
        if (!eventQueryState.nil? && !eventQueryState.empty?)
          # No need to close file handle (f) due to block scope
          File.open(@@KubeEventsStateFile, "w") do |f|
            f.puts(eventQueryState)
          end
        end
      rescue => errorStr
        $log.warn $log.warn line.dump, error: errorStr.to_s
        $log.debug_backtrace(errorStr.backtrace)
        ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
      end
    end

    def getK8sWatchClient
      if @k8sWatchClient.nil?
        ssl_options = {
          ca_file: "/var/run/secrets/kubernetes.io/serviceaccount/ca.crt",
          verify_ssl: OpenSSL::SSL::VERIFY_PEER,
        }
        timeouts = {
          open: 60,  # default setting (in seconds)
          read: nil  # read never times out
        }
        getTokenStr = "Bearer " + KubernetesApiClient.getTokenStr
        auth_options = { bearer_token: KubernetesApiClient.getTokenStr }
        @k8sWatchClient = Kubeclient::Client.new("https://#{ENV["KUBERNETES_SERVICE_HOST"]}:#{ENV["KUBERNETES_PORT_443_TCP_PORT"]}/api/", "v1", ssl_options: ssl_options, auth_options: auth_options, as: :parsed, timeouts: timeouts)
      end
      return @k8sWatchClient
    end

    def watch_events
      loop do
        begin
            @eventsStateLock.synchronize do
              @currentEventQueryState = getEventQueryState
            end
            client = getK8sWatchClient
            if @resourceVersion == 0
              eventList = client.get_events(limit: @EVENTS_CHUNK_SIZE, field_selector: @fieldSelector)
              @resourceVersion = eventList["metadata"]["resourceVersion"]
              continue = eventList["metadata"]["continue"]
              if (!eventList.nil? && !eventList.empty? && eventList.key?("items") && !eventList["items"].nil? && !eventList["items"].empty?)
                  eventList["items"].each do |item|
                    eventId = item["metadata"]["uid"] + "/" + item["count"].to_s
                    eventItem = getEventOptimizedItem(item)
                    @eventsStateLock.synchronize do
                      if @currentEventQueryState.empty? || !@currentEventQueryState.include?(eventId)
                        @events[eventId] = eventItem
                        @currentEventQueryState.push(eventId)
                      end
                    end
                  end
              end
              while !continue.nil? && !continue.empty?
                eventList = client.get_events(limit: @EVENTS_CHUNK_SIZE, continue: continue, field_selector: @fieldSelector)
                @resourceVersion = eventList["metadata"]["resourceVersion"]
                continue = eventList["metadata"]["continue"]
                if (!eventList.nil? && !eventList.empty? && eventList.key?("items") && !eventList["items"].nil? && !eventList["items"].empty?)
                    eventList["items"].each do |item|
                      eventId = item["metadata"]["uid"] + "/" + item["count"].to_s
                      eventItem = getEventOptimizedItem(item)
                      @eventsStateLock.synchronize do
                        if @currentEventQueryState.empty? || !@currentEventQueryState.include?(eventId)
                          @events[eventId] = eventItem
                          @currentEventQueryState.push(eventId)
                        end
                      end
                    end
                end
              end
            end
            watcher = client.watch_events(resource_version: @resourceVersion, field_selector: @fieldSelector, allowWatchBookmarks: true, as: :parsed)
            stop_reason = 'disconnect'
            watcher.each do |notice|
                item = notice["object"]
                # extract latest resource version to use for watch reconnect
                if !item.nil? && !item.empty? &&
                !item["metadata"].nil? && !item["metadata"].empty?  &&
                !item["metadata"]["resourceVersion"].nil? && !item["metadata"]["resourceVersion"].empty?
                @resourceVersion = item["metadata"]["resourceVersion"]
                end
                case notice["type"]
                when 'BOOKMARK' then
                  $log.info("BOOKMARK event and resource version: #{resourceVersion}")
                when 'ADDED', 'MODIFIED' then
                item = notice["object"]
                eventId = item["metadata"]["uid"] + "/" + item["count"].to_s
                eventItem = getEventOptimizedItem(item)
                @eventsStateLock.synchronize do
                  if @currentEventQueryState.empty? || !@currentEventQueryState.include?(eventId)
                    @events[eventId] = eventItem
                    @currentEventQueryState.push(eventId)
                  end
                end
                when 'DELETED' then
                eventId = item["metadata"]["uid"] + "/" + item["count"].to_s
                @eventsStateLock.synchronize do
                  @events.delete(eventId)
                end
                when 'ERROR'
                  $log.warn("ERROR event and resource version: #{resourceVersion}")
                  stop_reason = 'error'
                  # enforce list in case of error such as resource verison expired or version too long error etc.
                  @resourceVersion = 0
                break
                else
                  $log.warn("Unsupported event type #{notice["type"]}")
                end
            end
        rescue => exceptionStr
           $log.warn("Watch session got broken with reason: #{stop_reason} and exception: #{exceptionStr}")
        end
      end
    end

    def getEventOptimizedItem(resourceItem)
      item = {}
      item["metadata"] =  {}
      if !resourceItem["metadata"].nil?
          item["metadata"]["uid"] =  resourceItem["metadata"]["uid"]
          item["metadata"]["creationTimestamp"] =  resourceItem["metadata"]["creationTimestamp"]
      end
      item["source"] =  {}
      if !resourceItem["source"].nil? && !resourceItem["source"].empty?
          if !resourceItem["source"]["host"].nil? && !resourceItem["source"]["host"].empty?
              item["source"]["host"] = resourceItem["source"]["host"]
          end
          item["source"]["component"] = {}
          if !resourceItem["source"]["component"].nil? && !resourceItem["source"]["component"].empty?
              item["source"]["component"] = resourceItem["source"]["component"]
          end
      end
      item["involvedObject"] =  {}
      if !resourceItem["involvedObject"].nil? && !resourceItem["involvedObject"].empty?
          item["involvedObject"]["kind"] =  {}
          if !resourceItem["involvedObject"]["kind"].nil? && !resourceItem["involvedObject"]["kind"].empty?
              item["involvedObject"]["kind"] = resourceItem["involvedObject"]["kind"]
          end
          item["involvedObject"]["namespace"] =  {}
          if !resourceItem["involvedObject"]["namespace"].nil? && !resourceItem["involvedObject"]["namespace"].empty?
              item["involvedObject"]["namespace"] = resourceItem["involvedObject"]["namespace"]
          end
      end
      item["reason"] = resourceItem["reason"]
      item["message"] = resourceItem["message"]
      item["type"] = resourceItem["type"]
      item["firstTimestamp"] = resourceItem["firstTimestamp"]
      item["lastTimestamp"] = resourceItem["lastTimestamp"]
      item["count"] = resourceItem["count"]
      return item
  end


  end # Kube_Event_Input
end # module

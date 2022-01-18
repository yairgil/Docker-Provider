#!/usr/local/bin/ruby
# frozen_string_literal: true

require "fluent/plugin/input"

module Fluent::Plugin
  class Kube_PerfInventory_Input < Input
    Fluent::Plugin.register_input("kube_perfinventory", self)

    def initialize
      super
      require "yaml"
      require "yajl/json_gem"
      require "yajl"
      require "set"
      require "time"
      require "net/http"

      require_relative "KubernetesApiClient"
      require_relative "ApplicationInsightsUtility"
      require_relative "oms_common"
      require_relative "omslog"
      require_relative "constants"
      require_relative "extension_utils"

      # refer tomlparser-agent-config for updating defaults
      # this configurable via configmap
      @PODS_CHUNK_SIZE = 0
      @PODS_EMIT_STREAM_BATCH_SIZE = 0
      @NODES_CHUNK_SIZE = 0

      @watchPodsThread = nil
      @podItemsCache = {}

      @watchNodesThread = nil
      @nodeAllocatableCache = {}

      @kubeperfTag = "oneagent.containerInsights.LINUX_PERF_BLOB"
      @insightsMetricsTag = "oneagent.containerInsights.INSIGHTS_METRICS_BLOB"
    end

    config_param :run_interval, :time, :default => 60
    config_param :tag, :string, :default => "oneagent.containerInsights.LINUX_PERF_BLOB"

    def configure(conf)
      super
    end

    def start
      if @run_interval
        super
        if !ENV["PODS_CHUNK_SIZE"].nil? && !ENV["PODS_CHUNK_SIZE"].empty? && ENV["PODS_CHUNK_SIZE"].to_i > 0
          @PODS_CHUNK_SIZE = ENV["PODS_CHUNK_SIZE"].to_i
        else
          # this shouldnt happen just setting default here as safe guard
          $log.warn("in_kube_perfinventory::start: setting to default value since got PODS_CHUNK_SIZE nil or empty")
          @PODS_CHUNK_SIZE = 1000
        end
        $log.info("in_kube_perfinventory::start: PODS_CHUNK_SIZE  @ #{@PODS_CHUNK_SIZE}")

        if !ENV["PODS_EMIT_STREAM_BATCH_SIZE"].nil? && !ENV["PODS_EMIT_STREAM_BATCH_SIZE"].empty? && ENV["PODS_EMIT_STREAM_BATCH_SIZE"].to_i > 0
          @PODS_EMIT_STREAM_BATCH_SIZE = ENV["PODS_EMIT_STREAM_BATCH_SIZE"].to_i
        else
          # this shouldnt happen just setting default here as safe guard
          $log.warn("in_kube_perfinventory::start: setting to default value since got PODS_EMIT_STREAM_BATCH_SIZE nil or empty")
          @PODS_EMIT_STREAM_BATCH_SIZE = 200
        end
        $log.info("in_kube_perfinventory::start: PODS_EMIT_STREAM_BATCH_SIZE  @ #{@PODS_EMIT_STREAM_BATCH_SIZE}")

        if !ENV["NODES_CHUNK_SIZE"].nil? && !ENV["NODES_CHUNK_SIZE"].empty? && ENV["NODES_CHUNK_SIZE"].to_i > 0
          @NODES_CHUNK_SIZE = ENV["NODES_CHUNK_SIZE"].to_i
        else
          # this shouldnt happen just setting default here as safe guard
          $log.warn("in_kube_perfinventory::start: setting to default value since got NODES_CHUNK_SIZE nil or empty")
          @NODES_CHUNK_SIZE = 250
        end
        $log.info("in_kube_perfinventory::start : NODES_CHUNK_SIZE  @ #{@NODES_CHUNK_SIZE}")

        @finished = false
        @condition = ConditionVariable.new
        @mutex = Mutex.new
        @podCacheMutex = Mutex.new
        @nodeAllocatableCacheMutex = Mutex.new
        @thread = Thread.new(&method(:run_periodic))
        @watchNodesThread = Thread.new(&method(:watch_nodes))
        @watchPodsThread = Thread.new(&method(:watch_pods))
      end
    end

    def shutdown
      if @run_interval
        @mutex.synchronize {
          @finished = true
          @condition.signal
        }
        @thread.join
        @watchPodsThread.join
        @watchNodesThread.join
        super # This super must be at the end of shutdown method
      end
    end

    def enumerate(podList = nil)
      begin
        podInventory = podList
        @podCount = 0
        currentTime = Time.now
        batchTime = currentTime.utc.iso8601
        if ExtensionUtils.isAADMSIAuthMode()
          $log.info("in_kube_perfinventory::enumerate: AAD AUTH MSI MODE")
          if @kubeperfTag.nil? || !@kubeperfTag.start_with?(Constants::EXTENSION_OUTPUT_STREAM_ID_TAG_PREFIX)
            @kubeperfTag = ExtensionUtils.getOutputStreamId(Constants::PERF_DATA_TYPE)
          end
          if @insightsMetricsTag.nil? || !@insightsMetricsTag.start_with?(Constants::EXTENSION_OUTPUT_STREAM_ID_TAG_PREFIX)
            @insightsMetricsTag = ExtensionUtils.getOutputStreamId(Constants::INSIGHTS_METRICS_DATA_TYPE)
          end
          $log.info("in_kube_perfinventory::enumerate: using perf tag -#{@kubeperfTag} @ #{Time.now.utc.iso8601}")
          $log.info("in_kube_perfinventory::enumerate: using insightsmetrics tag -#{@insightsMetricsTag} @ #{Time.now.utc.iso8601}")
        end

        nodeAllocatableRecords = {}
        nodeAllocatableCacheSizeKB = 0
        @nodeAllocatableCacheMutex.synchronize {
          nodeAllocatableRecords = @nodeAllocatableCache.clone
        }
        $log.info("in_kube_perfinventory::enumerate : number of nodeAllocatableRecords :#{nodeAllocatableRecords.length} from Kube API @ #{Time.now.utc.iso8601}")
        # Initializing continuation token to nil
        continuationToken = nil
        podItemsCacheSizeKB = 0
        podInventory = {}
        @podCacheMutex.synchronize {
          podInventory["items"] = @podItemsCache.values.clone
        }
        if (!podInventory.nil? && !podInventory.empty? && podInventory.key?("items") && !podInventory["items"].nil? && !podInventory["items"].empty?)
          $log.info("in_kube_perfinventory::enumerate : number of pod items :#{podInventory["items"].length}  from Kube API @ #{Time.now.utc.iso8601}")
          parse_and_emit_records(podInventory, nodeAllocatableRecords, continuationToken, batchTime)
        else
          $log.warn "in_kube_perfinventory::enumerate:Received empty podInventory"
        end
        # Setting these to nil so that we dont hold memory until GC kicks in
        podInventory = nil
        nodeAllocatableRecords = nil
      rescue => errorStr
        $log.warn "in_kube_perfinventory::enumerate:Failed in enumerate: #{errorStr}"
        $log.debug_backtrace(errorStr.backtrace)
        ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
      end
    end

    def parse_and_emit_records(podInventory, nodeAllocatableRecords, continuationToken, batchTime = Time.utc.iso8601)
      currentTime = Time.now
      emitTime = Fluent::Engine.now
      kubePerfEventStream = Fluent::MultiEventStream.new
      insightsMetricsEventStream = Fluent::MultiEventStream.new
      @@istestvar = ENV["ISTEST"]

      begin #begin block start
        # # Getting windows nodes from kubeapi
        # winNodes = KubernetesApiClient.getWindowsNodesArray
        podInventory["items"].each do |item| #podInventory block start
          nodeName = ""
          if !item["spec"]["nodeName"].nil?
            nodeName = item["spec"]["nodeName"]
          end

          nodeAllocatableRecord = {}
          if !nodeName.empty? && !nodeAllocatableRecords.nil? && !nodeAllocatableRecords.empty? && nodeAllocatableRecords.has_key?(nodeName)
            nodeAllocatableRecord = nodeAllocatableRecords[nodeName]
          end
          #container perf records
          containerMetricDataItems = []
          containerMetricDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimits(item, "requests", "cpu", "cpuRequestNanoCores", nodeAllocatableRecord, batchTime))
          containerMetricDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimits(item, "requests", "memory", "memoryRequestBytes", nodeAllocatableRecord, batchTime))
          containerMetricDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimits(item, "limits", "cpu", "cpuLimitNanoCores", nodeAllocatableRecord, batchTime))
          containerMetricDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimits(item, "limits", "memory", "memoryLimitBytes", nodeAllocatableRecord, batchTime))

          containerMetricDataItems.each do |record|
            kubePerfEventStream.add(emitTime, record) if record
          end

          if @PODS_EMIT_STREAM_BATCH_SIZE > 0 && kubePerfEventStream.count >= @PODS_EMIT_STREAM_BATCH_SIZE
            $log.info("in_kube_perfinventory::parse_and_emit_records: number of container perf records emitted #{kubePerfEventStream.count} @ #{Time.now.utc.iso8601}")
            router.emit_stream(@kubeperfTag, kubePerfEventStream) if kubePerfEventStream
            if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0)
              $log.info("kubeContainerPerfEventEmitStreamSuccess @ #{Time.now.utc.iso8601}")
            end
            kubePerfEventStream = Fluent::MultiEventStream.new
          end

          # container GPU records
          containerGPUInsightsMetricsDataItems = []
          containerGPUInsightsMetricsDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimitsAsInsightsMetrics(item, "requests", "nvidia.com/gpu", "containerGpuRequests", nodeAllocatableRecord, batchTime))
          containerGPUInsightsMetricsDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimitsAsInsightsMetrics(item, "limits", "nvidia.com/gpu", "containerGpuLimits", nodeAllocatableRecord, batchTime))
          containerGPUInsightsMetricsDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimitsAsInsightsMetrics(item, "requests", "amd.com/gpu", "containerGpuRequests", nodeAllocatableRecord, batchTime))
          containerGPUInsightsMetricsDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimitsAsInsightsMetrics(item, "limits", "amd.com/gpu", "containerGpuLimits", nodeAllocatableRecord, batchTime))
          containerGPUInsightsMetricsDataItems.each do |insightsMetricsRecord|
            insightsMetricsEventStream.add(emitTime, insightsMetricsRecord) if insightsMetricsRecord
          end

          if @PODS_EMIT_STREAM_BATCH_SIZE > 0 && insightsMetricsEventStream.count >= @PODS_EMIT_STREAM_BATCH_SIZE
            $log.info("in_kube_perfinventory::parse_and_emit_records: number of GPU insights metrics records emitted #{insightsMetricsEventStream.count} @ #{Time.now.utc.iso8601}")
            if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0)
              $log.info("kubePodInsightsMetricsEmitStreamSuccess @ #{Time.now.utc.iso8601}")
            end
            router.emit_stream(@insightsMetricsTag, insightsMetricsEventStream) if insightsMetricsEventStream
            insightsMetricsEventStream = Fluent::MultiEventStream.new
          end
        end  #podInventory block end

        if kubePerfEventStream.count > 0
          $log.info("in_kube_perfinventory::parse_and_emit_records: number of perf records emitted #{kubePerfEventStream.count} @ #{Time.now.utc.iso8601}")
          router.emit_stream(@kubeperfTag, kubePerfEventStream) if kubePerfEventStream
          kubePerfEventStream = nil
          if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0)
            $log.info("kubeContainerPerfEventEmitStreamSuccess @ #{Time.now.utc.iso8601}")
          end
        end

        if insightsMetricsEventStream.count > 0
          $log.info("in_kube_perfinventory::parse_and_emit_records: number of insights metrics records emitted #{insightsMetricsEventStream.count} @ #{Time.now.utc.iso8601}")
          router.emit_stream(@insightsMetricsTag, insightsMetricsEventStream) if insightsMetricsEventStream
          if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0)
            $log.info("kubePodInsightsMetricsEmitStreamSuccess @ #{Time.now.utc.iso8601}")
          end
          insightsMetricsEventStream = nil
        end
      rescue => errorStr
        $log.warn "Failed in parse_and_emit_record kube perf inventory: #{errorStr}"
        $log.debug_backtrace(errorStr.backtrace)
        ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
      end #begin block end
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
            $log.info("in_kube_perfinventory::run_periodic.enumerate.start #{Time.now.utc.iso8601}")
            enumerate
            $log.info("in_kube_perfinventory::run_periodic.enumerate.end #{Time.now.utc.iso8601}")
          rescue => errorStr
            $log.warn "in_kube_perfinventory::run_periodic: enumerate Failed to retrieve perf inventory: #{errorStr}"
            ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
          end
        end
        @mutex.lock
      end
      @mutex.unlock
    end

    def watch_pods
      $log.info("in_kube_perfinventory::watch_pods:Start @ #{Time.now.utc.iso8601}")
      podsResourceVersion = nil
      loop do
        begin
          if podsResourceVersion.nil?
            # clear cache before filling the cache with list
            @podCacheMutex.synchronize {
              @podItemsCache.clear()
            }
            currentWindowsNodeNameList = []
            continuationToken = nil
            $log.info("in_kube_perfinventory::watch_pods:Getting pods from Kube API since podsResourceVersion is #{podsResourceVersion}  @ #{Time.now.utc.iso8601}")
            continuationToken, podInventory = KubernetesApiClient.getResourcesAndContinuationToken("pods?limit=#{@PODS_CHUNK_SIZE}")
            $log.info("in_kube_perfinventory::watch_pods:Done getting pods from Kube API @ #{Time.now.utc.iso8601}")
            if (!podInventory.nil? && !podInventory.empty?)
              podsResourceVersion = podInventory["metadata"]["resourceVersion"]
              if (podInventory.key?("items") && !podInventory["items"].nil? && !podInventory["items"].empty?)
                $log.info("in_kube_perfinventory::watch_pods:number of pod items :#{podInventory["items"].length}  from Kube API @ #{Time.now.utc.iso8601}")
                podInventory["items"].each do |item|
                  key = item["metadata"]["uid"]
                  if !key.nil? && !key.empty?
                    podItem = KubernetesApiClient.getOptimizedItem("pods", item)
                    if !podItem.nil? && !podItem.empty?
                      @podCacheMutex.synchronize {
                        @podItemsCache[key] = podItem
                      }
                    else
                      $log.warn "in_kube_perfinventory::watch_pods:Received podItem either empty or nil  @ #{Time.now.utc.iso8601}"
                    end
                  else
                    $log.warn "in_kube_perfinventory::watch_pods:Received poduid either nil or empty  @ #{Time.now.utc.iso8601}"
                  end
                end
              end
            else
              $log.warn "in_kube_perfinventory::watch_pods:Received empty podInventory"
            end
            while (!continuationToken.nil? && !continuationToken.empty?)
              continuationToken, podInventory = KubernetesApiClient.getResourcesAndContinuationToken("pods?limit=#{@PODS_CHUNK_SIZE}&continue=#{continuationToken}")
              if (!podInventory.nil? && !podInventory.empty?)
                podsResourceVersion = podInventory["metadata"]["resourceVersion"]
                if (podInventory.key?("items") && !podInventory["items"].nil? && !podInventory["items"].empty?)
                  $log.info("in_kube_perfinventory::watch_pods:number of pod items :#{podInventory["items"].length} from Kube API @ #{Time.now.utc.iso8601}")
                  podInventory["items"].each do |item|
                    key = item["metadata"]["uid"]
                    if !key.nil? && !key.empty?
                      podItem = KubernetesApiClient.getOptimizedItem("pods", item)
                      if !podItem.nil? && !podItem.empty?
                        @podCacheMutex.synchronize {
                          @podItemsCache[key] = podItem
                        }
                      else
                        $log.warn "in_kube_perfinventory::watch_pods:Received podItem is empty or nil  @ #{Time.now.utc.iso8601}"
                      end
                    else
                      $log.warn "in_kube_perfinventory::watch_pods:Received poduid either nil or empty  @ #{Time.now.utc.iso8601}"
                    end
                  end
                end
              else
                $log.warn "in_kube_perfinventory::watch_pods:Received empty podInventory  @ #{Time.now.utc.iso8601}"
              end
            end
          end
          begin
            $log.info("in_kube_perfinventory::watch_pods:Establishing Watch connection for pods with resourceversion: #{podsResourceVersion} @ #{Time.now.utc.iso8601}")
            watcher = KubernetesApiClient.watch("pods", resource_version: podsResourceVersion, allow_watch_bookmarks: true)
            if watcher.nil?
              $log.warn("in_kube_perfinventory::watch_pods:watch API returned nil watcher for watch connection with resource version: #{podsResourceVersion} @ #{Time.now.utc.iso8601}")
            else
              watcher.each do |notice|
                case notice["type"]
                when "ADDED", "MODIFIED", "DELETED", "BOOKMARK"
                  item = notice["object"]
                  # extract latest resource version to use for watch reconnect
                  if !item.nil? && !item.empty? &&
                     !item["metadata"].nil? && !item["metadata"].empty? &&
                     !item["metadata"]["resourceVersion"].nil? && !item["metadata"]["resourceVersion"].empty?
                    podsResourceVersion = item["metadata"]["resourceVersion"]
                    $log.info("in_kube_perfinventory::watch_pods:received event type: #{notice["type"]} with resource version: #{podsResourceVersion} @ #{Time.now.utc.iso8601}")
                  else
                    $log.info("in_kube_perfinventory::watch_pods:received event type with no resourceVersion hence stopping watcher to reconnect @ #{Time.now.utc.iso8601}")
                    podsResourceVersion = nil
                    # We have to abort here because this might cause lastResourceVersion inconsistency by skipping a potential RV with valid data!
                    break
                  end
                  if ((notice["type"] == "ADDED") || (notice["type"] == "MODIFIED"))
                    key = item["metadata"]["uid"]
                    if !key.nil? && !key.empty?
                      podItem = KubernetesApiClient.getOptimizedItem("pods", item)
                      if !podItem.nil? && !podItem.empty?
                        @podCacheMutex.synchronize {
                          @podItemsCache[key] = podItem
                        }
                      else
                        $log.warn "in_kube_perfinventory::watch_pods:Received podItem is empty or nil  @ #{Time.now.utc.iso8601}"
                      end
                    else
                      $log.warn "in_kube_perfinventory::watch_pods:Received poduid either nil or empty  @ #{Time.now.utc.iso8601}"
                    end
                  elsif notice["type"] == "DELETED"
                    key = item["metadata"]["uid"]
                    if !key.nil? && !key.empty?
                      @podCacheMutex.synchronize {
                        @podItemsCache.delete(key)
                      }
                    end
                  end
                when "ERROR"
                  podsResourceVersion = nil
                  $log.warn("in_kube_perfinventory::watch_pods:ERROR event with :#{notice["object"]} @ #{Time.now.utc.iso8601}")
                  break
                else
                  $log.warn("in_kube_perfinventory::watch_pods:Unsupported event type #{notice["type"]} @ #{Time.now.utc.iso8601}")
                end
              end
              $log.info("in_kube_perfinventory::watch_pods:Watch connection got disconnected for pods with resourceversion: #{podsResourceVersion} @ #{Time.now.utc.iso8601}")
            end
          rescue Net::ReadTimeout => errorStr
            ## This expected if there is no activity more than readtimeout value used in the connection
            $log.warn("in_kube_perfinventory::watch_pods:Watch failed with an error: #{errorStr} @ #{Time.now.utc.iso8601}")
          rescue => errorStr
            $log.warn("in_kube_perfinventory::watch_pods:Watch failed with an error: #{errorStr} @ #{Time.now.utc.iso8601}")
            podsResourceVersion = nil
            sleep(5) # do not overwhelm the api-server if api-server broken
          ensure
            watcher.finish if watcher
          end
        rescue => errorStr
          $log.warn("in_kube_perfinventory::watch_pods:failed with an error: #{errorStr} @ #{Time.now.utc.iso8601}")
          podsResourceVersion = nil
        end
      end
      $log.info("in_kube_perfinventory::watch_pods:End @ #{Time.now.utc.iso8601}")
    end

    def watch_nodes
      $log.info("in_kube_perfinventory::watch_nodes:Start @ #{Time.now.utc.iso8601}")
      nodesResourceVersion = nil
      loop do
        begin
          if nodesResourceVersion.nil?
            # clear node limits cache before filling the cache with list
            @nodeAllocatableCacheMutex.synchronize {
              @nodeAllocatableCache.clear()
            }
            continuationToken = nil
            $log.info("in_kube_perfinventory::watch_nodes:Getting nodes from Kube API since nodesResourceVersion is #{nodesResourceVersion} @ #{Time.now.utc.iso8601}")
            resourceUri = KubernetesApiClient.getNodesResourceUri("nodes?limit=#{@NODES_CHUNK_SIZE}")
            continuationToken, nodeInventory = KubernetesApiClient.getResourcesAndContinuationToken(resourceUri)
            $log.info("in_kube_perfinventory::watch_nodes:Done getting nodes from Kube API @ #{Time.now.utc.iso8601}")
            if (!nodeInventory.nil? && !nodeInventory.empty?)
              nodesResourceVersion = nodeInventory["metadata"]["resourceVersion"]
              if (nodeInventory.key?("items") && !nodeInventory["items"].nil? && !nodeInventory["items"].empty?)
                $log.info("in_kube_perfinventory::watch_nodes: number of node items :#{nodeInventory["items"].length}  from Kube API @ #{Time.now.utc.iso8601}")
                nodeInventory["items"].each do |item|
                  key = item["metadata"]["name"]
                  if !key.nil? && !key.empty?
                    nodeAllocatable = KubernetesApiClient.getNodeAllocatableValues(item)
                    if !nodeAllocatable.nil? && !nodeAllocatable.empty?
                      @nodeAllocatableCacheMutex.synchronize {
                        @nodeAllocatableCache[key] = nodeAllocatable
                      }
                    else
                      $log.warn "in_kube_perfinventory::watch_nodes:Received nodeItem nil or empty  @ #{Time.now.utc.iso8601}"
                    end
                  else
                    $log.warn "in_kube_perfinventory::watch_nodes:Received node name either nil or empty  @ #{Time.now.utc.iso8601}"
                  end
                end
              end
            else
              $log.warn "in_kube_perfinventory::watch_nodes:Received empty nodeInventory  @ #{Time.now.utc.iso8601}"
            end
            while (!continuationToken.nil? && !continuationToken.empty?)
              continuationToken, nodeInventory = KubernetesApiClient.getResourcesAndContinuationToken(resourceUri + "&continue=#{continuationToken}")
              if (!nodeInventory.nil? && !nodeInventory.empty?)
                nodesResourceVersion = nodeInventory["metadata"]["resourceVersion"]
                if (nodeInventory.key?("items") && !nodeInventory["items"].nil? && !nodeInventory["items"].empty?)
                  $log.info("in_kube_perfinventory::watch_nodes : number of node items :#{nodeInventory["items"].length} from Kube API @ #{Time.now.utc.iso8601}")
                  nodeInventory["items"].each do |item|
                    key = item["metadata"]["name"]
                    if !key.nil? && !key.empty?
                      nodeAllocatable = KubernetesApiClient.getNodeAllocatableValues(item)
                      if !nodeAllocatable.nil? && !nodeAllocatable.empty?
                        @nodeAllocatableCacheMutex.synchronize {
                          @nodeAllocatableCache[key] = nodeAllocatable
                        }
                      else
                        $log.warn "in_kube_perfinventory::watch_nodes:Received nodeItem nil or empty  @ #{Time.now.utc.iso8601}"
                      end
                    else
                      $log.warn "in_kube_perfinventory::watch_nodes:Received node name either nil or empty  @ #{Time.now.utc.iso8601}"
                    end
                  end
                end
              else
                $log.warn "in_kube_perfinventory::watch_nodes:Received empty nodeInventory  @ #{Time.now.utc.iso8601}"
              end
            end
          end
          begin
            $log.info("in_kube_perfinventory::watch_nodes:Establishing Watch connection for nodes with resourceversion: #{nodesResourceVersion} @ #{Time.now.utc.iso8601}")
            watcher = KubernetesApiClient.watch("nodes", resource_version: nodesResourceVersion, allow_watch_bookmarks: true)
            if watcher.nil?
              $log.warn("in_kube_perfinventory::watch_nodes:watch API returned nil watcher for watch connection with resource version: #{nodesResourceVersion} @ #{Time.now.utc.iso8601}")
            else
              watcher.each do |notice|
                case notice["type"]
                when "ADDED", "MODIFIED", "DELETED", "BOOKMARK"
                  item = notice["object"]
                  # extract latest resource version to use for watch reconnect
                  if !item.nil? && !item.empty? &&
                     !item["metadata"].nil? && !item["metadata"].empty? &&
                     !item["metadata"]["resourceVersion"].nil? && !item["metadata"]["resourceVersion"].empty?
                    nodesResourceVersion = item["metadata"]["resourceVersion"]
                    $log.info("in_kube_perfinventory::watch_nodes: received event type: #{notice["type"]} with resource version: #{nodesResourceVersion} @ #{Time.now.utc.iso8601}")
                  else
                    $log.info("in_kube_perfinventory::watch_nodes: received event type with no resourceVersion hence stopping watcher to reconnect @ #{Time.now.utc.iso8601}")
                    nodesResourceVersion = nil
                    # We have to abort here because this might cause lastResourceVersion inconsistency by skipping a potential RV with valid data!
                    break
                  end
                  if ((notice["type"] == "ADDED") || (notice["type"] == "MODIFIED"))
                    key = item["metadata"]["name"]
                    if !key.nil? && !key.empty?
                      nodeAllocatable = KubernetesApiClient.getNodeAllocatableValues(item)
                      if !nodeAllocatable.nil? && !nodeAllocatable.empty?
                        @nodeAllocatableCacheMutex.synchronize {
                          @nodeAllocatableCache[key] = nodeAllocatable
                        }
                      else
                        $log.warn "in_kube_perfinventory::watch_nodes:Received nodeItem nil or empty  @ #{Time.now.utc.iso8601}"
                      end
                    else
                      $log.warn "in_kube_perfinventory::watch_nodes:Received node name either nil or empty  @ #{Time.now.utc.iso8601}"
                    end
                  elsif notice["type"] == "DELETED"
                    key = item["metadata"]["name"]
                    if !key.nil? && !key.empty?
                      @nodeAllocatableCacheMutex.synchronize {
                        @nodeAllocatableCache.delete(key)
                      }
                    end
                  end
                when "ERROR"
                  nodesResourceVersion = nil
                  $log.warn("in_kube_perfinventory::watch_nodes:ERROR event with :#{notice["object"]} @ #{Time.now.utc.iso8601}")
                  break
                else
                  $log.warn("in_kube_perfinventory::watch_nodes:Unsupported event type #{notice["type"]} @ #{Time.now.utc.iso8601}")
                end
              end
            end
          rescue Net::ReadTimeout => errorStr
            $log.warn("in_kube_perfinventory::watch_nodes:failed with an error: #{errorStr} @ #{Time.now.utc.iso8601}")
          rescue => errorStr
            $log.warn("in_kube_perfinventory::watch_nodes:failed with an error: #{errorStr} @ #{Time.now.utc.iso8601}")
            nodesResourceVersion = nil
            sleep(5) # do not overwhelm the api-server if api-server broken
          ensure
            watcher.finish if watcher
          end
        rescue => errorStr
          $log.warn("in_kube_perfinventory::watch_nodes:failed with an error: #{errorStr} @ #{Time.now.utc.iso8601}")
          nodesResourceVersion = nil
        end
      end
      $log.info("in_kube_perfinventory::watch_nodes:End @ #{Time.now.utc.iso8601}")
    end
  end # Kube_Pod_Input
end # module

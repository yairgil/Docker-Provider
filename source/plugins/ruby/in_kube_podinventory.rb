#!/usr/local/bin/ruby
# frozen_string_literal: true

module Fluent
  require_relative "podinventory_to_mdm"

  class Kube_PodInventory_Input < Input
    Plugin.register_input("kubepodinventory", self)

    @@MDMKubePodInventoryTag = "mdm.kubepodinventory"
    @@hostName = (OMS::Common.get_hostname)
    @@kubeperfTag = "oms.api.KubePerf"
    @@kubeservicesTag = "oms.containerinsights.KubeServices"

    def initialize
      super
      require "yaml"
      require "yajl/json_gem"
      require "yajl"
      require "set"
      require "time"

      require_relative "kubernetes_container_inventory"
      require_relative "KubernetesApiClient"
      require_relative "ApplicationInsightsUtility"
      require_relative "oms_common"
      require_relative "omslog"
      require_relative "constants"

      # refer tomlparser-agent-config for updating defaults
      # this configurable via configmap
      @PODS_CHUNK_SIZE = 0
      @PODS_EMIT_STREAM_BATCH_SIZE = 0

      @podCount = 0
      @serviceCount = 0
      @controllerSet = Set.new []
      @winContainerCount = 0
      @controllerData = {}
      @podInventoryE2EProcessingLatencyMs = 0
      @podsAPIE2ELatencyMs = 0
      @isDisableKubeContainerPerf = false
      @isDisableKubeGPUPerf = false
      @isDisableKubeWinContainerInventory = false
      @isDiableKubeServices = false
      @isDisableMDM = false
      @isDisableKPIAll = false
    end

    config_param :run_interval, :time, :default => 60
    config_param :tag, :string, :default => "oms.containerinsights.KubePodInventory"

    def configure(conf)
      super
      @inventoryToMdmConvertor = Inventory2MdmConvertor.new()
    end

    def start
      if @run_interval
        if !ENV["PODS_CHUNK_SIZE"].nil? && !ENV["PODS_CHUNK_SIZE"].empty? && ENV["PODS_CHUNK_SIZE"].to_i > 0
          @PODS_CHUNK_SIZE = ENV["PODS_CHUNK_SIZE"].to_i
        else
          # this shouldnt happen just setting default here as safe guard
          $log.warn("in_kube_podinventory::start: setting to default value since got PODS_CHUNK_SIZE nil or empty")
          @PODS_CHUNK_SIZE = 1000
        end
        $log.info("in_kube_podinventory::start : PODS_CHUNK_SIZE  @ #{@PODS_CHUNK_SIZE} @ #{Time.now.utc.round(10).iso8601(6)}")

        if !ENV["PODS_EMIT_STREAM_BATCH_SIZE"].nil? && !ENV["PODS_EMIT_STREAM_BATCH_SIZE"].empty? && ENV["PODS_EMIT_STREAM_BATCH_SIZE"].to_i > 0
          @PODS_EMIT_STREAM_BATCH_SIZE = ENV["PODS_EMIT_STREAM_BATCH_SIZE"].to_i
        else
          # this shouldnt happen just setting default here as safe guard
          $log.warn("in_kube_podinventory::start: setting to default value since got PODS_EMIT_STREAM_BATCH_SIZE nil or empty")
          @PODS_EMIT_STREAM_BATCH_SIZE = 200
        end
        $log.info("in_kube_podinventory::start : PODS_EMIT_STREAM_BATCH_SIZE  @ #{@PODS_EMIT_STREAM_BATCH_SIZE} @ #{Time.now.utc.round(10).iso8601(6)}")

        if !ENV["DISABLE_KPI_KUBE_CONTAINER_PERF"].nil? && !ENV["DISABLE_KPI_KUBE_CONTAINER_PERF"].empty? && ENV["DISABLE_KPI_KUBE_CONTAINER_PERF"].downcase == "true".downcase
          @isDisableKubeContainerPerf = true
        end
        $log.info("in_kube_podinventory::start : isDisableKubeContainerPerf=#{@isDisableKubeContainerPerf} @ #{Time.now.utc.round(10).iso8601(6)}")

        if !ENV["DISABLE_KPI_KUBE_GPU_PERF"].nil? && !ENV["DISABLE_KPI_KUBE_GPU_PERF"].empty? && ENV["DISABLE_KPI_KUBE_GPU_PERF"].downcase == "true".downcase
          @isDisableKubeGPUPerf = true
        end
        $log.info("in_kube_podinventory::start : isDisableKubeGPUPerf=#{@isDisableKubeGPUPerf} @ #{Time.now.utc.round(10).iso8601(6)}")

        if !ENV["DISABLE_KPI_WIN_CONTAINER_INVENTORY"].nil? && !ENV["DISABLE_KPI_WIN_CONTAINER_INVENTORY"].empty? && ENV["DISABLE_KPI_WIN_CONTAINER_INVENTORY"].downcase == "true".downcase
          @isDisableKubeWinContainerInventory = true
        end
        $log.info("in_kube_podinventory::start : isDisableKubeWinContainerInventory=#{@isDisableKubeWinContainerInventory} @ #{Time.now.utc.round(10).iso8601(6)}")

        if !ENV["DISABLE_KPI_KUBE_SERVICES"].nil? && !ENV["DISABLE_KPI_KUBE_SERVICES"].empty? && ENV["DISABLE_KPI_KUBE_SERVICES"].downcase == "true".downcase
          @isDiableKubeServices = true
        end
        $log.info("in_kube_podinventory::start : isDiableKubeServices=#{@isDiableKubeServices} @ #{Time.now.utc.round(10).iso8601(6)}")

        if !ENV["DISABLE_KPI_MDM"].nil? && !ENV["DISABLE_KPI_MDM"].empty? && ENV["DISABLE_KPI_MDM"].downcase == "true".downcase
          @isDisableMDM = true
        end
        $log.info("in_kube_podinventory::start : isDisableMDM=#{@isDisableMDM} @ #{Time.now.utc.round(10).iso8601(6)}")

        if !ENV["DISABLE_KPI_ALL"].nil? && !ENV["DISABLE_KPI_ALL"].empty? && ENV["DISABLE_KPI_ALL"].downcase == "true".downcase
          @isDisableKPIAll = true
        end
        $log.info("in_kube_podinventory::start : isDisableKPIAll=#{@isDisableKPIAll} @ #{Time.now.utc.round(10).iso8601(6)}")

        @finished = false
        @condition = ConditionVariable.new
        @mutex = Mutex.new
        @thread = Thread.new(&method(:run_periodic))
        @@podTelemetryTimeTracker = DateTime.now.to_time.to_i
      end
    end

    def shutdown
      if @run_interval
        @mutex.synchronize {
          @finished = true
          @condition.signal
        }
        @thread.join
      end
    end

    def enumerate(podList = nil)
      if @isDisableKPIAll
        $log.info("in_kube_podinventory::enumerate: ***isDisableKPIAll*** @ #{Time.now.utc.round(10).iso8601(6)}")
      else
        begin
          podInventory = podList
          telemetryFlush = false
          @podCount = 0
          @serviceCount = 0
          @controllerSet = Set.new []
          @winContainerCount = 0
          @controllerData = {}
          currentTime = Time.now
          batchTime = currentTime.utc.iso8601
          serviceRecords = []
          @podInventoryE2EProcessingLatencyMs = 0
          podInventoryStartTime = (Time.now.to_f * 1000).to_i
          # Get services first so that we dont need to make a call for very chunk
          $log.info("in_kube_podinventory::enumerate : Getting services from Kube API @ #{Time.now.utc.round(10).iso8601(6)}")
          serviceInfo = KubernetesApiClient.getKubeResourceInfo("services")
          # serviceList = JSON.parse(KubernetesApiClient.getKubeResourceInfo("services").body)
          $log.info("in_kube_podinventory::enumerate : Done getting services from Kube API @ #{Time.now.utc.round(10).iso8601(6)}")

          if !serviceInfo.nil?
            $log.info("in_kube_podinventory::enumerate:Start:Parsing services data using yajl @ #{Time.now.utc.round(10).iso8601(6)}")
            serviceList = Yajl::Parser.parse(StringIO.new(serviceInfo.body))
            $log.info("in_kube_podinventory::enumerate:End:Parsing services data using yajl @ #{Time.now.utc.round(10).iso8601(6)}")
            serviceInfo = nil
            # service inventory records much smaller and fixed size compared to serviceList
            $log.info("in_kube_podinventory::enumerate:Start:getKubeServicesInventoryRecords @ #{Time.now.utc.round(10).iso8601(6)}")
            serviceRecords = KubernetesApiClient.getKubeServicesInventoryRecords(serviceList, batchTime)
            # updating for telemetry
            @serviceCount += serviceRecords.length
            serviceList = nil
            $log.info("in_kube_podinventory::enumerate:End:getKubeServicesInventoryRecords @ #{Time.now.utc.round(10).iso8601(6)}")
          end

          # to track e2e processing latency
          @podsAPIE2ELatencyMs = 0
          podsAPIChunkStartTime = (Time.now.to_f * 1000).to_i
          # Initializing continuation token to nil
          continuationToken = nil
          $log.info("in_kube_podinventory::enumerate : Getting pods from Kube API @ #{Time.now.utc.round(10).iso8601(6)}")
          continuationToken, podInventory = KubernetesApiClient.getResourcesAndContinuationToken("pods?limit=#{@PODS_CHUNK_SIZE}")
          $log.info("in_kube_podinventory::enumerate : Done getting pods from Kube API @ #{Time.now.utc.round(10).iso8601(6)}")
          podsAPIChunkEndTime = (Time.now.to_f * 1000).to_i
          @podsAPIE2ELatencyMs = (podsAPIChunkEndTime - podsAPIChunkStartTime)
          if (!podInventory.nil? && !podInventory.empty? && podInventory.key?("items") && !podInventory["items"].nil? && !podInventory["items"].empty?)
            $log.info("in_kube_podinventory::enumerate : number of pod items :#{podInventory["items"].length}  from Kube API @ #{Time.now.utc.round(10).iso8601(6)}")
            $log.info("in_kube_podinventory::enumerate:Start:parse_and_emit_records @ #{Time.now.utc.round(10).iso8601(6)}")
            parse_and_emit_records(podInventory, serviceRecords, continuationToken, batchTime)
            $log.info("in_kube_podinventory::enumerate:End:parse_and_emit_records @ #{Time.now.utc.round(10).iso8601(6)}")
          else
            $log.warn "in_kube_podinventory::enumerate:Received empty podInventory @ #{Time.now.utc.round(10).iso8601(6)}"
          end

          #If we receive a continuation token, make calls, process and flush data until we have processed all data
          while (!continuationToken.nil? && !continuationToken.empty?)
            podsAPIChunkStartTime = (Time.now.to_f * 1000).to_i
            continuationToken, podInventory = KubernetesApiClient.getResourcesAndContinuationToken("pods?limit=#{@PODS_CHUNK_SIZE}&continue=#{continuationToken}")
            podsAPIChunkEndTime = (Time.now.to_f * 1000).to_i
            @podsAPIE2ELatencyMs = @podsAPIE2ELatencyMs + (podsAPIChunkEndTime - podsAPIChunkStartTime)
            if (!podInventory.nil? && !podInventory.empty? && podInventory.key?("items") && !podInventory["items"].nil? && !podInventory["items"].empty?)
              $log.info("in_kube_podinventory::enumerate : number of pod items :#{podInventory["items"].length} from Kube API @ #{Time.now.utc.round(10).iso8601(6)}")
              $log.info("in_kube_podinventory::enumerate:Start:parse_and_emit_records @ #{Time.now.utc.round(10).iso8601(6)}")
              parse_and_emit_records(podInventory, serviceRecords, continuationToken, batchTime)
              $log.info("in_kube_podinventory::enumerate:End:parse_and_emit_records @ #{Time.now.utc.round(10).iso8601(6)}")
            else
              $log.warn "in_kube_podinventory::enumerate:Received empty podInventory @ #{Time.now.utc.round(10).iso8601(6)}"
            end
          end

          @podInventoryE2EProcessingLatencyMs = ((Time.now.to_f * 1000).to_i - podInventoryStartTime)
          # Setting these to nil so that we dont hold memory until GC kicks in
          podInventory = nil
          serviceRecords = nil

          # Adding telemetry to send pod telemetry every 5 minutes
          timeDifference = (DateTime.now.to_time.to_i - @@podTelemetryTimeTracker).abs
          timeDifferenceInMinutes = timeDifference / 60
          if (timeDifferenceInMinutes >= 5)
            telemetryFlush = true
            $log.info("in_kube_podinventory::enumerate:set telemetryFlush to true @ #{Time.now.utc.round(10).iso8601(6)}")
          end

          # Flush AppInsights telemetry once all the processing is done
          if telemetryFlush == true
            $log.info("in_kube_podinventory::enumerate:Start:telemetryFlush @ #{Time.now.utc.round(10).iso8601(6)}")
            telemetryProperties = {}
            telemetryProperties["Computer"] = @@hostName
            telemetryProperties["PODS_CHUNK_SIZE"] = @PODS_CHUNK_SIZE
            telemetryProperties["PODS_EMIT_STREAM_BATCH_SIZE"] = @PODS_EMIT_STREAM_BATCH_SIZE
            ApplicationInsightsUtility.sendCustomEvent("KubePodInventoryHeartBeatEvent", telemetryProperties)
            ApplicationInsightsUtility.sendMetricTelemetry("PodCount", @podCount, {})
            ApplicationInsightsUtility.sendMetricTelemetry("ServiceCount", @serviceCount, {})
            telemetryProperties["ControllerData"] = @controllerData.to_json
            ApplicationInsightsUtility.sendMetricTelemetry("ControllerCount", @controllerSet.length, telemetryProperties)
            if @winContainerCount > 0
              telemetryProperties["ClusterWideWindowsContainersCount"] = @winContainerCount
              ApplicationInsightsUtility.sendCustomEvent("WindowsContainerInventoryEvent", telemetryProperties)
            end
            ApplicationInsightsUtility.sendMetricTelemetry("PodInventoryE2EProcessingLatencyMs", @podInventoryE2EProcessingLatencyMs, telemetryProperties)
            ApplicationInsightsUtility.sendMetricTelemetry("PodsAPIE2ELatencyMs", @podsAPIE2ELatencyMs, telemetryProperties)
            @@podTelemetryTimeTracker = DateTime.now.to_time.to_i
            $log.info("in_kube_podinventory::enumerate:End:telemetryFlush @ #{Time.now.utc.round(10).iso8601(6)}")
          end
        rescue => errorStr
          $log.warn "in_kube_podinventory::enumerate:Failed in enumerate: #{errorStr} @ #{Time.now.utc.round(10).iso8601(6)}"
          $log.debug_backtrace(errorStr.backtrace)
          ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
        end
      end
    end

    def parse_and_emit_records(podInventory, serviceRecords, continuationToken, batchTime = Time.utc.iso8601)
      currentTime = Time.now
      emitTime = currentTime.to_f
      #batchTime = currentTime.utc.iso8601
      eventStream = MultiEventStream.new
      kubePerfEventStream = MultiEventStream.new
      insightsMetricsEventStream = MultiEventStream.new
      @@istestvar = ENV["ISTEST"]

      begin #begin block start
        # Getting windows nodes from kubeapi

        if !@isDisableKubeWinContainerInventory
          $log.info "in_kube_podinventory::parse_and_emit_records:Start:getWindowsNodesArray@ #{Time.now.utc.round(10).iso8601(6)}"
          winNodes = KubernetesApiClient.getWindowsNodesArray
          $log.info "in_kube_podinventory::parse_and_emit_records:End:getWindowsNodesArray @ #{Time.now.utc.round(10).iso8601(6)}"
        end

        podInventory["items"].each do |item| #podInventory block start
          # pod inventory records
          podName = item["metadata"]["name"]
          $log.info "in_kube_podinventory::parse_and_emit_records:Start:getPodInventoryRecords:podName: #{podName} @ #{Time.now.utc.round(10).iso8601(6)}"
          podInventoryRecords = getPodInventoryRecords(item, serviceRecords, batchTime)
          $log.info "in_kube_podinventory::parse_and_emit_records:End:getPodInventoryRecords:podName: #{podName} @ #{Time.now.utc.round(10).iso8601(6)}"

          podInventoryRecords.each do |record|
            $log.info "in_kube_podinventory::parse_and_emit_records:Start:podInventoryRecords.each:PodName: #{podName} @ #{Time.now.utc.round(10).iso8601(6)}"
            if !record.nil?
              wrapper = {
                          "DataType" => "KUBE_POD_INVENTORY_BLOB",
                          "IPName" => "ContainerInsights",
                          "DataItems" => [record.each { |k, v| record[k] = v }],
                        }
              eventStream.add(emitTime, wrapper) if wrapper
              if !@isDisableMDM
                $log.info "in_kube_podinventory::parse_and_emit_records:Start:process_pod_inventory_record - PodName: #{podName} @ #{Time.now.utc.round(10).iso8601(6)}"
                @inventoryToMdmConvertor.process_pod_inventory_record(wrapper)
                $log.info "in_kube_podinventory::parse_and_emit_records:End:process_pod_inventory_record - PodName: #{podName} @ #{Time.now.utc.round(10).iso8601(6)}"
              end
            end
            $log.info "in_kube_podinventory::parse_and_emit_records:End:podInventoryRecords.each:PodName: #{podName} @ #{Time.now.utc.round(10).iso8601(6)}"
          end
          # Setting this flag to true so that we can send ContainerInventory records for containers
          # on windows nodes and parse environment variables for these containers
          if !@isDisableKubeWinContainerInventory
            if winNodes.length > 0
              $log.info "in_kube_podinventory::parse_and_emit_records:Start: get windows container inventory records since winNodes length: #{winNodes.length } @ #{Time.now.utc.round(10).iso8601(6)}"
              nodeName = ""
              if !item["spec"]["nodeName"].nil?
                nodeName = item["spec"]["nodeName"]
              end
              if (!nodeName.empty? && (winNodes.include? nodeName))
                clusterCollectEnvironmentVar = ENV["AZMON_CLUSTER_COLLECT_ENV_VAR"]
                #Generate ContainerInventory records for windows nodes so that we can get image and image tag in property panel
                $log.info "in_kube_podinventory::parse_and_emit_records:getContainerInventoryRecords:start - nodeName: #{nodeName} and podName:#{podName} @ #{Time.now.utc.round(10).iso8601(6)}"
                containerInventoryRecords = KubernetesContainerInventory.getContainerInventoryRecords(item, batchTime, clusterCollectEnvironmentVar, true)
                $log.info "in_kube_podinventory::parse_and_emit_records:getContainerInventoryRecords:end - nodeName: #{nodeName} and podName:#{podName} @ #{Time.now.utc.round(10).iso8601(6)}"

                # Send container inventory records for containers on windows nodes
                @winContainerCount += containerInventoryRecords.length
                containerInventoryRecords.each do |cirecord|
                  if !cirecord.nil?
                    ciwrapper = {
                      "DataType" => "CONTAINER_INVENTORY_BLOB",
                      "IPName" => "ContainerInsights",
                      "DataItems" => [cirecord.each { |k, v| cirecord[k] = v }],
                    }
                    eventStream.add(emitTime, ciwrapper) if ciwrapper
                  end
                end
              end
              $log.info "in_kube_podinventory::parse_and_emit_records:End: get windows container inventory records since winNodes length: #{winNodes.length } @ #{Time.now.utc.round(10).iso8601(6)}"
            end
          end

          if @PODS_EMIT_STREAM_BATCH_SIZE > 0 && eventStream.count >= @PODS_EMIT_STREAM_BATCH_SIZE
            $log.info("in_kube_podinventory::parse_and_emit_records:Start:number of pod and windows container inventory records emitted #{eventStream.count} @ #{Time.now.utc.round(10).iso8601(6)}")
            if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0)
              $log.info("kubePodInventoryEmitStreamSuccess @ #{Time.now.utc.round(10).iso8601(6)}")
            end
            router.emit_stream(@tag, eventStream) if eventStream
            eventStream = MultiEventStream.new
            $log.info("in_kube_podinventory::parse_and_emit_records:End:number of pod and windows container inventory records emitted #{eventStream.count} @ #{Time.now.utc.round(10).iso8601(6)}")
          end

          #container perf records
          containerMetricDataItems = []
          if !@isDisableKubeContainerPerf
            $log.info("in_kube_podinventory::parse_and_emit_records:Start:getContainerResourceRequestsAndLimits @ #{Time.now.utc.round(10).iso8601(6)}")
            containerMetricDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimits(item, "requests", "cpu", "cpuRequestNanoCores", batchTime))
            containerMetricDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimits(item, "requests", "memory", "memoryRequestBytes", batchTime))
            containerMetricDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimits(item, "limits", "cpu", "cpuLimitNanoCores", batchTime))
            containerMetricDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimits(item, "limits", "memory", "memoryLimitBytes", batchTime))
            $log.info("in_kube_podinventory::parse_and_emit_records:End:getContainerResourceRequestsAndLimits- containerMetricDataItems: #{containerMetricDataItems.length} @ #{Time.now.utc.round(10).iso8601(6)}")
          end
          $log.info("in_kube_podinventory::parse_and_emit_records:Start:Adding metric data items to the kubePerfEventStream : #{containerMetricDataItems.length} @ #{Time.now.utc.round(10).iso8601(6)}")

          containerMetricDataItems.each do |record|
            record["DataType"] = "LINUX_PERF_BLOB"
            record["IPName"] = "LogManagement"
            kubePerfEventStream.add(emitTime, record) if record
          end
          $log.info("in_kube_podinventory::parse_and_emit_records:End:Adding metric data items to the kubePerfEventStream : #{containerMetricDataItems.length} @ #{Time.now.utc.round(10).iso8601(6)}")

          if @PODS_EMIT_STREAM_BATCH_SIZE > 0 && kubePerfEventStream.count >= @PODS_EMIT_STREAM_BATCH_SIZE
            $log.info("in_kube_podinventory::parse_and_emit_records:Start: number of container perf records emitted #{@PODS_EMIT_STREAM_BATCH_SIZE} @ #{Time.now.utc.round(10).iso8601(6)}")
            router.emit_stream(@@kubeperfTag, kubePerfEventStream) if kubePerfEventStream
            if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0)
              $log.info("kubeContainerPerfEventEmitStreamSuccess @ #{Time.now.utc.round(10).iso8601(6)}")
            end
            kubePerfEventStream = MultiEventStream.new
            $log.info("in_kube_podinventory::parse_and_emit_records:End:number of container perf records emitted #{@PODS_EMIT_STREAM_BATCH_SIZE} @ #{Time.now.utc.round(10).iso8601(6)}")
          end

          # container GPU records
          containerGPUInsightsMetricsDataItems = []
          if !@isDisableKubeGPUPerf
            $log.info("in_kube_podinventory::parse_and_emit_records:Start:getContainerResourceRequestsAndLimitsAsInsightsMetrics @ #{Time.now.utc.round(10).iso8601(6)}")
            containerGPUInsightsMetricsDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimitsAsInsightsMetrics(item, "requests", "nvidia.com/gpu", "containerGpuRequests", batchTime))
            containerGPUInsightsMetricsDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimitsAsInsightsMetrics(item, "limits", "nvidia.com/gpu", "containerGpuLimits", batchTime))
            containerGPUInsightsMetricsDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimitsAsInsightsMetrics(item, "requests", "amd.com/gpu", "containerGpuRequests", batchTime))
            containerGPUInsightsMetricsDataItems.concat(KubernetesApiClient.getContainerResourceRequestsAndLimitsAsInsightsMetrics(item, "limits", "amd.com/gpu", "containerGpuLimits", batchTime))
            $log.info("in_kube_podinventory::parse_and_emit_records:End:getContainerResourceRequestsAndLimitsAsInsightsMetrics @ #{Time.now.utc.round(10).iso8601(6)}")
          end

          $log.info("in_kube_podinventory::parse_and_emit_records:Start:Adding GPU metric data items to the insightsMetricsEventStream : #{containerGPUInsightsMetricsDataItems.length} @ #{Time.now.utc.round(10).iso8601(6)}")

          containerGPUInsightsMetricsDataItems.each do |insightsMetricsRecord|
            wrapper = {
              "DataType" => "INSIGHTS_METRICS_BLOB",
              "IPName" => "ContainerInsights",
              "DataItems" => [insightsMetricsRecord.each { |k, v| insightsMetricsRecord[k] = v }],
            }
            insightsMetricsEventStream.add(emitTime, wrapper) if wrapper
          end

          $log.info("in_kube_podinventory::parse_and_emit_records:Start:Adding GPU metric data items to the insightsMetricsEventStream : #{containerGPUInsightsMetricsDataItems.length} @ #{Time.now.utc.round(10).iso8601(6)}")

          if @PODS_EMIT_STREAM_BATCH_SIZE > 0 && insightsMetricsEventStream.count >= @PODS_EMIT_STREAM_BATCH_SIZE
            $log.info("in_kube_podinventory::parse_and_emit_records:Start:number of GPU insights metrics records emitted #{insightsMetricsEventStream.count} @ #{Time.now.utc.round(10).iso8601(6)}")
            if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0)
              $log.info("kubePodInsightsMetricsEmitStreamSuccess @ #{Time.now.utc.round(10).iso8601(6)}")
            end
            router.emit_stream(Constants::INSIGHTSMETRICS_FLUENT_TAG, insightsMetricsEventStream) if insightsMetricsEventStream
            insightsMetricsEventStream = MultiEventStream.new
            $log.info("in_kube_podinventory::parse_and_emit_records:End:number of GPU insights metrics records emitted #{insightsMetricsEventStream.count} @ #{Time.now.utc.round(10).iso8601(6)}")
          end
        end  #podInventory block end

        if eventStream.count > 0
          $log.info("in_kube_podinventory::parse_and_emit_records:Start:number of pod and windows container inventory records emitted #{eventStream.count} @ #{Time.now.utc.round(10).iso8601(6)}")
          router.emit_stream(@tag, eventStream) if eventStream
          if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0)
            $log.info("kubePodInventoryEmitStreamSuccess @ #{Time.now.utc.round(10).iso8601(6)}")
          end
          eventStream = nil
          $log.info("in_kube_podinventory::parse_and_emit_records:End:number of pod and windows container inventory records emitted #{eventStream.count} @ #{Time.now.utc.round(10).iso8601(6)}")
        end

        if kubePerfEventStream.count > 0
          $log.info("in_kube_podinventory::parse_and_emit_records:Start:number of perf records emitted #{kubePerfEventStream.count} @ #{Time.now.utc.round(10).iso8601(6)}")
          router.emit_stream(@@kubeperfTag, kubePerfEventStream) if kubePerfEventStream
          kubePerfEventStream = nil
          if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0)
            $log.info("kubeContainerPerfEventEmitStreamSuccess @ #{Time.now.utc.round(10).iso8601(6)}")
          end
          $log.info("in_kube_podinventory::parse_and_emit_records:End:number of perf records emitted #{kubePerfEventStream.count} @ #{Time.now.utc.round(10).iso8601(6)}")
        end

        if insightsMetricsEventStream.count > 0
          $log.info("in_kube_podinventory::parse_and_emit_records:Start:number of insights metrics records emitted #{insightsMetricsEventStream.count} @ #{Time.now.utc.round(10).iso8601(6)}")
          router.emit_stream(Constants::INSIGHTSMETRICS_FLUENT_TAG, insightsMetricsEventStream) if insightsMetricsEventStream
          if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0)
            $log.info("kubePodInsightsMetricsEmitStreamSuccess @ #{Time.now.utc.round(10).iso8601(6)}")
          end
          insightsMetricsEventStream = nil
          $log.info("in_kube_podinventory::parse_and_emit_records:End:number of insights metrics records emitted #{insightsMetricsEventStream.count} @ #{Time.now.utc.round(10).iso8601(6)}")
        end

        if continuationToken.nil? #no more chunks in this batch to be sent, get all mdm pod inventory records to send
          if !@isDisableMDM
            @log.info "START: Sending pod inventory mdm records to out_mdm @ #{Time.now.utc.round(10).iso8601(6)}"
            pod_inventory_mdm_records = @inventoryToMdmConvertor.get_pod_inventory_mdm_records(batchTime)
            @log.info "pod_inventory_mdm_records.size #{pod_inventory_mdm_records.size}"
            mdm_pod_inventory_es = MultiEventStream.new
            pod_inventory_mdm_records.each { |pod_inventory_mdm_record|
              mdm_pod_inventory_es.add(batchTime, pod_inventory_mdm_record) if pod_inventory_mdm_record
            } if pod_inventory_mdm_records
            router.emit_stream(@@MDMKubePodInventoryTag, mdm_pod_inventory_es) if mdm_pod_inventory_es
            @log.info "END: Sending pod inventory mdm records to out_mdm @ #{Time.now.utc.round(10).iso8601(6)}"
         end
        end

        if continuationToken.nil? # sending kube services inventory records
          if !@isDiableKubeServices
            @log.info "START: Sending kube services inventory records @ #{Time.now.utc.round(10).iso8601(6)}"
            kubeServicesEventStream = MultiEventStream.new
            serviceRecords.each do |kubeServiceRecord|
              if !kubeServiceRecord.nil?
                # adding before emit to reduce memory foot print
                kubeServiceRecord["ClusterId"] = KubernetesApiClient.getClusterId
                kubeServiceRecord["ClusterName"] = KubernetesApiClient.getClusterName
                kubeServicewrapper = {
                  "DataType" => "KUBE_SERVICES_BLOB",
                  "IPName" => "ContainerInsights",
                  "DataItems" => [kubeServiceRecord.each { |k, v| kubeServiceRecord[k] = v }],
                }
                kubeServicesEventStream.add(emitTime, kubeServicewrapper) if kubeServicewrapper
                if @PODS_EMIT_STREAM_BATCH_SIZE > 0 && kubeServicesEventStream.count >= @PODS_EMIT_STREAM_BATCH_SIZE
                  $log.info("in_kube_podinventory::parse_and_emit_records: number of service records emitted #{@PODS_EMIT_STREAM_BATCH_SIZE} @ #{Time.now.utc.round(10).iso8601(6)}")
                  router.emit_stream(@@kubeservicesTag, kubeServicesEventStream) if kubeServicesEventStream
                  kubeServicesEventStream = MultiEventStream.new
                  if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0)
                    $log.info("kubeServicesEventEmitStreamSuccess @ #{Time.now.utc.round(10).iso8601(6)}")
                  end
                end
              end
            end
          end

          if kubeServicesEventStream.count > 0
            $log.info("in_kube_podinventory::parse_and_emit_records : number of service records emitted #{kubeServicesEventStream.count} @ #{Time.now.utc.round(10).iso8601(6)}")
            router.emit_stream(@@kubeservicesTag, kubeServicesEventStream) if kubeServicesEventStream
            if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0)
              $log.info("kubeServicesEventEmitStreamSuccess @ #{Time.now.utc.round(10).iso8601(6)}")
            end
          end
          kubeServicesEventStream = nil
          @log.info "END: Sending kube services inventory records @ #{Time.now.utc.round(10).iso8601(6)}"
        end

        #Updating value for AppInsights telemetry
        @podCount += podInventory["items"].length
      rescue => errorStr
        $log.warn "Failed in parse_and_emit_record pod inventory: #{errorStr}"
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
            $log.info("in_kube_podinventory::run_periodic.enumerate.start #{Time.now.utc.round(10).iso8601(6)}")
            enumerate
            $log.info("in_kube_podinventory::run_periodic.enumerate.end #{Time.now.utc.round(10).iso8601(6)}")
          rescue => errorStr
            $log.warn "in_kube_podinventory::run_periodic: enumerate Failed to retrieve pod inventory: #{errorStr}"
            ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
          end
        end
        @mutex.lock
      end
      @mutex.unlock
    end

    # TODO - move this method to KubernetesClient or helper class
    def getPodInventoryRecords(item, serviceRecords, batchTime = Time.utc.iso8601)
      records = []
      record = {}
      $log.info "in_kube_podinventory::getPodInventoryRecords:start @ #{Time.now.utc.round(10).iso8601(6)}"
      begin
        record["CollectionTime"] = batchTime #This is the time that is mapped to become TimeGenerated
        record["Name"] = item["metadata"]["name"]
        podNameSpace = item["metadata"]["namespace"]
        $log.info "in_kube_podinventory::getPodInventoryRecords:getPodUid - start @ #{Time.now.utc.round(10).iso8601(6)}"
        podUid = KubernetesApiClient.getPodUid(podNameSpace, item["metadata"])
        $log.info "in_kube_podinventory::getPodInventoryRecords:getPodUid - end @ #{Time.now.utc.round(10).iso8601(6)}"
        if podUid.nil?
          return records
        end

        nodeName = ""
        #for unscheduled (non-started) pods nodeName does NOT exist
        if !item["spec"]["nodeName"].nil?
          nodeName = item["spec"]["nodeName"]
        end
        # For ARO v3 cluster, skip the pods scheduled on to master or infra nodes
        $log.info "in_kube_podinventory::getPodInventoryRecords:isAROv3MasterOrInfraPod - start @ #{Time.now.utc.round(10).iso8601(6)}"
        if KubernetesApiClient.isAROv3MasterOrInfraPod(nodeName)
          return records
        end
        $log.info "in_kube_podinventory::getPodInventoryRecords:isAROv3MasterOrInfraPod - end @ #{Time.now.utc.round(10).iso8601(6)}"

        record["PodUid"] = podUid
        record["PodLabel"] = [item["metadata"]["labels"]]
        record["Namespace"] = podNameSpace
        record["PodCreationTimeStamp"] = item["metadata"]["creationTimestamp"]
        #for unscheduled (non-started) pods startTime does NOT exist
        if !item["status"]["startTime"].nil?
          record["PodStartTime"] = item["status"]["startTime"]
        else
          record["PodStartTime"] = ""
        end
        #podStatus
        # the below is for accounting 'NodeLost' scenario, where-in the pod(s) in the lost node is still being reported as running
        podReadyCondition = true
        if !item["status"]["reason"].nil? && item["status"]["reason"] == "NodeLost" && !item["status"]["conditions"].nil?
          item["status"]["conditions"].each do |condition|
            if condition["type"] == "Ready" && condition["status"] == "False"
              podReadyCondition = false
              break
            end
          end
        end
        if podReadyCondition == false
          record["PodStatus"] = "Unknown"
          # ICM - https://portal.microsofticm.com/imp/v3/incidents/details/187091803/home
        elsif !item["metadata"]["deletionTimestamp"].nil? && !item["metadata"]["deletionTimestamp"].empty?
          record["PodStatus"] = Constants::POD_STATUS_TERMINATING
        else
          record["PodStatus"] = item["status"]["phase"]
        end
        #for unscheduled (non-started) pods podIP does NOT exist
        if !item["status"]["podIP"].nil?
          record["PodIp"] = item["status"]["podIP"]
        else
          record["PodIp"] = ""
        end

        $log.info "in_kube_podinventory::getPodInventoryRecords:get clusterId and name from KubernetesApiClient - start @ #{Time.now.utc.round(10).iso8601(6)}"

        record["Computer"] = nodeName
        record["ClusterId"] = KubernetesApiClient.getClusterId
        record["ClusterName"] = KubernetesApiClient.getClusterName

        $log.info "in_kube_podinventory::getPodInventoryRecords:get clusterId and name from KubernetesApiClient - end @ #{Time.now.utc.round(10).iso8601(6)}"

        record["ServiceName"] = getServiceNameFromLabels(item["metadata"]["namespace"], item["metadata"]["labels"], serviceRecords)

        if !item["metadata"]["ownerReferences"].nil?
          record["ControllerKind"] = item["metadata"]["ownerReferences"][0]["kind"]
          record["ControllerName"] = item["metadata"]["ownerReferences"][0]["name"]
          @controllerSet.add(record["ControllerKind"] + record["ControllerName"])
          #Adding controller kind to telemetry ro information about customer workload
          if (@controllerData[record["ControllerKind"]].nil?)
            @controllerData[record["ControllerKind"]] = 1
          else
            controllerValue = @controllerData[record["ControllerKind"]]
            @controllerData[record["ControllerKind"]] += 1
          end
        end
        podRestartCount = 0
        record["PodRestartCount"] = 0

        if !@isDisableMDM
          $log.info "in_kube_podinventory::getPodInventoryRecords:process_record_for_pods_ready_metric - start @ #{Time.now.utc.round(10).iso8601(6)}"
          #Invoke the helper method to compute ready/not ready mdm metric
          @inventoryToMdmConvertor.process_record_for_pods_ready_metric(record["ControllerName"], record["Namespace"], item["status"]["conditions"])
          $log.info "in_kube_podinventory::getPodInventoryRecords:process_record_for_pods_ready_metric - end @ #{Time.now.utc.round(10).iso8601(6)}"
        end

        podContainers = []
        if item["status"].key?("containerStatuses") && !item["status"]["containerStatuses"].empty?
          podContainers = podContainers + item["status"]["containerStatuses"]
        end
        # Adding init containers to the record list as well.
        if item["status"].key?("initContainerStatuses") && !item["status"]["initContainerStatuses"].empty?
          podContainers = podContainers + item["status"]["initContainerStatuses"]
        end
        # if items["status"].key?("containerStatuses") && !items["status"]["containerStatuses"].empty? #container status block start
        if !podContainers.empty? #container status block start
          podContainers.each do |container|
            containerRestartCount = 0
            lastFinishedTime = nil
            # Need this flag to determine if we need to process container data for mdm metrics like oomkilled and container restart
            #container Id is of the form
            #docker://dfd9da983f1fd27432fb2c1fe3049c0a1d25b1c697b2dc1a530c986e58b16527
            if !container["containerID"].nil?
              record["ContainerID"] = container["containerID"].split("//")[1]
            else
              # for containers that have image issues (like invalid image/tag etc..) this will be empty. do not make it all 0
              record["ContainerID"] = ""
            end
            #keeping this as <PodUid/container_name> which is same as InstanceName in perf table
            if podUid.nil? || container["name"].nil?
              next
            else
              record["ContainerName"] = podUid + "/" + container["name"]
            end
            #Pod restart count is a sumtotal of restart counts of individual containers
            #within the pod. The restart count of a container is maintained by kubernetes
            #itself in the form of a container label.
            containerRestartCount = container["restartCount"]
            record["ContainerRestartCount"] = containerRestartCount

            containerStatus = container["state"]
            record["ContainerStatusReason"] = ""
            # state is of the following form , so just picking up the first key name
            # "state": {
            #   "waiting": {
            #     "reason": "CrashLoopBackOff",
            #      "message": "Back-off 5m0s restarting failed container=metrics-server pod=metrics-server-2011498749-3g453_kube-system(5953be5f-fcae-11e7-a356-000d3ae0e432)"
            #   }
            # },
            # the below is for accounting 'NodeLost' scenario, where-in the containers in the lost node/pod(s) is still being reported as running
            if podReadyCondition == false
              record["ContainerStatus"] = "Unknown"
            else
              record["ContainerStatus"] = containerStatus.keys[0]
            end
            #TODO : Remove ContainerCreationTimeStamp from here since we are sending it as a metric
            #Picking up both container and node start time from cAdvisor to be consistent
            if containerStatus.keys[0] == "running"
              record["ContainerCreationTimeStamp"] = container["state"]["running"]["startedAt"]
            else
              if !containerStatus[containerStatus.keys[0]]["reason"].nil? && !containerStatus[containerStatus.keys[0]]["reason"].empty?
                record["ContainerStatusReason"] = containerStatus[containerStatus.keys[0]]["reason"]
              end
              # Process the record to see if job was completed 6 hours ago. If so, send metric to mdm
              if !record["ControllerKind"].nil? && record["ControllerKind"].downcase == Constants::CONTROLLER_KIND_JOB
                if !@isDisableMDM
                  $log.info "in_kube_podinventory::getPodInventoryRecords:process_record_for_terminated_job_metric - start @ #{Time.now.utc.round(10).iso8601(6)}"
                  @inventoryToMdmConvertor.process_record_for_terminated_job_metric(record["ControllerName"], record["Namespace"], containerStatus)
                  $log.info "in_kube_podinventory::getPodInventoryRecords:process_record_for_terminated_job_metric - end @ #{Time.now.utc.round(10).iso8601(6)}"
                end
              end
            end

            # Record the last state of the container. This may have information on why a container was killed.
            begin
              if !container["lastState"].nil? && container["lastState"].keys.length == 1
                lastStateName = container["lastState"].keys[0]
                lastStateObject = container["lastState"][lastStateName]
                if !lastStateObject.is_a?(Hash)
                  raise "expected a hash object. This could signify a bug or a kubernetes API change"
                end

                if lastStateObject.key?("reason") && lastStateObject.key?("startedAt") && lastStateObject.key?("finishedAt")
                  newRecord = Hash.new
                  newRecord["lastState"] = lastStateName  # get the name of the last state (ex: terminated)
                  lastStateReason = lastStateObject["reason"]
                  # newRecord["reason"] = lastStateObject["reason"]  # (ex: OOMKilled)
                  newRecord["reason"] = lastStateReason  # (ex: OOMKilled)
                  newRecord["startedAt"] = lastStateObject["startedAt"]  # (ex: 2019-07-02T14:58:51Z)
                  lastFinishedTime = lastStateObject["finishedAt"]
                  newRecord["finishedAt"] = lastFinishedTime  # (ex: 2019-07-02T14:58:52Z)

                  # only write to the output field if everything previously ran without error
                  record["ContainerLastStatus"] = newRecord

                  #Populate mdm metric for OOMKilled container count if lastStateReason is OOMKilled
                  if lastStateReason.downcase == Constants::REASON_OOM_KILLED
                    if !@isDisableMDM
                      $log.info "in_kube_podinventory::getPodInventoryRecords:process_record_for_oom_killed_metric - start @ #{Time.now.utc.round(10).iso8601(6)}"
                      @inventoryToMdmConvertor.process_record_for_oom_killed_metric(record["ControllerName"], record["Namespace"], lastFinishedTime)
                      $log.info "in_kube_podinventory::getPodInventoryRecords:process_record_for_oom_killed_metric - end @ #{Time.now.utc.round(10).iso8601(6)}"
                    end
                  end
                  lastStateReason = nil
                else
                  record["ContainerLastStatus"] = Hash.new
                end
              else
                record["ContainerLastStatus"] = Hash.new
              end

              #Populate mdm metric for container restart count if greater than 0
              if (!containerRestartCount.nil? && (containerRestartCount.is_a? Integer) && containerRestartCount > 0)
                if !@isDisableMDM
                  $log.info "in_kube_podinventory::getPodInventoryRecords:process_record_for_container_restarts_metric - start @ #{Time.now.utc.round(10).iso8601(6)}"
                  @inventoryToMdmConvertor.process_record_for_container_restarts_metric(record["ControllerName"], record["Namespace"], lastFinishedTime)
                  $log.info "in_kube_podinventory::getPodInventoryRecords:process_record_for_container_restarts_metric - end @ #{Time.now.utc.round(10).iso8601(6)}"
                end
              end
            rescue => errorStr
              $log.warn "Failed in parse_and_emit_record pod inventory while processing ContainerLastStatus: #{errorStr}"
              $log.debug_backtrace(errorStr.backtrace)
              ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
              record["ContainerLastStatus"] = Hash.new
            end

            podRestartCount += containerRestartCount
            records.push(record.dup)
          end
        else # for unscheduled pods there are no status.containerStatuses, in this case we still want the pod
          records.push(record)
        end  #container status block end

        records.each do |record|
          if !record.nil?
            record["PodRestartCount"] = podRestartCount
          end
        end
      rescue => error
        $log.warn("getPodInventoryRecords failed: #{error}")
      end
      $log.info "in_kube_podinventory::getPodInventoryRecords:end @ #{Time.now.utc.round(10).iso8601(6)}"
      return records
    end

    # TODO - move this method to KubernetesClient or helper class
    def getServiceNameFromLabels(namespace, labels, serviceRecords)
      serviceName = ""
      begin
        if !labels.nil? && !labels.empty?
          serviceRecords.each do |kubeServiceRecord|
            found = 0
            if kubeServiceRecord["Namespace"] == namespace
              selectorLabels = {}
              # selector labels wrapped in array in kube service records so unwrapping here
              if !kubeServiceRecord["SelectorLabels"].nil? && kubeServiceRecord["SelectorLabels"].length > 0
                selectorLabels = kubeServiceRecord["SelectorLabels"][0]
              end
              if !selectorLabels.nil? && !selectorLabels.empty?
                selectorLabels.each do |key, value|
                  if !(labels.select { |k, v| k == key && v == value }.length > 0)
                    break
                  end
                  found = found + 1
                end
                # service can have no selectors
                if found == selectorLabels.length
                  return kubeServiceRecord["ServiceName"]
                end
              end
            end
          end
        end
      rescue => errorStr
        $log.warn "Failed to retrieve service name from labels: #{errorStr}"
        $log.debug_backtrace(errorStr.backtrace)
        ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
      end
      return serviceName
    end
  end # Kube_Pod_Input
end # module

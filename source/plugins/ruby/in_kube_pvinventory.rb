module Fluent
  class Kube_PVInventory_Input < Input
    Plugin.register_input("kubepvinventory", self)

    @@MDMKubePVInventoryTag = "mdm.kubepvinventory"
    @@hostName = (OMS::Common.get_hostname)

    def initialize
      super
      require "yaml"
      require "yajl/json_gem"
      require "yajl"
      require "time"
      require_relative "KubernetesApiClient"
      require_relative "ApplicationInsightsUtility"
      require_relative "oms_common"
      require_relative "omslog"
      require_relative "constants"

      @PV_CHUNK_SIZE = "5000"
      @pvKindToCountHash = {}
    end

    config_param :run_interval, :time, :default => 60
    config_param :tag, :string, :default => "oms.containerinsights.KubePVInventory"

    def configure(conf)
      super
    end

    def start
      if @run_interval
        @finished = false
        @condition = ConditionVariable.new
        @mutex = Mutex.new
        @thread = Thread.new(&method(:run_periodic))
        @@pvTelemetryTimeTracker = DateTime.now.to_time.to_i
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

    def enumerate
      begin
        pvInventory = nil
        telemetryFlush = false
        @pvKindToCountHash = {}
        currentTime = Time.now
        batchTime = currentTime.utc.iso8601

        #continuationToken = nil
        #$log.info("in_kube_pvinventory::enumerate : Getting PVs from Kube API @ #{Time.now.utc.iso8601}")
        #continuationToken, pvInventory = KubernetesApiClient.getResourcesAndContinuationToken("persistentvolumes?limit=#{@PV_CHUNK_SIZE}")
        #$log.info("in_kube_pvinventory::enumerate : Done getting PVs from Kube API @ #{Time.now.utc.iso8601}")

        pvInfo = nil
        $log.info("in_kube_pvinventory::enumerate : Getting PVs from Kube API @ #{Time.now.utc.iso8601}")
        pvInfo = KubernetesApiClient.getKubeResourceInfo("persistentvolumes")
        $log.info("in_kube_pvinventory::enumerate : Done getting PVs from Kube API @ #{Time.now.utc.iso8601}")

        if !pvInfo.nil?
          $log.info("in_kube_pvinventory::enumerate : Response size of #{pvInfo.size}")
          $log.info("in_kube_pvinventory::enumerate : Response header size of #{pvInfo.header.size}")
          $log.info("in_kube_pvinventory::enumerate : Response body size of #{pvInfo.body.size}")
          $log.info("in_kube_pvinventory::enumerate:Start:Parsing pvc data using yajl @ #{Time.now.utc.iso8601}")
          pvInventory = Yajl::Parser.parse(StringIO.new(pvInfo.body))
          $log.info("in_kube_pvinventory::enumerate:End:Parsing pvc data using yajl @ #{Time.now.utc.iso8601}")
          pvInfo = nil
        end

        if (!pvInventory.nil? && !pvInventory.empty? && pvInventory.key?("items") && !pvInventory["items"].nil? && !pvInventory["items"].empty?)
          parse_and_emit_records(pvInventory, batchTime)
        else
          $log.warn "in_kube_pvinventory::enumerate:Received empty pvInventory"
        end

        # If we receive a continuation token, make calls, process and flush data until we have processed all data
        while (!continuationToken.nil? && !continuationToken.empty?)
          continuationToken, pvInventory = KubernetesApiClient.getResourcesAndContinuationToken("persistentvolumes?limit=#{@PV_CHUNK_SIZE}&continue=#{continuationToken}")
          if (!pvInventory.nil? && !pvInventory.empty? && pvInventory.key?("items") && !pvInventory["items"].nil? && !pvInventory["items"].empty?)
            parse_and_emit_records(pvInventory, batchTime)
          else
            $log.warn "in_kube_pvinventory::enumerate:Received empty pvInventory"
          end
        end

        # Setting this to nil so that we dont hold memory until GC kicks in
        pvInventory = nil

        # Adding telemetry to send pod telemetry every 5 minutes
        timeDifference = (DateTime.now.to_time.to_i - @@pvTelemetryTimeTracker).abs
        timeDifferenceInMinutes = timeDifference / 60
        if (timeDifferenceInMinutes >= 5)
          telemetryFlush = true
        end
        
        # Flush AppInsights telemetry once all the processing is done
        if telemetryFlush == true
          telemetryProperties = {}
          telemetryProperties["Computer"] = @@hostName
          telemetryProperties["CountsOfPVKinds"] = @pvKindToCountHash
          ApplicationInsightsUtility.sendCustomEvent("KubePVInventoryHeartBeatEvent", telemetryProperties)
          @@pvTelemetryTimeTracker = DateTime.now.to_time.to_i
        end

      rescue => errorStr
        $log.warn "in_kube_pvinventory::enumerate:Failed in enumerate: #{errorStr}"
        $log.debug_backtrace(errorStr.backtrace)
        ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
      end
    end # end enumerate

    def parse_and_emit_records(pvInventory, batchTime = Time.utc.iso8601)
      currentTime = Time.now
      emitTime = currentTime.to_f
      eventStream = MultiEventStream.new
      @@istestvar = ENV["ISTEST"]

      begin
        records = []
        pvInventory["items"].each do |item|

          # Check if the PV has a PVC
          hasPVC = false
          if !item["spec"].nil? && !item["spec"]["claimRef"].nil?
            claimRef = item["spec"]["claimRef"]
            if claimRef["kind"] == "PersistentVolumeClaim"
              hasPVC = true
              namespace = claimRef["namespace"]
              pvcName = claimRef["name"]
            end
          end
          # Return if no PVC
          if !hasPVC
            return records
          end

          # Check if the PV is an Azure Disk
          isAzureDisk = false
          if !item["spec"].nil? && !item["spec"]["azureDisk"].nil?
            isAzureDisk = true
            azureDisk = item["spec"]["azureDisk"]
            diskName = azureDisk["diskName"]
            diskUri = azureDisk["diskURI"]
          end

          # Get telemetry on PV kind
          if !item["metadata"].nil? && !item["metadata"]["annotations"].nil? && !item["metadata"]["annotations"]["pv.kubernetes.io/provisioned-by"].nil?
            kind = item["metadata"]["annotations"]["pv.kubernetes.io/provisioned-by"].downcase
            if (@pvKindToCountHash.has_key? kind)
              @pvKindToCountHash[kind] += 1
            else
              @pvKindToCountHash[kind] = 1
            end
          end

          metricItem = {}
          metricItem["CollectionTime"] = batchTime
          metricItem["Computer"] = @@hostName
          metricItem["Name"] = "pvInventory"
          metricItem["Value"] = 0
          metricItem["Origin"] = Constants::INSIGHTSMETRICS_TAGS_ORIGIN
          metricItem["Namespace"] = Constants::INSIGTHTSMETRICS_TAGS_PV_NAMESPACE

          metricTags = {}
          metricTags[Constants::INSIGHTSMETRICS_TAGS_CLUSTERID] = KubernetesApiClient.getClusterId
          metricTags[Constants::INSIGHTSMETRICS_TAGS_CLUSTERNAME] = KubernetesApiClient.getClusterName
          metricTags["PVName"] = item["metadata"]["name"]
          metricTags["PVCName"] = pvcName
          metricTags["PVCNamespace"] = namespace
          metricTags["CreationTimeStamp"] = item["metadata"]["creationTimestamp"]
          metricTags["Kind"] = kind
          metricTags["StorageClassName"] = item["spec"]["storageClassName"]
          metricTags["Status"] = item["status"]["phase"]
          metricTags["AccessMode"] = item["spec"]["accessModes"]
          metricTags["RequestSize"] = item["spec"]["capacity"]["storage"]

          if isAzureDisk
            metricTags["DiskName"] = diskName
            metricTags["DiskURI"] = diskUri
          end

          metricItem["Tags"] = metricTags
          records.push(metricItem)
        end

        $log.info "pvKindToCountHash: #{@pvKindToCountHash}"

        records.each do |record|
          if !record.nil?
            wrapper = {
              "DataType" => "INSIGHTS_METRICS_BLOB",
              "IPName" => "ContainerInsights",
              "DataItems" => [record.each { |k, v| record[k] = v }],
            }
            eventStream.add(emitTime, wrapper) if wrapper
          end
        end

        #router.emit_stream(@tag, eventStream) if eventStream
        router.emit_stream(Constants::INSIGHTSMETRICS_FLUENT_TAG, eventStream) if eventStream

      rescue => errorStr
        $log.warn "Failed in parse_and_emit_record pv inventory: #{errorStr}"
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
            $log.info("in_kube_pvinventory::run_periodic.enumerate.start #{Time.now.utc.iso8601}")
            enumerate
            $log.info("in_kube_pvinventory::run_periodic.enumerate.end #{Time.now.utc.iso8601}")
          rescue => errorStr
            $log.warn "in_kube_pvinventory::run_periodic: enumerate Failed to retrieve pod inventory: #{errorStr}"
            ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
          end
        end
        @mutex.lock
      end
      @mutex.unlock
    end

  end # Kube_PVInventory_Input
end # module
module Fluent
  class Kube_PVInventory_Input < Input
    Plugin.register_input("kubepvinventory", self)

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
        @pvTypeToCountHash = {}
        currentTime = Time.now
        batchTime = currentTime.utc.iso8601

        continuationToken = nil
        $log.info("in_kube_pvinventory::enumerate : Getting PVs from Kube API @ #{Time.now.utc.iso8601}")
        continuationToken, pvInventory = KubernetesApiClient.getResourcesAndContinuationToken("persistentvolumes?limit=#{@PV_CHUNK_SIZE}")
        $log.info("in_kube_pvinventory::enumerate : Done getting PVs from Kube API @ #{Time.now.utc.iso8601}")

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

        # Adding telemetry to send pod telemetry every 10 minutes
        timeDifference = (DateTime.now.to_time.to_i - @@pvTelemetryTimeTracker).abs
        timeDifferenceInMinutes = timeDifference / 60
        if (timeDifferenceInMinutes >= Constants::TELEMETRY_FLUSH_INTERVAL_IN_MINUTES)
          telemetryFlush = true
        end
        
        # Flush AppInsights telemetry once all the processing is done
        if telemetryFlush == true
          telemetryProperties = {}
          telemetryProperties["CountsOfPVTypes"] = @pvKindToCountHash
          ApplicationInsightsUtility.sendCustomEvent(Constants::PV_INVENTORY_HEART_BEAT_EVENT, telemetryProperties)
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

      begin
        records = []
        pvInventory["items"].each do |item|

          # Check if the PV has a PVC
          hasPVC = false
          if !item["spec"].nil? && !item["spec"]["claimRef"].nil?
            claimRef = item["spec"]["claimRef"]
            if claimRef["kind"] == "PersistentVolumeClaim"
              hasPVC = true
              pvcNamespace = claimRef["namespace"]
              pvcName = claimRef["name"]
            end
          end

          # Determine PV Type
          type = "empty"
          hasType = false
          isAzureDisk = false
          isAzureFile = false
          if !item["spec"].nil?
            Constants::PV_TYPE.each do |pvType|

              # PV is this type
              if !item["spec"][pvType].nil?
                type = pvType
                hasType = true

                # Get additional info if azure disk/file
                if pvType == "azureDisk"
                  isAzureDisk = true
                  azureDisk = item["spec"]["azureDisk"]
                  diskName = azureDisk["diskName"]
                  diskUri = azureDisk["diskURI"]
                elsif pvType == "azureFile"
                  isAzureFile = true
                  azureFileShareName = item["spec"]["azureFile"]["shareName"]
                end

              end
            end
          end

          # Record telemetry
          if (@pvTypeToCountHash.has_key? type)
            @pvTypeToCountHash[type] += 1
          else
            @pvTypeToCountHash[type] = 1
          end

          # Node and Pod info can be found by joining with pvUsedBytes metric using PVCNamespace/PVCName
          record = {}
          record["CollectionTime"] = batchTime
          record["ClusterId"] = KubernetesApiClient.getClusterId
          record["ClusterName"] = KubernetesApiClient.getClusterName

          record["Name"] = "pvInventory"
          record["Origin"] = Constants::INSIGHTSMETRICS_TAGS_ORIGIN 
          record["Namespace"] = Constants::INSIGTHTSMETRICS_TAGS_PV_NAMESPACE
          record["Computer"] = @@hostName

          recordTags = {}
          recordTags["PVName"] = item["metadata"]["name"]
          recordTags["PVStatus"] = item["status"]["phase"]
          recordTags["PVAccessModes"] = item["spec"]["accessModes"].join(', ')
          recordTags["PVStorageClassName"] = item["spec"]["storageClassName"]
          recordTags["PVCapacityBytes"] = KubernetesApiClient.getMetricNumericValue("memory", item["spec"]["capacity"]["storage"])
          recordTags["PVCreationTimeStamp"] = item["metadata"]["creationTimestamp"]

          # Optional values
          if hasPVC
            recordTags["PVCName"] = pvcName
            recordTags["PVCNamespace"] = pvcNamespace
          end
          if hasType
            recordTags["PVType"] = type
          end
          typeInfo = {}
          if isAzureDisk
            typeInfo["DiskName"] = diskName
            typeInfo["DiskURI"] = diskUri
          elsif isAzureFile
            typeInfo["FileShareName"] = azureFileShareName
          end
          recordTags["PVTypeInfo"] = typeInfo

          records.push(record)
        end

        $log.info "pvTypeToCountHash: #{@pvTypeToCountHash}"

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
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
        @pvKindToCountHash = {}
        currentTime = Time.now
        batchTime = currentTime.utc.iso8601

        continuationToken = nil
        $log.info("in_kube_pvinventory::enumerate : Getting PVs from Kube API @ #{Time.now.utc.iso8601}")
        continuationToken, pvInventory = KubernetesApiClient.getResourcesAndContinuationToken("persistentvolumes?limit=#{@PV_CHUNK_SIZE}")
        $log.info("in_kube_pvinventory::enumerate : Done getting PVs from Kube API @ #{Time.now.utc.iso8601}")

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
          telemetryProperties["CountsOfPVKinds"] = @pvKindToCountHash
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

          # Node and Pod info can be found by joining with pvUsedBytes metric using namespace/PVCName
          record = {}
          record["CollectionTime"] = batchTime
          record["ClusterId"] = KubernetesApiClient.getClusterId
          record["ClusterName"] = KubernetesApiClient.getClusterName
          # Name or PVName
          record["Name"] = item["metadata"]["name"]
          record["PVCName"] = pvcName
          # Namespace, PodNamespace, or PVNamespace
          record["Namespace"] = namespace
          record["CreationTimeStamp"] = item["metadata"]["creationTimestamp"]
          # Should kubernetes.io/ be removed?
          record["Kind"] = kind
          # This is the storage class name rather than type. Would require another api call to get more storage class info
          record["StorageClassName"] = item["spec"]["storageClassName"]
          # Available, Bound, Released, Failed
          record["Status"] = item["status"]["phase"]
          # RWO for azure disks; azure files can have multiple in the spec: RWO, ROX, and/or RWX
          record["AccessModes"] = item["spec"]["accessModes"]
          # This is a string
          record["RequestSize"] = item["spec"]["capacity"]["storage"]
          # Should these be their own columns or tags for PV Kind
          kindTags = {}
          if isAzureDisk
            kindTags["DiskName"] = diskName
            kindTags["DiskURI"] = diskUri
          end
          record["KindInfo"] = kindTags

          records.push(record)
        end

        $log.info "pvKindToCountHash: #{@pvKindToCountHash}"

        records.each do |record|
          if !record.nil?
            wrapper = {
              "DataType" => "KUBE_PV_INVENTORY_BLOB",
              "IPName" => "ContainerInsights",
              "DataItems" => [record.each { |k, v| record[k] = v }],
            }
            eventStream.add(emitTime, wrapper) if wrapper
          end
        end

        router.emit_stream(@tag, eventStream) if eventStream

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
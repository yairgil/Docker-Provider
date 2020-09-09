module Fluent
  class Kube_PVInventory_Input < Input
    Plugin.register_input("kubepvinventory", self)

    @@MDMKubePVInventoryTag = "mdm.kubepvinventory"

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

      @PVC_CHUNK_SIZE = "1500"
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

    def enumerate
      begin
        pvInventory = nil
        currentTime = Time.now
        batchTime = currentTime.utc.iso8601

        continuationToken = nil
        $log.info("in_kube_pvinventory::enumerate : Getting PVs from Kube API @ #{Time.now.utc.iso8601}")
        continuationToken, pvInventory = KubernetesApiClient.getResourcesAndContinuationToken("persistentvolumes?limit=#{@PVC_CHUNK_SIZE}")
        $log.info("in_kube_pvinventory::enumerate : Done getting PVs from Kube API @ #{Time.now.utc.iso8601}")

        if (!pvInventory.nil? && !pvInventory.empty? && pvInventory.key?("items") && !pvInventory["items"].nil? && !pvInventory["items"].empty?)
          parse_and_emit_records(pvInventory, batchTime)
        else
          $log.warn "in_kube_pvinventory::enumerate:Received empty pvInventory"
        end

        #If we receive a continuation token, make calls, process and flush data until we have processed all data
        while (!continuationToken.nil? && !continuationToken.empty?)
          continuationToken, pvInventory = KubernetesApiClient.getResourcesAndContinuationToken("persistentvolumes?limit=#{@PVC_CHUNK_SIZE}&continue=#{continuationToken}")
          if (!pvInventory.nil? && !pvInventory.empty? && pvInventory.key?("items") && !pvInventory["items"].nil? && !pvInventory["items"].empty?)
            parse_and_emit_records(pvInventory, batchTime)
          else
            $log.warn "in_kube_pvinventory::enumerate:Received empty pvInventory"
          end
        end

        # Setting this to nil so that we dont hold memory until GC kicks in
        pvInventory = nil
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

          pvHasPVC = false
          if !item["spec"].nil? && !item["spec"]["claimRef"].nil?
            item["spec"]["claimRef"].each do |claimRef|
              if claimRef["kind"] == "PersistentVolumeClaim"
                namespace = claimRef["namespace"]
                pvcName = claimRef["name"]
                pvHasPVC = true
              end
            end
          end

          if !pvHasPVC
            return records
          end

          metricItem = {}
          metricItem["CollectionTime"] = batchTime
          metricItem["Computer"] = ""
          metricItem["Name"] = "pvInventory"
          metricItem["Value"] = 0
          metricItem["Origin"] = Constants::INSIGHTSMETRICS_TAGS_ORIGIN
          metricItem["Namespace"] = "container.azm.ms/pv"

          metricTags = {}
          metricTags[Constants::INSIGHTSMETRICS_TAGS_CLUSTERID] = KubernetesApiClient.getClusterId
          metricTags[Constants::INSIGHTSMETRICS_TAGS_CLUSTERNAME] = KubernetesApiClient.getClusterName
          metricTags["PVName"] = item["metadata"]["name"]
          metricTags["PVCName"] = pvcName
          metricTags["Namespace"] = namespace
          metricTags["CreationTimeStamp"] = item["metadata"]["creationTimestamp"]
          metricTags["Kind"] = item["metadata"]["annotations"]["pv.kubernetes.io/provisioned-by"]
          metricTags["StorageClassName"] = item["spec"]["storageClassName"]
          metricTags["Status"] = item["status"]["phase"]
          metricTags["AccessMode"] = item["spec"]["accessModes"][0]
          metricTags["RequestSize"] = item["spec"]["capacity"]["storage"]

          metricItem["Tags"] = metricTags
          records.push(metricItem)

          #record = {}
          #record["CollectionTime"] = batchTime
          #record["Name"] = item["metadata"]["name"]
          #record["Namespace"] = item["metadata"]["namespace"]
          #record["CreationTimeStamp"] = item["metadata"]["creationTimestamp"]
          #record["Kind"] = item["metadata"]["annotations"]["volume.beta.kubernetes.io/storage-provisioner"]
          #record["VolumeName"] = item["spec"]["volumeName"]
          #record["StorageClassName"] = item["spec"]["storageClassName"]
          #record["Status"] = item["status"]["phase"]
          #record["AccessMode"] = item["status"]["accessModes"][0]
          #record["RequestSize"] = item["status"]["capacity"]["storage"]

          #record["PodUid"] = ""
          #record["DiskId"] = ""
          #record["ClusterName"] = KubernetesApiClient.getClusterName
          #record["ClusterId"] = KubernetesApiClient.getClusterId

          #records.push(record.dup)
        end

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

        router.emit_stream(@tag, eventStream) if eventStream
        router.emit_stream(Constants::INSIGHTSMETRICS_FLUENT_TAG, eventStream) if eventStream

      rescue => errorStr
        $log.warn "Failed in parse_and_emit_record pv inventory: #{errorStr}"
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
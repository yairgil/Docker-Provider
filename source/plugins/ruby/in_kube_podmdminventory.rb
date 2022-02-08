#!/usr/local/bin/ruby
# frozen_string_literal: true

require "fluent/plugin/input"

module Fluent::Plugin
  require_relative "podinventory_to_mdm"

  class Kube_PodMDMInventory_Input < Input
    Fluent::Plugin.register_input("kube_podmdminventory", self)

    @@MDMKubePodInventoryTag = "mdm.kubepodinventory"

    def initialize
      super
      require "yaml"
      require "yajl/json_gem"
      require "yajl"
      require "set"
      require "time"
      require "net/http"
      require "fileutils"
      require_relative "ApplicationInsightsUtility"
      require_relative "oms_common"
      require_relative "omslog"
      require_relative "constants"
    end

    config_param :run_interval, :time, :default => 60

    def configure(conf)
      super
      @inventoryToMdmConvertor = Inventory2MdmConvertor.new()
    end

    def start
      if @run_interval
        super
        $log.info("in_kube_podmdminventory::start @ #{Time.now.utc.iso8601}")
        @finished = false
        @condition = ConditionVariable.new
        @mutex = Mutex.new
        @thread = Thread.new(&method(:run_periodic))
      end
    end

    def shutdown
      if @run_interval
        @mutex.synchronize {
          @finished = true
          @condition.signal
        }
        @thread.join
        super # This super must be at the end of shutdown method
      end
    end

    def enumerate
      begin
        currentTime = Time.now
        batchTime = currentTime.utc.iso8601
        parse_and_emit_records(batchTime)
      rescue => errorStr
        $log.warn "in_kube_podmdminventory::enumerate:Failed in enumerate: #{errorStr}"
        $log.debug_backtrace(errorStr.backtrace)
        ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
      end
    end

    def parse_and_emit_records(batchTime = Time.utc.iso8601)
      begin
        $log.info "in_kube_podmdminventory:parse_and_emit_records:Start:readMDMRecords @ #{Time.now.utc.iso8601}"
        mdmPodRecords = readMDMRecords()
        $log.info "in_kube_podmdminventory:parse_and_emit_records:End:readMDMRecords @ #{Time.now.utc.iso8601}"
        if !mdmPodRecords.nil? && !mdmPodRecords.empty? && mdmRecords.length > 0
          mdmPodRecords.each do |record|
            @inventoryToMdmConvertor.process_pod_inventory_record(record)
            @inventoryToMdmConvertor.process_record_for_pods_ready_metric(record["ControllerName"], record["Namespace"], record["PodReadyCondition"])
            containerRecords = record["containerRecords"]
            if !containerRecords.nil? && !containerRecords.empty? && containerRecords.length > 0
              containerRecords.each do |containerRecord|
                if !containerRecord["state"].nil? && !containerRecord["state"].empty?
                  @inventoryToMdmConvertor.process_record_for_terminated_job_metric(record["ControllerName"], record["Namespace"], containerRecord["state"])
                end
                begin
                  if !containerRecord["lastState"].nil? && containerRecord["lastState"].keys.length == 1
                    lastStateName = containerRecord["lastState"].keys[0]
                    lastStateObject = containerRecord["lastState"][lastStateName]
                    if !lastStateObject.is_a?(Hash)
                      raise "expected a hash object. This could signify a bug or a kubernetes API change"
                    end
                    if lastStateObject.key?("reason") && lastStateObject.key?("startedAt") && lastStateObject.key?("finishedAt")
                      lastStateReason = lastStateObject["reason"]
                      lastFinishedTime = lastStateObject["finishedAt"]
                      #Populate mdm metric for OOMKilled container count if lastStateReason is OOMKilled
                      if lastStateReason.downcase == Constants::REASON_OOM_KILLED
                        @inventoryToMdmConvertor.process_record_for_oom_killed_metric(record["ControllerName"], record["Namespace"], lastFinishedTime)
                      end
                      lastStateReason = nil
                    end
                  end
                  containerRestartCount = containerRecord["restartCount"]
                  #Populate mdm metric for container restart count if greater than 0
                  if (!containerRestartCount.nil? && (containerRestartCount.is_a? Integer) && containerRestartCount > 0)
                    @inventoryToMdmConvertor.process_record_for_container_restarts_metric(record["ControllerName"], record["Namespace"], lastFinishedTime)
                  end
                rescue => err
                  $log.warn "in_kube_podmdminventory:parse_and_emit_records: failed while processing ContainerLastStatus: #{err}"
                  $log.debug_backtrace(err.backtrace)
                  ApplicationInsightsUtility.sendExceptionTelemetry(err)
                end
              end
            end
          end
          @log.info "in_kube_podmdminventory:parse_and_emit_records:Sending pod inventory mdm records to out_mdm"
          pod_inventory_mdm_records = @inventoryToMdmConvertor.get_pod_inventory_mdm_records(batchTime)
          @log.info "in_kube_podmdminventory:parse_and_emit_records:pod_inventory_mdm_records.size #{pod_inventory_mdm_records.size}"
          mdm_pod_inventory_es = Fluent::MultiEventStream.new
          pod_inventory_mdm_records.each { |pod_inventory_mdm_record|
            mdm_pod_inventory_es.add(batchTime, pod_inventory_mdm_record) if pod_inventory_mdm_record
            if mdm_pod_inventory_es.count >= 5000 # 5k records of MDM is ~2MB and each record is ~400 bytes
              router.emit_stream(@@MDMKubePodInventoryTag, mdm_pod_inventory_es)
              mdm_pod_inventory_es = Fluent::MultiEventStream.new
            end
          } if pod_inventory_mdm_records
          if mdm_pod_inventory_es.count > 0
            router.emit_stream(@@MDMKubePodInventoryTag, mdm_pod_inventory_es)
          end
          mdm_pod_inventory_es = nil
        end
      rescue => errorStr
        $log.warn "in_kube_podmdminventory:parse_and_emit_records: failed with an error #{errorStr}"
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
            $log.info("in_kube_podmdminventory::run_periodic.enumerate.start #{Time.now.utc.iso8601}")
            enumerate
            $log.info("in_kube_podmdminventory::run_periodic.enumerate.end #{Time.now.utc.iso8601}")
          rescue => errorStr
            $log.warn "in_kube_podmdminventory::run_periodic: enumerate Failed to retrieve pod inventory: #{errorStr}"
            ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
          end
        end
        @mutex.lock
      end
      @mutex.unlock
    end

    def readMDMRecords()
      maxRetryCount = 3
      initialRetryDelaySecs = 0.5
      retryAttemptCount = 1
      mdmRecords = []
      begin
        f = File.open(Constants::MDM_POD_INVENTORY_STATE_FILE, "r")
        if !f.nil?
          isAcquiredLock = f.flock(File::LOCK_EX | File::LOCK_NB)
          raise "in_kube_podmdminventory:readMDMRecords:Failed to acquire file lock" if !isAcquiredLock
          startTime = (Time.now.to_f * 1000).to_i
          mdmRecords = Yajl::Parser.parse(f)
          timetakenMs = ((Time.now.to_f * 1000).to_i - startTime)
          $log.info "in_kube_podmdminventory:readMDMRecords:Number of MDM records: #{mdmRecords.length} with time taken(ms) for read: #{timetakenMs} @  #{Time.now.utc.iso8601}"
        else
          raise "in_kube_podmdminventory:readMDMRecords:Failed to open file for read"
        end
      rescue => err
        if retryAttemptCount < maxRetryCount
          f.flock(File::LOCK_UN) if !f.nil?
          f.close if !f.nil?
          retryAttemptCount = retryAttemptCount + 1
          sleep (initialRetryDelaySecs * retryAttemptCount)
          retry
        end
        $log.warn "in_kube_podmdminventory:readMDMRecords failed with an error: #{err} after retries: #{maxRetryCount} @  #{Time.now.utc.iso8601}"
      ensure
        f.flock(File::LOCK_UN) if !f.nil?
        f.close if !f.nil?
      end
      return mdmRecords
    end
  end # Kube_Pod_Input
end # module

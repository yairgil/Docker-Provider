#!/usr/local/bin/ruby
# frozen_string_literal: true
require 'debug/open_nonstop'
require 'sigdump/setup'
require "fluent/plugin/input"

module Fluent::Plugin
  class Win_CAdvisor_Perf_Input < Input
    Fluent::Plugin.register_input("win_cadvisor_perf", self)

    @@winNodes = []

    def initialize
      super
      require "yaml"
      require "yajl/json_gem"
      require "time"

      require_relative "CAdvisorMetricsAPIClient"
      require_relative "KubernetesApiClient"
      require_relative "oms_common"
      require_relative "omslog"
      require_relative "constants"
      @insightsMetricsTag = "oneagent.containerInsights.INSIGHTS_METRICS_BLOB"
    end

    config_param :run_interval, :time, :default => 60
    config_param :tag, :string, :default => "oneagent.containerInsights.LINUX_PERF_BLOB"
    config_param :mdmtag, :string, :default => "mdm.cadvisorperf"

    def configure(conf)
      super
    end

    def start
      if @run_interval
        @finished = false
        @condition = ConditionVariable.new
        @mutex = Mutex.new
        @thread = Thread.new(&method(:run_periodic))
        @@winNodeQueryTimeTracker = DateTime.now.to_time.to_i
        @@cleanupRoutineTimeTracker = DateTime.now.to_time.to_i
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

    def enumerate()
      time = Fluent::Engine.now
      begin
        timeDifference = (DateTime.now.to_time.to_i - @@winNodeQueryTimeTracker).abs
        timeDifferenceInMinutes = timeDifference / 60
        @@istestvar = ENV["ISTEST"]

        #Resetting this cache so that it is populated with the current set of containers with every call
        $log.info("in_win_cadvisor_perf::enumerate.resetWinContainerIdCache.start @ #{Time.now.utc.round(10).iso8601(6)}")
        CAdvisorMetricsAPIClient.resetWinContainerIdCache()
        $log.info("in_win_cadvisor_perf::enumerate.resetWinContainerIdCache.end @ #{Time.now.utc.round(10).iso8601(6)}")
        if (timeDifferenceInMinutes >= 5)
          $log.info "in_win_cadvisor_perf: Getting windows nodes @ #{Time.now.utc.round(10).iso8601(6)}"
          nodes = KubernetesApiClient.getWindowsNodes()
          if !nodes.nil?
            @@winNodes = nodes
          end
          $log.info "in_win_cadvisor_perf : Successuly got windows nodes after 5 minute interval @ #{Time.now.utc.round(10).iso8601(6)}"
          @@winNodeQueryTimeTracker = DateTime.now.to_time.to_i
        end
        @@winNodes.each do |winNode|
          eventStream = Fluent::MultiEventStream.new
          $log.info("in_win_cadvisor_perf::enumerate.getMetrics.start @ #{Time.now.utc.round(10).iso8601(6)}")
          metricData = CAdvisorMetricsAPIClient.getMetrics(winNode: winNode, metricTime: Time.now.utc.iso8601)
          $log.info("in_win_cadvisor_perf::enumerate.getMetrics.end @ #{Time.now.utc.round(10).iso8601(6)}")
          metricData.each do |record|
            if !record.empty?
              eventStream.add(time, record) if record
            end
          end
          $log.info("in_win_cadvisor_perf::enumerate.metricsemit_stream.start @ #{Time.now.utc.round(10).iso8601(6)}")
          router.emit_stream(@tag, eventStream) if eventStream
          $log.info("in_win_cadvisor_perf::enumerate.metricsemit_stream.end @ #{Time.now.utc.round(10).iso8601(6)}")

          if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0 && eventStream.count > 0)
            $log.info("winCAdvisorPerfEmitStreamSuccess @ #{Time.now.utc.iso8601}")
          end

          #start GPU InsightsMetrics items
          begin
            containerGPUusageInsightsMetricsDataItems = []
            $log.info("in_win_cadvisor_perf::enumerate.getInsightsMetrics.start @ #{Time.now.utc.round(10).iso8601(6)}")
            containerGPUusageInsightsMetricsDataItems.concat(CAdvisorMetricsAPIClient.getInsightsMetrics(winNode: winNode, metricTime: Time.now.utc.iso8601))
            $log.info("in_win_cadvisor_perf::enumerate.getInsightsMetrics.end @ #{Time.now.utc.round(10).iso8601(6)}")

            insightsMetricsEventStream = Fluent::MultiEventStream.new

            containerGPUusageInsightsMetricsDataItems.each do |insightsMetricsRecord|
              insightsMetricsEventStream.add(time, insightsMetricsRecord) if insightsMetricsRecord
            end

            $log.info("in_win_cadvisor_perf::enumerate.insightsmetricsemit_stream.start @ #{Time.now.utc.round(10).iso8601(6)}")
            router.emit_stream(@insightsMetricsTag, insightsMetricsEventStream) if insightsMetricsEventStream
            router.emit_stream(@mdmtag, insightsMetricsEventStream) if insightsMetricsEventStream
            $log.info("in_win_cadvisor_perf::enumerate.insightsmetricsemit_stream.end @ #{Time.now.utc.round(10).iso8601(6)}")

            if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0 && insightsMetricsEventStream.count > 0)
              $log.info("winCAdvisorInsightsMetricsEmitStreamSuccess @ #{Time.now.utc.iso8601}")
            end
          rescue => errorStr
            $log.warn "Failed when processing GPU Usage metrics in_win_cadvisor_perf : #{errorStr}"
            $log.debug_backtrace(errorStr.backtrace)
            ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
          end
          #end GPU InsightsMetrics items

        end

        # Cleanup routine to clear deleted containers from cache
        cleanupTimeDifference = (DateTime.now.to_time.to_i - @@cleanupRoutineTimeTracker).abs
        cleanupTimeDifferenceInMinutes = cleanupTimeDifference / 60
        if (cleanupTimeDifferenceInMinutes >= 5)
          $log.info "in_win_cadvisor_perf : Cleanup routine kicking in to clear deleted containers from cache:start @ #{Time.now.utc.round(10).iso8601(6)}"
          CAdvisorMetricsAPIClient.clearDeletedWinContainersFromCache()
          @@cleanupRoutineTimeTracker = DateTime.now.to_time.to_i
          $log.info "in_win_cadvisor_perf : Cleanup routine kicking in to clear deleted containers from cache:end @ #{Time.now.utc.round(10).iso8601(6)}"
        end
      rescue => errorStr
        $log.warn "Failed to retrieve cadvisor metric data for windows nodes: #{errorStr}"
        $log.debug_backtrace(errorStr.backtrace)
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
            $log.info("in_win_cadvisor_perf::run_periodic.enumerate.start @ #{Time.now.utc.iso8601}")
            enumerate
            $log.info("in_win_cadvisor_perf::run_periodic.enumerate.end @ #{Time.now.utc.iso8601}")
          rescue => errorStr
            $log.warn "in_win_cadvisor_perf::run_periodic: enumerate Failed to retrieve cadvisor perf metrics for windows nodes: #{errorStr}"
          end
        end
        @mutex.lock
      end
      @mutex.unlock
    end
  end # Win_CAdvisor_Perf_Input
end # module

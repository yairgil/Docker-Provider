#!/usr/local/bin/ruby
# frozen_string_literal: true
require 'debug/open_nonstop'
require 'sigdump/setup'
require "fluent/plugin/input"

module Fluent::Plugin
  class CAdvisor_Perf_Input < Input
    Fluent::Plugin.register_input("cadvisor_perf", self)
    @@isWindows = false
    @@os_type = ENV["OS_TYPE"]
    if !@@os_type.nil? && !@@os_type.empty? && @@os_type.strip.casecmp("windows") == 0
      @@isWindows = true
    end

    def initialize
      super
      require "yaml"
      require "yajl/json_gem"
      require "time"

      require_relative "CAdvisorMetricsAPIClient"
      require_relative "oms_common"
      require_relative "omslog"
      require_relative "constants"
    end

    config_param :run_interval, :time, :default => 60
    config_param :tag, :string, :default => "oneagent.containerInsights.LINUX_PERF_BLOB"
    config_param :mdmtag, :string, :default => "mdm.cadvisorperf"
    config_param :nodehealthtag, :string, :default => "kubehealth.DaemonSet.Node"
    config_param :containerhealthtag, :string, :default => "kubehealth.DaemonSet.Container"
    config_param :insightsmetricstag, :string, :default => "oneagent.containerInsights.INSIGHTS_METRICS_BLOB"

    def configure(conf)
      super
    end

    def start
      if @run_interval
        super
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

    def enumerate()
      currentTime = Time.now
      time = Fluent::Engine.now
      batchTime = currentTime.utc.iso8601
      @@istestvar = ENV["ISTEST"]
      begin
        eventStream = Fluent::MultiEventStream.new
        insightsMetricsEventStream = Fluent::MultiEventStream.new
        $log.info("in_cadvisor_perf::enumerate.getMetrics.start @ #{Time.now.utc.round(10).iso8601(6)}")
        metricData = CAdvisorMetricsAPIClient.getMetrics(winNode: nil, metricTime: batchTime )
        $log.info("in_cadvisor_perf::enumerate.getMetrics.end @ #{Time.now.utc.round(10).iso8601(6)}")
        metricData.each do |record|
          eventStream.add(time, record) if record
        end

        $log.info("in_cadvisor_perf::enumerate.metricsemit_stream.start @ #{Time.now.utc.round(10).iso8601(6)}")

        $log.info("in_cadvisor_perf::enumerate.metricsemit_stream-LINUX_PERF_BLOB.start @ #{Time.now.utc.round(10).iso8601(6)}")
        router.emit_stream(@tag, eventStream) if eventStream
        $log.info("in_cadvisor_perf::enumerate.metricsemit_stream-LINUX_PERF_BLOB.end @ #{Time.now.utc.round(10).iso8601(6)}")

        $log.info("in_cadvisor_perf::enumerate.metricsemit_stream-mdm.cadvisorperf.start @ #{Time.now.utc.round(10).iso8601(6)}")
        router.emit_stream(@mdmtag, eventStream) if eventStream
        $log.info("in_cadvisor_perf::enumerate.metricsemit_stream-mdm.cadvisorperf.end @ #{Time.now.utc.round(10).iso8601(6)}")

        $log.info("in_cadvisor_perf::enumerate.metricsemit_stream-kubehealth.DaemonSet.Container.start @ #{Time.now.utc.round(10).iso8601(6)}")
        router.emit_stream(@containerhealthtag, eventStream) if eventStream
        $log.info("in_cadvisor_perf::enumerate.metricsemit_stream-kubehealth.DaemonSet.Container.end @ #{Time.now.utc.round(10).iso8601(6)}")

        $log.info("in_cadvisor_perf::enumerate.metricsemit_stream-kubehealth.DaemonSet.Node.start @ #{Time.now.utc.round(10).iso8601(6)}")
        router.emit_stream(@nodehealthtag, eventStream) if eventStream
        $log.info("in_cadvisor_perf::enumerate.metricsemit_stream-kubehealth.DaemonSet.Node.end @ #{Time.now.utc.round(10).iso8601(6)}")

        $log.info("in_cadvisor_perf::enumerate.metricsemit_stream.end #{Time.now.utc.round(10).iso8601(6)}")

        if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0 && eventStream.count > 0)
          $log.info("cAdvisorPerfEmitStreamSuccess @ #{Time.now.utc.iso8601}")
        end

        #start GPU InsightsMetrics items
        begin
          if !@@isWindows.nil? && @@isWindows == false
            containerGPUusageInsightsMetricsDataItems = []
            $log.info("in_cadvisor_perf::enumerate.getInsightsMetrics.start @ #{Time.now.utc.round(10).iso8601(6)}")
            containerGPUusageInsightsMetricsDataItems.concat(CAdvisorMetricsAPIClient.getInsightsMetrics(winNode: nil, metricTime: batchTime))
            $log.info("in_cadvisor_perf::enumerate.getInsightsMetrics.end #{Time.now.utc.round(10).iso8601(6)}")

            $log.info("in_cadvisor_perf::enumerate.insightsmetricsemit_stream.start @ #{Time.now.utc.round(10).iso8601(6)}")
            containerGPUusageInsightsMetricsDataItems.each do |insightsMetricsRecord|
              insightsMetricsEventStream.add(time, insightsMetricsRecord) if insightsMetricsRecord
            end
            $log.info("in_cadvisor_perf::enumerate.insightsmetricsemit_stream-INSIGHTS_METRICS_BLOB.start @ #{Time.now.utc.round(10).iso8601(6)}")
            router.emit_stream(@insightsmetricstag, insightsMetricsEventStream) if insightsMetricsEventStream
            $log.info("in_cadvisor_perf::enumerate.insightsmetricsemit_stream-INSIGHTS_METRICS_BLOB.end @ #{Time.now.utc.round(10).iso8601(6)}")

            $log.info("in_cadvisor_perf::enumerate.insightsmetricsemit_stream-mdm.cadvisorperf.start @ #{Time.now.utc.round(10).iso8601(6)}")
            router.emit_stream(@mdmtag, insightsMetricsEventStream) if insightsMetricsEventStream
            $log.info("in_cadvisor_perf::enumerate.insightsmetricsemit_stream-mdm.cadvisorperf.end @ #{Time.now.utc.round(10).iso8601(6)}")

            if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0 && insightsMetricsEventStream.count > 0)
              $log.info("cAdvisorInsightsMetricsEmitStreamSuccess @ #{Time.now.utc.iso8601}")
            end
          end
        rescue => errorStr
          $log.warn "Failed when processing GPU Usage metrics in_cadvisor_perf : #{errorStr}"
          $log.debug_backtrace(errorStr.backtrace)
          ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
        end
        #end GPU InsightsMetrics items

      rescue => errorStr
        $log.warn "Failed to retrieve cadvisor metric data: #{errorStr}"
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
            $log.info("in_cadvisor_perf::run_periodic.enumerate.start @ #{Time.now.utc.iso8601}")
            enumerate
            $log.info("in_cadvisor_perf::run_periodic.enumerate.end @ #{Time.now.utc.iso8601}")
          rescue => errorStr
            $log.warn "in_cadvisor_perf::run_periodic: enumerate Failed to retrieve cadvisor perf metrics: #{errorStr}"
          end
        end
        @mutex.lock
      end
      @mutex.unlock
    end
  end # CAdvisor_Perf_Input
end # module

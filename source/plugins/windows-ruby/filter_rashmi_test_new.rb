# frozen_string_literal: true
# Why do the above though win windows? Check in Perf testing

# require "fluent/input"
require "fluent/config/error"
require "logger"

module Fluent
  class Rashmi_Test < Filter
    # First, register the plugin. NAME is the name of this plugin
    # and identifies the plugin in the configuration file.
    Plugin.register_filter("rashmi_test_new", self)
    config_param :log_path, :string, :default => "C:/etc/fluent/filter_rashmi_test.log"
    # config_param defines a parameter. You can refer a parameter via @port instance variable
    #config_param :run_interval, :time

    def initialize
      super
      #require_relative "omsagenthelper"
    end

    def configure(conf)
      super
      @log = nil
      @log = Logger.new(@log_path, 1, 5000000)
      @log.info { "Starting rashmi_test filter plugin" }
    end

    def start
      super
      #if @run_interval
      #  @finished = false
      #  @condition = ConditionVariable.new
      #  @mutex = Mutex.new
      #  @thread = Thread.new(&method(:run_periodic))
      @log.info "in rashmi_test filter plugin"
      #else
      # enumerate
      #end
    end

    # def enumerate
    #   begin
    #     @log.info "in rashmi_test filter plugin"
    #     #puts "Calling certificate renewal code..."
    #     #maintenance = OMS::OnboardingHelper.new(
    #     # ENV["WSID"],
    #     # ENV["DOMAIN"],
    #     # ENV["CI_AGENT_GUID"]
    #     #)
    #     #ret_code = maintenance.register_certs()
    #     #puts "Return code from register certs : #{ret_code}"
    #   rescue => errorStr
    #     @log.info "in_heartbeat_request::enumerate:Failed in enumerate: #{errorStr}"
    #     # STDOUT telemetry should alredy be going to Traces in AI.
    #     # ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
    #   end
    # end

    def shutdown
      # if @run_interval
      #   @mutex.synchronize {
      #     @finished = true
      #     @condition.signal
      #   }
      #   @thread.join
      # end
      super
    end

    def filter(tag, time, record)
      # if @process_incoming_stream
      #   begin
      #     if !record.nil? && !record["name"].nil? && record["name"].downcase == Constants::TELEGRAF_DISK_METRICS
      #       return MdmMetricsGenerator.getDiskUsageMetricRecords(record)
      #     else
      #       return MdmMetricsGenerator.getMetricRecords(record)
      #     end
      #     return []
      #   rescue Exception => errorStr
      #     @log.info "Error processing telegraf record Exception: #{errorStr}"
      #     ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
      #     return [] #return empty array if we ran into any errors
      #   end
      # end
      @log.info "in rashmi_test_new filter method"
      return []
    end

    def filter_stream(tag, es)
      new_es = MultiEventStream.new
      begin
        es.each { |time, record|
          filtered_records = filter(tag, time, record)
          filtered_records.each { |filtered_record|
            new_es.add(time, filtered_record) if filtered_record
          } if filtered_records
        }
      rescue => e
        @log.info "Error in filter_stream #{e.message}"
      end
      new_es
    end

    # def run_periodic
    #   @mutex.lock
    #   done = @finished
    #   until done
    #     @condition.wait(@mutex, @run_interval)
    #     done = @finished
    #     @mutex.unlock
    #     if !done
    #       enumerate
    #     end
    #     @mutex.lock
    #   end
    #   @mutex.unlock
    # end
  end # class Heartbeat_Request
end # module Fluent

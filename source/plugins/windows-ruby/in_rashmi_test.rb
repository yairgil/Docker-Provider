# frozen_string_literal: true
# Why do the above though win windows? Check in Perf testing

require "fluent/input"
require "fluent/config/error"
require "logger"

module Fluent
  class Rashmi_Test < Input
    # First, register the plugin. NAME is the name of this plugin
    # and identifies the plugin in the configuration file.
    Plugin.register_input("rashmi_test", self)
    config_param :log_path, :string, :default => "C:/etc/fluent/filter_telegraf2mdm.log"
    # config_param defines a parameter. You can refer a parameter via @port instance variable
    config_param :run_interval, :time

    def initialize
      super
      #require_relative "omsagenthelper"
    end

    def configure(conf)
      super
      @log = nil
      @log = Logger.new(@log_path, 1, 5000000)
      @log.info { "Starting rashmi_test plugin" }
    end

    def start
      super
      #if @run_interval
      #  @finished = false
      #  @condition = ConditionVariable.new
      #  @mutex = Mutex.new
      #  @thread = Thread.new(&method(:run_periodic))
      @log.info "in rashmi_test"
      #else
      enumerate
      #end
    end

    def enumerate
      begin
        @log.info "in rashmi_test"
        #puts "Calling certificate renewal code..."
        #maintenance = OMS::OnboardingHelper.new(
        # ENV["WSID"],
        # ENV["DOMAIN"],
        # ENV["CI_AGENT_GUID"]
        #)
        #ret_code = maintenance.register_certs()
        #puts "Return code from register certs : #{ret_code}"
      rescue => errorStr
        @log.info "in_heartbeat_request::enumerate:Failed in enumerate: #{errorStr}"
        # STDOUT telemetry should alredy be going to Traces in AI.
        # ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
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

    def run_periodic
      @mutex.lock
      done = @finished
      until done
        @condition.wait(@mutex, @run_interval)
        done = @finished
        @mutex.unlock
        if !done
          enumerate
        end
        @mutex.lock
      end
      @mutex.unlock
    end
  end # class Heartbeat_Request
end # module Fluent

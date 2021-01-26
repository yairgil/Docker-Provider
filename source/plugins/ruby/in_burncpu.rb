require 'fluent/plugin/input'

module Fluent::Plugin
  class BurnCpu < Input
    # First, register the plugin. 'NAME' is the name of this plugin
    # and identifies the plugin in the configuration file.
    Fluent::Plugin.register_input('burncpu', self)

    # `config_param` defines a parameter.
    # You can refer to a parameter like an instance variable e.g. @port.
    # `:default` means that the parameter is optional.
    config_param :port, :integer, default: 8888

    config_param :run_interval, :time, :default => 1
    config_param :tag, :string, :default => "data.tag"

    # `configure` is called before `start`.
    # 'conf' is a `Hash` that includes the configuration parameters.
    # If the configuration is invalid, raise `Fluent::ConfigError`.
    def configure(conf)
      super
    end


    # `start` is called when starting and after `configure` is successfully completed.
    # Open sockets or files and create threads here.
    def start
      super

      # Startup code goes here!
        @finished = false
        @condition = ConditionVariable.new
        @mutex = Mutex.new
        @thread = Thread.new(&method(:run_periodic))
        @@podTelemetryTimeTracker = DateTime.now.to_time.to_i

        @running = 1
        
        @burnThread1 = Thread.new(&method(:burnCpuThread))
        @burnThread2 = Thre ad.new(&method(:burnCpuThread))
        @burnThread3 = Thread.new(&method(:burnCpuThread))
    end

    # `shutdown` is called while closing down.
    def shutdown
      # Shutdown code goes here!
      @running = 0
      if @run_interval
        @mutex.synchronize {
          @finished = true
          @condition.signal
        }
        @thread.join
      end
      super
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
              $log.info("burncpu::run_periodic.enumerate.start #{Time.now.utc.iso8601}")
              
              #TODO: spin here
              emit_record

              $log.info("burncpu::run_periodic.enumerate.end #{Time.now.utc.iso8601}")
            rescue => errorStr
              $log.warn "burncpu::run_periodic: enumerate Failed to retrieve pod inventory: #{errorStr}"
              ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
            end
          end
          @mutex.lock
        end
        @mutex.unlock
      end

      def burnCpuThread
        x = 1
        while @running == 1
            x = x + 1
            if x > 10
                x = -99999
            end
        end
      end


    def emit_record
        tag = @tag
        time = Fluent::Engine.now
        record = {"message"=>"body"}
        router.emit(tag, time, record)
    end

  end
end

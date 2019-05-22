# frozen_string_literal: true

module HealthModel
  class MonitorSet
    attr_accessor :monitors
    # attr_reader :changed_monitors

    def initialize
      @monitors = {}
    end

    def contains?(monitor_instance_id)
      @monitors.key?(monitor_instance_id)
    end

    def add_or_update(monitor)
        # if @monitors.key?(monitor.monitor_instance_id)
        #     current_monitor = @monitors[monitor.monitor_instance_id]
        #     if current_monitor.state.downcase != monitor.state.downcase
        #         @monitors[monitor.monitor_instance_id] = monitor
        #         @changed_monitors[monitor.monitor_instance_id] = monitor
        #     end
        # else
        #     @monitors[monitor.monitor_instance_id] = monitor
        #     @changed_monitors[monitor.monitor_instance_id] = monitor
        # end
        @monitors[monitor.monitor_instance_id] = monitor
    end

    def get_monitor(monitor_instance_id)
      @monitors[monitor_instance_id] if @monitors.key?(monitor_instance_id)
    end

    def delete(monitor_instance_id)
      if @monitors.key?(monitor_instance_id)
        @monitors.delete(monitor_instance_id)
      end

    #   if @changed_monitors.key(monitor_instance_id)
    #     @changed_monitors.delete(monitor_instance_id)
    #   end
    end

    def get_size
      @monitors.length
    end

    def get_map
        @monitors
    end

    # def clear_changed_monitors
    #     @changed_monitors = {}
    # end
  end
end

require_relative '../../../../source/code/plugin/health/unit_monitor'
require 'minitest/autorun'
require 'time'

class UnitMonitorTest < Minitest::Test
    include HealthModel

    def test_is_aggregate_monitor_false
        monitor = UnitMonitor.new(:monitor_id, :monitor_instance_id, :pass, Time.now.utc.iso8601, {}, {}, {})
        assert_equal monitor.is_aggregate_monitor, false
    end
end

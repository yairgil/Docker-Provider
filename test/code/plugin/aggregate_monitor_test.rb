require 'test/unit'
require_relative '../../../source/code/plugin/aggregate_monitor'

class AggregateMonitor_Test < Test::Unit::TestCase
    def test_get_name
        puts 'starting test_get_name'
        agg = AggregateMonitor.new('Cluster', 'Cluster')
        assert_equal(agg.name, 'Cluster')
    end
end

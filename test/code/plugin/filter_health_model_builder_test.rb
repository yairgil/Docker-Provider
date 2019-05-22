# frozen_string_literal: true

require 'test/unit'
require 'json'
# require_relative '../../../source/code/plugin/health'

Dir[File.join(__dir__, '../../../source/code/plugin/health', '*.rb')].each { |file| require file }

class FilterHealthModelBuilderTest < Test::Unit::TestCase
  include HealthModel

  def test_event_stream
    health_definition_path = 'C:\AzureMonitor\ContainerInsights\Docker-Provider\installer\conf\health_model_definition.json'
    health_model_definition = HealthModelDefinition.new(HealthModelDefinitionParser.new(health_definition_path).parse_file)
    monitor_factory = MonitorFactory.new
    state_transition_processor = StateTransitionProcessor.new(health_model_definition, monitor_factory)
    state_finalizers = [NodeMonitorHierarchyReducer.new, AggregateMonitorStateFinalizer.new]
    monitor_set = MonitorSet.new
    model_builder = HealthModelBuilder.new(state_transition_processor, state_finalizers, monitor_set)

    i = 1
    loop do
        mock_data_path = "C:/AzureMonitor/ContainerInsights/Docker-Provider/source/code/plugin/mock_data-#{i}.json"
        file = File.read(mock_data_path)
        data = JSON.parse(file)

        state_transitions = []
        data.each do |record|
        state_transition = MonitorStateTransition.new(
            record[HealthMonitorRecordFields::MONITOR_ID],
            record[HealthMonitorRecordFields::MONITOR_INSTANCE_ID],
            record[HealthMonitorRecordFields::TIME_FIRST_OBSERVED],
            record[HealthMonitorRecordFields::OLD_STATE],
            record[HealthMonitorRecordFields::NEW_STATE],
            record[HealthMonitorRecordFields::MONITOR_LABELS],
            record[HealthMonitorRecordFields::MONITOR_CONFIG],
            record[HealthMonitorRecordFields::DETAILS]
        )
        state_transitions.push(state_transition)
        end

        model_builder.process_state_transitions(state_transitions)
        changed_monitors = model_builder.finalize_model
        i = i + 1
        if i == 5
            break
        end
    end
    puts "Done"
  end
end

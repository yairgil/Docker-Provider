# frozen_string_literal: true

require 'test/unit'
require 'json'
# require_relative '../../../source/code/plugin/health'

Dir[File.join(__dir__, '../../../source/code/plugin/health', '*.rb')].each { |file| require file }

class FilterHealthModelBuilderTest < Test::Unit::TestCase
  include HealthModel

  def test_event_stream

    health_definition_path = 'C:\AzureMonitor\ContainerInsights\Docker-Provider\installer\conf\health_model_definition.json'
    health_monitor_config_path = 'C:\AzureMonitor\ContainerInsights\Docker-Provider\installer\conf\healthmonitorconfig.json'
    health_model_definition = HealthModel::ParentMonitorProvider.new(HealthModel::HealthModelDefinitionParser.new(health_definition_path).parse_file)
    monitor_factory = HealthModel::MonitorFactory.new
    hierarchy_builder = HealthHierarchyBuilder.new(health_model_definition, monitor_factory)

    state_finalizers = [HealthModel::AggregateMonitorStateFinalizer.new]
    monitor_set = HealthModel::MonitorSet.new
    model_builder = HealthModel::HealthModelBuilder.new(hierarchy_builder, state_finalizers, monitor_set)

    kube_api_down_handler = HealthKubeApiDownHandler.new
    resources = HealthKubernetesResources.instance
    reducer = HealthSignalReducer.new
    generator = HealthMissingSignalGenerator.new


    resources.node_inventory = JSON.parse(File.read('C:\AzureMonitor\ContainerInsights\Docker-Provider\health_model_redesign\nodes.json'))
    resources.pod_inventory = JSON.parse(File.read('C:\AzureMonitor\ContainerInsights\Docker-Provider\health_model_redesign\pods.json'))
    resources.set_replicaset_inventory(JSON.parse(File.read('C:\AzureMonitor\ContainerInsights\Docker-Provider\health_model_redesign\rs.json')))
    resources.build_pod_uid_lookup

    cluster_id = '/subscriptions/72c8e8ca-dc16-47dc-b65c-6b5875eb600a/resourcegroups/dilipr-health-test/providers/Microsoft.ContainerService/managedClusters/dilipr-health-test'
    labels = {}
    labels['container.azm.ms/cluster-region'] = 'eastus'
    labels['container.azm.ms/cluster-subscription-id'] = '72c8e8ca-dc16-47dc-b65c-6b5875eb600a'
    labels['container.azm.ms/cluster-resource-group'] = 'dilipr-health-test'
    labels['container.azm.ms/cluster-name'] = 'dilipr-health-test'

    provider = HealthMonitorProvider.new(cluster_id, labels, resources, health_monitor_config_path)

    state = HealthMonitorState.new

    mock_data_path = 'C:\AzureMonitor\ContainerInsights\Docker-Provider\health_model_redesign\records.json'
    file = File.read(mock_data_path)
    data = JSON.parse(file)
    state_transitions = []

    health_monitor_records = []
    data.each do |record|
        monitor_id = record[HealthMonitorRecordFields::MONITOR_ID]
        health_monitor_record = HealthMonitorRecord.new(
            record[HealthMonitorRecordFields::MONITOR_ID],
            record[HealthMonitorRecordFields::MONITOR_INSTANCE_ID],
            record[HealthMonitorRecordFields::TIME_FIRST_OBSERVED],
            record[HealthMonitorRecordFields::DETAILS]["state"],
            provider.get_labels(record),
            provider.get_config(monitor_id),
            record[HealthMonitorRecordFields::DETAILS]
        )
        state_transitions.push(health_monitor_record)
    end

    model_builder.process_records(state_transitions)
    changed_monitors = model_builder.finalize_model
    sorted = changed_monitors.sort.to_h
    print_hierarchy(changed_monitors)
  end

  def print_hierarchy(monitors)
    root = monitors.select{|k,v| k == 'cluster'}.first[1]
    p root
    print_tree(root, monitors)
  end

  def print_tree(root, monitors)


  end
end

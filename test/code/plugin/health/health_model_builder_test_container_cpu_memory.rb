require 'test/unit'
require 'json'
# require_relative '../../../source/code/plugin/health'

Dir[File.join(__dir__, '../../../../source/code/plugin/health', '*.rb')].each { |file| require file }

class FilterHealthModelBuilderTest < Test::Unit::TestCase
    include HealthModel

    def test_container_memory_cpu_with_model
        health_definition_path = File.join(__dir__, '../../../../installer/conf/health_model_definition.json')
        health_model_definition = ParentMonitorProvider.new(HealthModelDefinitionParser.new(health_definition_path).parse_file)
        monitor_factory = MonitorFactory.new
        hierarchy_builder = HealthHierarchyBuilder.new(health_model_definition, monitor_factory)
        # TODO: Figure out if we need to add NodeMonitorHierarchyReducer to the list of finalizers. For now, dont compress/optimize, since it becomes impossible to construct the model on the UX side
        state_finalizers = [AggregateMonitorStateFinalizer.new]
        monitor_set = MonitorSet.new
        model_builder = HealthModelBuilder.new(hierarchy_builder, state_finalizers, monitor_set)

        cluster_labels = {
            'container.azm.ms/cluster-region' => 'eastus',
            'container.azm.ms/cluster-subscription-id' => '72c8e8ca-dc16-47dc-b65c-6b5875eb600a',
            'container.azm.ms/cluster-resource-group' => 'dilipr-health-test',
            'container.azm.ms/cluster-name' => 'dilipr-health-test'
        }

        cluster_id = 'fake_cluster_id'

        #test
        state = HealthMonitorState.new()
        generator = HealthMissingSignalGenerator.new

        mock_data_path = "C:/Users/dilipr/desktop/health/container_cpu_memory/daemonset.json"
        file = File.read(mock_data_path)
        records = JSON.parse(file)

        node_inventory = JSON.parse(File.read("C:/Users/dilipr/desktop/health/container_cpu_memory/nodes.json"))
        pod_inventory = JSON.parse(File.read("C:/Users/dilipr/desktop/health/container_cpu_memory/pods.json"))
        deployment_inventory = JSON.parse(File.read("C:/Users/dilipr/desktop/health/container_cpu_memory/deployments.json"))
        resources = HealthKubernetesResources.instance
        resources.node_inventory = node_inventory
        resources.pod_inventory = pod_inventory
        resources.set_deployment_inventory(deployment_inventory)

        workload_names = resources.get_workload_names
        provider = HealthMonitorProvider.new(cluster_id, cluster_labels, resources, File.join(__dir__, "../../../../installer/conf/healthmonitorconfig.json"))


        #container memory cpu records
        file = File.read('C:/Users/dilipr/desktop/health/container_cpu_memory/cadvisor_perf.json')
        cadvisor_records = JSON.parse(file)

        #begin filter_cadvisor_health_node
        cadvisor_records = cadvisor_records.select{|record| record['DataItems'][0]['ObjectName'] == 'K8SContainer'}
        formatted_records = []
        formatter = HealthContainerCpuMemoryRecordFormatter.new
        cadvisor_records.each{|record|
            formatted_record = formatter.get_record_from_cadvisor_record(record)
            formatted_records.push(formatted_record)
        }
        #end filter_cadvisor_health_node

        #begin in_kube_health
        resources.build_pod_uid_lookup #call this in in_kube_health every min
        #end in_kube_health

        #begin filter_health_model_builder

        #begin processing container health records
        aggregator = HealthContainerCpuMemoryAggregator.new(resources, provider)
        deduped_records = aggregator.dedupe_records(formatted_records)
        aggregator.aggregate(deduped_records)
        aggregator.compute_state
        container_cpu_memory_records = aggregator.get_records
        records.concat(container_cpu_memory_records)
        #end processing container health records

        health_monitor_records = []
        records.each do |record|
            monitor_instance_id = record[HealthMonitorRecordFields::MONITOR_INSTANCE_ID]
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

            state.update_state(health_monitor_record,
                provider.get_config(health_monitor_record.monitor_id)
                )

            # get the health state based on the monitor's operational state
            # update state calls updates the state of the monitor based on configuration and history of the the monitor records
            health_monitor_record.state = state.get_state(monitor_instance_id).new_state
            health_monitor_records.push(health_monitor_record)
            #puts "#{monitor_instance_id} #{instance_state.new_state} #{instance_state.old_state} #{instance_state.should_send}"
        end

        #handle kube api down
        kube_api_down_handler = HealthKubeApiDownHandler.new
        health_monitor_records = kube_api_down_handler.handle_kube_api_down(health_monitor_records)

        # Dedupe daemonset signals
        # Remove unit monitor signals for “gone” objects
        reducer = HealthSignalReducer.new()
        reduced_records = reducer.reduce_signals(health_monitor_records, resources)

        cluster_id = 'fake_cluster_id'

        #get the list of  'none' and 'unknown' signals
        missing_signals = generator.get_missing_signals(cluster_id, reduced_records, resources, provider)
        #update state for missing signals
        missing_signals.each{|signal|
            state.update_state(signal,
                provider.get_config(signal.monitor_id)
                )
        }
        generator.update_last_received_records(reduced_records)
        reduced_records.push(*missing_signals)

        # build the health model
        all_records = reduced_records
        model_builder.process_records(all_records)
        all_monitors = model_builder.finalize_model

        # update the state for aggregate monitors (unit monitors are updated above)
        all_monitors.each{|monitor_instance_id, monitor|
            if monitor.is_aggregate_monitor
                state.update_state(monitor,
                    provider.get_config(monitor.monitor_id)
                    )
            end

            instance_state = state.get_state(monitor_instance_id)
            #puts "#{monitor_instance_id} #{instance_state.new_state} #{instance_state.old_state} #{instance_state.should_send}"
            should_send = instance_state.should_send

            # always send cluster monitor as a heartbeat
            if !should_send && monitor_instance_id != MonitorId::CLUSTER
                all_monitors.delete(monitor_instance_id)
            end
        }

        records_to_send = []
        all_monitors.keys.each{|key|
            record = provider.get_record(all_monitors[key], state)
            #puts "#{record["MonitorInstanceId"]} #{record["OldState"]} #{record["NewState"]}"
        }
    end
end
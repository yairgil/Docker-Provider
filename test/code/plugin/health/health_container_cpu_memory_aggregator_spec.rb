require_relative '../test_helpers'
Dir[File.join(File.expand_path(File.dirname(__FILE__)), "../../../../source/code/plugin/health/*.rb")].reject{|f| f.include?('health_monitor_utils')}.each { |file| require file }
include HealthModel

describe 'HealthContainerCpuMemoryAggregator spec' do
    it 'aggregates based on container name' do
        file = File.read('C:/Users/dilipr/desktop/health/container_cpu_memory/cadvisor_perf.json')
        records = JSON.parse(file)
        records = records.select{|record| record['DataItems'][0]['ObjectName'] == 'K8SContainer'}
        formatted_records = []
        formatter = HealthContainerCpuMemoryRecordFormatter.new
        records.each{|record|
            formatted_record = formatter.get_record_from_cadvisor_record(record)
            formatted_records.push(formatted_record)
        }

        resources = HealthKubernetesResources.instance
        nodes = JSON.parse(File.read('C:/Users/dilipr/desktop/health/container_cpu_memory/nodes.json'))
        pods = JSON.parse(File.read('C:/Users/dilipr/desktop/health/container_cpu_memory/pods.json'))
        deployments = JSON.parse(File.read('C:/Users/dilipr/desktop/health/container_cpu_memory/deployments.json'))

        resources.pod_inventory = pods
        resources.node_inventory = nodes
        resources.set_deployment_inventory(deployments)
        resources.build_pod_uid_lookup #call this in in_kube_health every min

        cluster_labels = {
            'container.azm.ms/cluster-region' => 'eastus',
            'container.azm.ms/cluster-subscription-id' => '72c8e8ca-dc16-47dc-b65c-6b5875eb600a',
            'container.azm.ms/cluster-resource-group' => 'dilipr-health-test',
            'container.azm.ms/cluster-name' => 'dilipr-health-test'
        }

        cluster_id = 'fake_cluster_id'

        provider = HealthMonitorProvider.new(cluster_id, cluster_labels, resources, File.join(__dir__, "../../../../installer/conf/healthmonitorconfig.json"))

        aggregator = HealthContainerCpuMemoryAggregator.new(resources, provider)

        deduped_records = aggregator.dedupe_records(formatted_records)
        aggregator.aggregate(deduped_records)
        aggregator.compute_state
        records = aggregator.get_records
        puts "done"
    end
end
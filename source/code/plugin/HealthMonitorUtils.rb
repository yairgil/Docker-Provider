#!/usr/local/bin/ruby
# frozen_string_literal: true

require_relative 'KubernetesApiClient'
require_relative 'HealthMonitorConstants'
require 'time'
require 'json'

class HealthMonitorUtils

    @LogPath = "/var/opt/microsoft/docker-cimprov/log/health_monitors.log"
    @log = Logger.new(@LogPath, 2, 10 * 1048576) #keep last 2 files, max log file size = 10M
    @@hostName = (OMS::Common.get_hostname)

    @@containerMetadata = {}
    @@controllerMapping = {}
    @@podInventory = {}
    @@lastRefreshTime = '2019-01-01T00:00:00Z'
    @@nodeInventory = []
    @@clusterId = KubernetesApiClient.getClusterId

    def initialize
    end

    class << self

        def build_metrics_hash(metrics_to_collect)
            @log.debug "Building Hash of Metrics to Collect #{metrics_to_collect}"
            metrics_to_collect_arr = metrics_to_collect.split(',').map(&:strip)
            metrics_hash = metrics_to_collect_arr.map {|x| [x.downcase,true]}.to_h
            @log.info "Metrics Collected : #{metrics_hash}"
            return metrics_hash
        end

        def ensure_cpu_memory_capacity_set(cpu_capacity, memory_capacity, hostname)

            @log.info "ensure_cpu_memory_capacity_set cpu_capacity #{cpu_capacity} memory_capacity #{memory_capacity}"
            if cpu_capacity != 0.0 && memory_capacity != 0.0
                @log.info "CPU And Memory Capacity are already set"
                return [cpu_capacity, memory_capacity]
            end

            begin
                @@nodeInventory = JSON.parse(KubernetesApiClient.getKubeResourceInfo("nodes").body)
            rescue Exception => e
                @log.info "Error when getting nodeInventory from kube API. Exception: #{e.class} Message: #{e.message} "
                ApplicationInsightsUtility.sendExceptionTelemetry(e.backtrace)
            end
            if !@@nodeInventory.nil?
                cpu_capacity_json = KubernetesApiClient.parseNodeLimits(@@nodeInventory, "capacity", "cpu", "cpuCapacityNanoCores")
                if !cpu_capacity_json.nil?
                    cpu_capacity_json.each do |cpu_info_node|
                        if !cpu_info_node['DataItems'][0]['Host'].nil? && cpu_info_node['DataItems'][0]['Host'] == @@hostName
                            if !cpu_info_node['DataItems'][0]['Collections'][0]['Value'].nil?
                                cpu_capacity = cpu_info_node['DataItems'][0]['Collections'][0]['Value']
                            end
                        end
                    end
                    @log.info "CPU Limit #{cpu_capacity}"
                else
                    @log.info "Error getting cpu_capacity"
                end
                memory_capacity_json = KubernetesApiClient.parseNodeLimits(@@nodeInventory, "capacity", "memory", "memoryCapacityBytes")
                if !memory_capacity_json.nil?
                    memory_capacity_json.each do |memory_info_node|
                        if !memory_info_node['DataItems'][0]['Host'].nil? && memory_info_node['DataItems'][0]['Host'] == @@hostName
                            if !memory_info_node['DataItems'][0]['Collections'][0]['Value'].nil?
                                memory_capacity = memory_info_node['DataItems'][0]['Collections'][0]['Value']
                            end
                        end
                    end
                    @log.info "memory Limit #{memory_capacity}"
                else
                    @log.info "Error getting memory_capacity"
                end
                return [cpu_capacity, memory_capacity]
            end
        end

        def getContainerKeyFromInstanceName(instance_name)
            if instance_name.nil?
                return ""
            end
            size = instance_name.size
            instance_name_elements = instance_name.split("/")
            key = [instance_name_elements[9], instance_name_elements[10]].join("/")
            return key
        end

        def getMonitorInstanceId(log, monitor_id, args = [])
            #log.debug "getMonitorInstanceId"
            string_to_hash = args.join("/")
            return "#{monitor_id}-#{Digest::MD5.hexdigest(string_to_hash)}"
        end

        def getClusterLabels
            labels = {}
            cluster_id = @@clusterId
            region = KubernetesApiClient.getClusterRegion
            labels['container.azm.ms/cluster-region'] = region
            if !cluster_id.nil?
                cluster_id_elements = cluster_id.split('/')
                azure_sub_id =  cluster_id_elements[2]
                resource_group = cluster_id_elements[4]
                cluster_name = cluster_id_elements[8]
                labels['container.azm.ms/cluster-subscription-id'] = azure_sub_id
                labels['container.azm.ms/cluster-resource-group'] = resource_group
                labels['container.azm.ms/cluster-name'] = cluster_name
            end
            return labels
        end

        def getMonitorLabels(log, monitor_id, key: nil, pod_aggregator: nil, node_name: nil, namespace: nil, pod_aggregator_kind: nil)
            #log.debug "monitor_id #{monitor_id} pod_aggregator #{pod_aggregator} pod_aggregator_kind #{pod_aggregator_kind} namespace #{namespace}"
            monitor_labels = {}
            case monitor_id
            when HealthMonitorConstants::WORKLOAD_CONTAINER_CPU_PERCENTAGE_MONITOR_ID, HealthMonitorConstants::WORKLOAD_CONTAINER_MEMORY_PERCENTAGE_MONITOR_ID, HealthMonitorConstants::WORKLOAD_PODS_READY_PERCENTAGE_MONITOR_ID, HealthMonitorConstants::MANAGEDINFRA_PODS_READY_PERCENTAGE_MONITOR_ID, HealthMonitorConstants::POD_STATUS
                if !key.nil? #container
                    monitor_labels['container.azm.ms/pod-aggregator'] = getContainerControllerName(key)
                    monitor_labels['container.azm.ms/namespace'] = getContainerNamespace(key)
                else
                    monitor_labels['container.azm.ms/pod-aggregator'] = pod_aggregator.split('~~')[1]
                    monitor_labels['container.azm.ms/pod-aggregator-kind'] = pod_aggregator_kind
                    monitor_labels['container.azm.ms/namespace'] = namespace
                end
                return monitor_labels
            when HealthMonitorConstants::NODE_CPU_MONITOR_ID, HealthMonitorConstants::NODE_MEMORY_MONITOR_ID, HealthMonitorConstants::NODE_KUBELET_HEALTH_MONITOR_ID, HealthMonitorConstants::NODE_CONDITION_MONITOR_ID, HealthMonitorConstants::NODE_CONTAINER_RUNTIME_MONITOR_ID

                @@nodeInventory["items"].each do |node|
                    if !node_name.nil? && !node['metadata']['name'].nil? && node_name == node['metadata']['name']
                        #log.debug "Matched node name "
                        if !node["metadata"].nil? && !node["metadata"]["labels"].nil?
                            monitor_labels = node["metadata"]["labels"]
                        end
                    end
                end
                return monitor_labels
            end
        end

        def refreshKubernetesApiData(log, hostName, force: false)
            #log.debug "refreshKubernetesApiData"
            if ( ((Time.now.utc - Time.parse(@@lastRefreshTime)) / 60 ) < 5.0 && !force)
                log.debug "Less than 5 minutes since last refresh at #{@@lastRefreshTime}"
                return
            end
            if force
                @log.debug "Force Refresh"
            end

            begin

                @@nodeInventory = JSON.parse(KubernetesApiClient.getKubeResourceInfo("nodes").body)
                if !hostName.nil?
                    podInventory = JSON.parse(KubernetesApiClient.getKubeResourceInfo("pods?fieldSelector=spec.nodeName%3D#{hostName}").body)
                else
                    podInventory = JSON.parse(KubernetesApiClient.getKubeResourceInfo("pods").body)
                end
                podInventory['items'].each do |pod|
                    has_owner = !pod['metadata']['ownerReferences'].nil?
                    if !has_owner
                        pod_aggregator = pod['metadata']['name']
                    else
                        pod_aggregator = pod['metadata']['ownerReferences'][0]['name']
                    end
                    namespace = pod['metadata']['namespace']
                    @@controllerMapping[pod_aggregator] = namespace
                    #log.debug "pod_aggregator #{pod_aggregator} namespace #{namespace}"
                    pod['spec']['containers'].each do |container|
                        key = [pod['metadata']['uid'], container['name']].join('/')

                        if !container['resources'].empty? && !container['resources']['limits'].nil? && !container['resources']['limits']['cpu'].nil?
                            cpu_limit_value = KubernetesApiClient.getMetricNumericValue('cpu', container['resources']['limits']['cpu'])
                        else
                            @log.info "CPU limit not set for container : #{container['name']}. Using Node Capacity"
                            #TODO: Send warning health event #bestpractices
                            cpu_limit_value = @cpu_capacity
                        end

                        if !container['resources'].empty? && !container['resources']['limits'].nil? && !container['resources']['limits']['memory'].nil?
                            #@log.info "Raw Memory Value #{container['resources']['limits']['memory']}"
                            memory_limit_value = KubernetesApiClient.getMetricNumericValue('memory', container['resources']['limits']['memory'])
                        else
                            @log.info "Memory limit not set for container : #{container['name']}. Using Node Capacity"
                            memory_limit_value = @memory_capacity
                        end

                        @@containerMetadata[key] = {"cpuLimit" => cpu_limit_value, "memoryLimit" => memory_limit_value, "controllerName" => pod_aggregator, "namespace" => namespace}
                    end
                end
            rescue => e
                @log.info "Error Refreshing Container Resource Limits #{e.backtrace}"
            end
            # log.info "Controller Mapping #{@@controllerMapping}"
            # log.info "Node Inventory #{@@nodeInventory}"
            # log.info "Container Metadata #{@@containerMetadata}"
            # log.info "------------------------------------"
            @@lastRefreshTime = Time.now.utc.iso8601
        end

        def getContainerMetadata(key)
            if @@containerMetadata.has_key?(key)
                return @@containerMetadata[key]
            else
                # This is to handle new containers/controllers that might have come up since the last refresh
                @log.info "Adhoc refresh getContainerMetadata"
                HealthMonitorUtils.refreshKubernetesApiData(@log, nil, force: true)
                if @@containerMetadata.has_key?(key)
                    return @@containerMetadata[key]
                else
                    return nil
                end
            end
        end

        def getContainerMemoryLimit(key)
            if @@containerMetadata.has_key?(key)
                return @@containerMetadata[key]['memoryLimit']
            else
                @log.info "Adhoc refresh getContainerMemoryLimit"
                # This is to handle new containers/controllers that might have come up since the last refresh
                HealthMonitorUtils.refreshKubernetesApiData(@log,nil, force: true)
                if @@containerMetadata.has_key?(key)
                    return @@containerMetadata[key]['memoryLimit']
                else
                    return ''
                end
            end
        end

        def getContainerControllerName(key)
            if @@containerMetadata.has_key?(key)
                return @@containerMetadata[key]['controllerName']
            else
                @log.info "Adhoc refresh getContainerControllerName"
                # This is to handle new containers/controllers that might have come up since the last refresh
                HealthMonitorUtils.refreshKubernetesApiData(@log,nil, force: true)
                if @@containerMetadata.has_key?(key)
                    return @@containerMetadata[key]['controllerName']
                else
                    return ''
                end
            end
        end

        def getContainerNamespace(key)
            if @@containerMetadata.has_key?(key)
                return @@containerMetadata[key]['namespace']
            else
                @log.info "Adhoc refresh getContainerNamespace"
                # This is to handle new containers/controllers that might have come up since the last refresh
                HealthMonitorUtils.refreshKubernetesApiData(@log,nil, force: true)
                if @@containerMetadata.has_key?(key)
                    return @@containerMetadata[key]['namespace']
                else
                    return ''
                end
            end
        end

        # def getControllerNamespace(controller_name)
        #     if @@controllerMapping.has_key?(controller_name)
        #         return @@controllerMapping[controller_name]
        #     else
        #         @log.info "Adhoc refresh getControllerNamespace"
        #         # This is to handle new containers/controllers that might have come up since the last refresh
        #         HealthMonitorUtils.refreshKubernetesApiData(@log,nil, force: true)
        #         if @@controllerMapping.has_key?(controller_name)
        #             return @@controllerMapping[controller_name]
        #         else
        #             return ''
        #         end
        #     end
        # end

        def getClusterCpuMemoryCapacity
            begin
                node_inventory = JSON.parse(KubernetesApiClient.getKubeResourceInfo("nodes").body)
                cluster_cpu_capacity = 0.0
                cluster_memory_capacity = 0.0
                if !node_inventory.empty?
                    node_inventory['items'].each do |node|
                        cpu_capacity_json = KubernetesApiClient.parseNodeLimits(node_inventory, "capacity", "cpu", "cpuCapacityNanoCores")
                        if !cpu_capacity_json.nil?
                            cpu_capacity_json.each do |cpu_capacity_node|
                                if !cpu_capacity_node['DataItems'][0]['Collections'][0]['Value'].to_s.nil?
                                    cluster_cpu_capacity += cpu_capacity_node['DataItems'][0]['Collections'][0]['Value']
                                end
                            end
                            @log.info "Cluster CPU Limit #{cluster_cpu_capacity}"
                        else
                            @log.info "Error getting cpu_capacity"
                        end
                        memory_capacity_json = KubernetesApiClient.parseNodeLimits(node_inventory, "capacity", "memory", "memoryCapacityBytes")
                        if !memory_capacity_json.nil?
                            memory_capacity_json.each do |memory_capacity_node|
                                if !memory_capacity_node['DataItems'][0]['Collections'][0]['Value'].to_s.nil?
                                    cluster_memory_capacity += memory_capacity_node['DataItems'][0]['Collections'][0]['Value']
                                end
                            end
                            @log.info "Cluster Memory Limit #{cluster_memory_capacity}"
                        else
                            @log.info "Error getting memory_capacity"
                        end
                    end
                else
                    @log.info "Unable to get cpu and memory capacity"
                    return [0.0, 0.0]
                end
                return [cluster_cpu_capacity, cluster_memory_capacity]
            rescue => e
                @log.info e
            end
        end

        def getResourceSubscription(pod_inventory, metric_name, metric_capacity)
            subscription = 0.0
            if !pod_inventory.empty?
                pod_inventory['items'].each do |pod|
                    pod['spec']['containers'].each do |container|
                        if !container['resources']['requests'].nil? && !container['resources']['requests'][metric_name].nil?
                            subscription += KubernetesApiClient.getMetricNumericValue(metric_name, container['resources']['requests'][metric_name])
                        end
                    end
                end
            end
            #@log.debug "#{metric_name} Subscription  #{subscription}"
            return subscription
        end

        def getHealthMonitorConfig
            health_monitor_config = {}
            begin
                file = File.open('/opt/microsoft/omsagent/plugin/healthmonitorconfig.json', "r")
                if !file.nil?
                    fileContents = file.read
                    health_monitor_config = JSON.parse(fileContents)
                    file.close
                end
            rescue => e
                @log.info "Error when opening health config file #{e}"
            end
            return health_monitor_config
        end

        def getLogHandle
            return @log
        end

        def getPodsReadyHash(pod_inventory, deployment_inventory)
            pods_ready_percentage_hash = {}
            deployment_lookup = {}
            deployment_inventory['items'].each do |deployment|
                match_labels = deployment['spec']['selector']['matchLabels'].to_h
                namespace = deployment['metadata']['namespace']
                match_labels.each{|k,v|
                    deployment_lookup["#{namespace}-#{k}=#{v}"] = "#{deployment['metadata']['namespace']}~~#{deployment['metadata']['name']}"
                }
            end
            pod_inventory['items'].each do |pod|
                begin
                    # controller_name = pod['metadata']['ownerReferences'][0]['name']
                    # namespace = pod['metadata']['namespace']
                    # status = pod['status']['phase']

                    # if pods_ready_percentage_hash.key?(controller_name)
                    #     total_pods = pods_ready_percentage_hash[controller_name]['totalPods']
                    #     pods_ready = pods_ready_percentage_hash[controller_name]['podsReady']
                    # else
                    #     total_pods = 0
                    #     pods_ready = 0
                    # end

                    # total_pods += 1
                    # if status == 'Running'
                    #     pods_ready += 1
                    # end
                    # pods_ready_percentage_hash[controller_name] = {'totalPods' => total_pods, 'podsReady' => pods_ready, 'namespace' => namespace}
                    has_owner = !pod['metadata']['ownerReferences'].nil?
                    owner_kind = ''
                    if has_owner
                        owner_kind = pod['metadata']['ownerReferences'][0]['kind']
                        controller_name = pod['metadata']['ownerReferences'][0]['name']
                    else
                        owner_kind = pod['kind']
                        controller_name = pod['metadata']['name']
                        #@log.info "#{JSON.pretty_generate(pod)}"
                    end

                    namespace = pod['metadata']['namespace']
                    status = pod['status']['phase']

                    pod_aggregator = ''
                    if owner_kind.nil?
                        owner_kind = 'Pod'
                    end
                    case owner_kind.downcase
                    when 'job'
                        # we are excluding jobs
                        next
                    when 'replicaset'
                        # get the labels, and see if there is a match. If there is, it is the deployment. If not, use replica set name/controller name
                        labels = pod['metadata']['labels'].to_h
                        labels.each {|k,v|
                            lookup_key = "#{namespace}-#{k}=#{v}"
                            if deployment_lookup.key?(lookup_key)
                                pod_aggregator = deployment_lookup[lookup_key]
                                break
                            end
                        }
                        if pod_aggregator.empty?
                            pod_aggregator = "#{namespace}~~#{controller_name}"
                        end
                    when 'daemonset'
                        pod_aggregator = "#{namespace}~~#{controller_name}"
                    else
                        pod_aggregator = "#{namespace}~~#{pod['metadata']['name']}"
                    end

                    if pods_ready_percentage_hash.key?(pod_aggregator)
                        total_pods = pods_ready_percentage_hash[pod_aggregator]['totalPods']
                        pods_ready = pods_ready_percentage_hash[pod_aggregator]['podsReady']
                    else
                        total_pods = 0
                        pods_ready = 0
                    end

                    total_pods += 1
                    if status == 'Running'
                        pods_ready += 1
                    end

                    pods_ready_percentage_hash[pod_aggregator] = {'totalPods' => total_pods, 'podsReady' => pods_ready, 'namespace' => namespace, 'pod_aggregator' => pod_aggregator, 'kind' => owner_kind}
                rescue => e
                    @log.info "Error when processing pod #{pod['metadata']['name']} #{e.message}"
                end
            end
            return pods_ready_percentage_hash
        end

        def getNodeStateFromNodeConditions(node_conditions)
            pass = false
            node_conditions.each do |condition|
                type = condition['type']
                status = condition['status']

                if ((type == "NetworkUnavailable" || type == "OutOfDisk") && (status == 'True' || status == 'Unknown'))
                    return "fail"
                elsif ((type == "DiskPressure" || type == "MemoryPressure" || type == "PIDPressure") && (status == 'True' || status == 'Unknown'))
                    return "warn"
                elsif type == "Ready" &&  status == 'True'
                    pass = true
                end
            end

            if pass
                return "pass"
            else
                return "fail"
            end
        end
    end
end
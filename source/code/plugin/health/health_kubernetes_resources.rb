module HealthModel
    class HealthKubernetesResources

        attr_accessor :node_inventory, :pod_inventory
        attr_reader :nodes, :pods

        def initialize(node_inventory, pod_inventory, deployment_inventory)
            @node_inventory = node_inventory || []
            @pod_inventory = pod_inventory || []
            @deployment_inventory = deployment_inventory || []
            @nodes = []
            @pods = []
            @workloads = get_workload_names

            @node_inventory['items'].each {|node|
                @nodes.push(node['metadata']['name'])
            }
        end

        def get_node_inventory
            return @node_inventory
        end

        def get_nodes
            return @nodes
        end

        def get_pod_inventory
            return @pod_inventory
        end

        def get_pods
            return @pods
        end

        def get_workload_names
            workload_names = {}
            deployment_lookup = {}
            @deployment_inventory['items'].each do |deployment|
                match_labels = deployment['spec']['selector']['matchLabels'].to_h
                namespace = deployment['metadata']['namespace']
                match_labels.each{|k,v|
                    deployment_lookup["#{namespace}-#{k}=#{v}"] = "#{deployment['metadata']['namespace']}~~#{deployment['metadata']['name']}"
                }
            end
            @pod_inventory['items'].each do |pod|
                begin
                    has_owner = !pod['metadata']['ownerReferences'].nil?
                    owner_kind = ''
                    if has_owner
                        owner_kind = pod['metadata']['ownerReferences'][0]['kind']
                        controller_name = pod['metadata']['ownerReferences'][0]['name']
                    else
                        owner_kind = pod['kind']
                        controller_name = pod['metadata']['name']
                    end

                    namespace = pod['metadata']['namespace']
                    status = pod['status']['phase']

                    workload_name = ''
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
                                workload_name = deployment_lookup[lookup_key]
                                break
                            end
                        }
                        if workload_name.empty?
                            workload_name = "#{namespace}~~#{controller_name}"
                        end
                    when 'daemonset'
                        workload_name = "#{namespace}~~#{controller_name}"
                    else
                        workload_name = "#{namespace}~~#{pod['metadata']['name']}"
                    end
                rescue => e
                    @log.info "Error when processing pod #{pod['metadata']['name']} #{e.message}"
                end
                workload_names[workload_name] = true
            end
            return workload_names.keys
        end
    end
end
module HealthModel
    class HealthKubernetesResources

        attr_accessor :node_inventory, :pod_inventory

        def initialize(node_inventory, pod_inventory)
            @node_inventory = node_inventory || []
            @pod_inventory = pod_inventory || []
        end

        def get_node_inventory
            return @node_inventory
        end

        def get_pod_inventory
            return @pod_inventory
        end
    end
end
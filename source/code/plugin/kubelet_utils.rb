# Copyright (c) Microsoft Corporation.  All rights reserved.
#!/usr/local/bin/ruby
# frozen_string_literal: true

require_relative 'CAdvisorMetricsAPIClient'

class KubeletUtils
    class << self
        def get_node_capacity
            
            cpu_capacity = 1.0
            memory_capacity = 1.0

            response = CAdvisorMetricsAPIClient.getNodeCapacityFromCAdvisor(winNode: nil)
            if !response.nil? && !response.body.nil?
                cpu_capacity = JSON.parse(response.body)["num_cores"].nil? ? 1.0 : (JSON.parse(response.body)["num_cores"] * 1000.0)
                memory_capacity = JSON.parse(response.body)["memory_capacity"].nil? ? 1.0 : JSON.parse(response.body)["memory_capacity"].to_f
                $log.info "CPU = #{cpu_capacity}mc Memory = #{memory_capacity/1024/1024}MB"
                return [cpu_capacity, memory_capacity]
            end
        end

        def getContainerInventoryRecords(batchTime, clusterCollectEnvironmentVar)    
            containerInventoryRecords = Array.new        
            response = CAdvisorMetricsAPIClient.getPodsFromCAdvisor(winNode: nil)
            if !response.nil? && !response.body.nil?
                podList = JSON.parse(response.body)
                if !podList.nil? && !podList.empty? 
                    podList["items"].each do |item| 
                        nodeName = item["spec"]["nodeName"]
                        containerInstance = {}                      
                        status = item.status                        
                        if !status.nil? && !status.empty?
                            containerStatuses = status.containerStatuses
                            if !containerStatuses.nil? && !containerStatuses.empty?
                               containerStatuses.each do |item| 
                                 containerInstance["ElementName"] = containerStatuses["name"]
                                 containerInstance["InstanceID"] = container["containerID"]
                                 containerInstance["Computer"] = nodeName
                                 containerInstance["CollectionTime"] = batchTime #This is the time that is mapped to become TimeGenerated
                               end
                            end 
                        end   
                        containerSpec = item.spec
                        containerInventoryRecords.push containerInstance
                    end    
                end
            end

            return containerInventoryRecords
        end

    end
end
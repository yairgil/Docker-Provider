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
            begin
                response = CAdvisorMetricsAPIClient.getPodsFromCAdvisor(winNode: nil)
                if !response.nil? && !response.body.nil?
                    podList = JSON.parse(response.body)
                    if !podList.nil? && !podList.empty? 
                        podList["items"].each do |item|
                            containersInfoMap = getContainersInfoMap(spec, clusterCollectEnvironmentVar)
                            containerInstance = {}                                                                                          
                            if !item["status"].nil? && !item["status"].empty?                              
                                if !item["status"]["containerStatuses"].nil? && !item["status"]["containerStatuses"].empty?
                                    item["status"]["containerStatuses"].each do |containerStatus| 
                                     containerInstance["CollectionTime"] = batchTime #This is the time that is mapped to become TimeGenerated    
                                     containerName = containerStatus["name"]    
                                     containerInstance["ContainerID"] = containerStatus["containerID"].split('//')[0]
                                     containerInfoMap = containersInfoMap[containerName]  
                                     containerInstance["ElementName"] = containerInfoMap["ElementName"]
                                     containerInstance["Computer"] = containerInfoMap["Computer"]
                                     containerInstance["ContainerHostname"] = containerInfoMap["ContainerHostname"]
                                     containerInstance["CreatedTime"] = containerInfoMap["CreatedTime"]
                                     containerInstance["EnvironmentVar"] = containerInfoMap["EnvironmentVar"]
                                     containerInstance["Ports"] = containerInfoMap["Ports"]
                                     containerInstance["Command"] = containerInfoMap["Command"]
                                     containerInventoryRecords.push containerInstance
                                    end
                                end 
                            end                                                      
                        end    
                    end
                end
            rescue => error
                @Log.warn("kubelet_utils::getContainerInventoryRecords : Get Container Inventory Records failed: #{error}")
            end          
            return containerInventoryRecords
        end

        def getContainersInfoMap(item, clusterCollectEnvironmentVar)
            containersInfoMap = {}
            begin
                createdTime = item["metadata"]["creationTimestamp"]                  
                nodeName = (!item["spec"]["nodeName"].nil?) ? item["spec"]["nodeName"] : ""                
                item["spec"]["containers"].each do |container|
                    containerInfoMap = {}
                    containerName = container["name"]    
                    containerInfoMap["ElementName"] = containerName
                    containerInfoMap["Computer"] = nodeName             
                    containerInfoMap["ContainerHostname"] = nodeName
                    containerInfoMap["CreatedTime"] = createdTime
                    envValue = container["env"]
                    envValueString = (envValue.nil?) ? "" : envValue.to_s
                    containerInfoMap["EnvironmentVar"] = envValueString  
                    portsValue = container["ports"]
                    portsValueString = (portsValue.nil?) ? "" : portsValue.to_s     
                    containerInfoMap["Ports"] = portsValueString    
                    argsValue = container["args"]
                    argsValueString = (argsValue.nil?) ? "" : argsValue.to_s     
                    containerInfoMap["Command"] = argsValueString

                    containersInfoMap[containerName] = containerInfoMap         
                end                 
            rescue => error      
                @Log.warn("kubelet_utils::getContainersInfoMap : Get Container Info Maps failed: #{error}")          
            end
            return containersInfoMap
        end  
    end
end
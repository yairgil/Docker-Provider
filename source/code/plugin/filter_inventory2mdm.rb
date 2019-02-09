# Copyright (c) Microsoft Corporation.  All rights reserved.

# frozen_string_literal: true

module Fluent
    require 'logger'
    require 'json'
    require_relative 'oms_common'

	class Inventory2MdmFilter < Filter
		Fluent::Plugin.register_filter('filter_inventory2mdm', self)
		
		config_param :enable_log, :integer, :default => 0
        config_param :log_path, :string, :default => '/var/opt/microsoft/docker-cimprov/log/filter_inventory2mdm.log'
        config_param :custom_metrics_azure_regions, :string
        
        @@node_count_metric_name = 'nodeCount'
        @@pod_count_metric_name = 'podCount'
        @@pod_inventory_tag = 'mdm.kubepodinventory'
        @@node_inventory_tag = 'mdm.kubenodeinventory'
        @@node_status_ready = 'Ready'
        @@node_status_not_ready = 'NotReady'

        @@node_inventory_custom_metrics_template = '
            { 
                "time": "%{timestamp}", 
                "data": { 
                    "baseData": { 
                        "metric": "%{metricName}", 
                        "namespace": "insights.container/nodes", 
                        "dimNames": [ 
                        "status"
                        ], 
                        "series": [ 
                        { 
                            "dimValues": [ 
                            "%{statusValue}"
                            ], 
                            "min": %{node_status_count},
                            "max": %{node_status_count}, 
                            "sum": %{node_status_count}, 
                            "count": 1
                        } 
                        ] 
                    } 
                } 
            }'

        @@pod_inventory_custom_metrics_template = '
            { 
                "time": "%{timestamp}", 
                "data": { 
                    "baseData": { 
                        "metric": "%{metricName}", 
                        "namespace": "insights.container/nodes", 
                        "dimNames": [ 
                        "podName",
                        "phase", 
                        "namespace", 
                        "node", 
                        "controllerName"
                        ], 
                        "series": [ 
                        { 
                            "dimValues": [ 
                            "%{podNameDimValue}",
                            "%{phaseDimValue}", 
                            "%{namespaceDimValue}", 
                            "%{nodeDimValue}", 
                            "%{controllerNameDimValue}"
                            ], 
                            "min": 1,
                            "max": 1, 
                            "sum": 1, 
                            "count": 1 
                        } 
                        ] 
                    } 
                } 
            }'

        @process_incoming_stream = true

		def initialize
            super
		end

		def configure(conf)
			super
			@log = nil
			
			if @enable_log
				@log = Logger.new(@log_path, 'weekly')
				@log.debug {'Starting filter_inventory2mdm plugin'}
			end
		end

        def start
            super
            @process_incoming_stream = check_custom_metrics_availability
            @log.debug "After check_custom_metrics_availability process_incoming_stream #{@process_incoming_stream}"
        end

        def check_custom_metrics_availability
            aks_region = ENV['AKS_REGION']
            aks_resource_id = ENV['AKS_RESOURCE_ID']
            if aks_region.to_s.empty? && aks_resource_id.to_s.empty?
                return false # This will also take care of AKS-Engine Scenario. AKS_REGION/AKS_RESOURCE_ID is not set for AKS-Engine. Only ACS_RESOURCE_NAME is set
            end
            @log.debug "AKS_REGION #{aks_region}"
            custom_metrics_regions_arr = @custom_metrics_azure_regions.split(',')
            custom_metrics_regions_hash = custom_metrics_regions_arr.map {|x| [x.downcase,true]}.to_h

            @log.debug "Custom Metrics Regions Hash #{custom_metrics_regions_hash}"

            if custom_metrics_regions_hash.key?(aks_region.downcase)
                @log.debug "Returning true for check_custom_metrics_availability"
                return true
            else 
                @log.debug "Returning false for check_custom_metrics_availability"
                return false
            end
        end

		def shutdown
			super
        end
        
        # KUBE_NODE_INVENTORY
        #{"DataType":"KUBE_NODE_INVENTORY_BLOB","IPName":"ContainerInsights","DataItems":[{"CollectionTime":"2019-02-07T21:51:12Z","Computer":"aks-agentpool-17494674-0","ClusterName":"dilipr-std","ClusterId":"/subscriptions/72c8e8ca-dc16-47dc-b65c-6b5875eb600a/resourcegroups/dilipr-std/providers/Microsoft.ContainerService/managedClusters/dilipr-std","CreationTimeStamp":"2018-10-03T15:41:45Z","Labels":[{"agentpool":"agentpool","beta.kubernetes.io/arch":"amd64","beta.kubernetes.io/instance-type":"Standard_DS2_v2","beta.kubernetes.io/os":"linux","failure-domain.beta.kubernetes.io/region":"eastus","failure-domain.beta.kubernetes.io/zone":"0","kubernetes.azure.com/cluster":"MC_dilipr-std_dilipr-std_eastus","kubernetes.io/hostname":"aks-agentpool-17494674-0","kubernetes.io/role":"agent","storageprofile":"managed","storagetier":"Premium_LRS"}],"Status":"Ready","LastTransitionTimeReady":"2019-01-14T23:41:05Z","KubeletVersion":"v1.11.3","KubeProxyVersion":"v1.11.3"}]}

        # KUBE_POD_INVENTORY
        #{"DataType":"KUBE_POD_INVENTORY_BLOB","IPName":"ContainerInsights","DataItems":[{"CollectionTime":"2019-02-07T21:51:13Z","Name":"addon-http-application-routing-default-http-backend-5ccb95vldz8","PodUid":"db7393f1-0242-11e9-a2f8-62ffdcb08be4","PodLabel":[{"app":"addon-http-application-routing-default-http-backend","pod-template-hash":"1776511215"}],"Namespace":"kube-system","PodCreationTimeStamp":"2018-12-17T21:29:38Z","PodStartTime":"2018-12-17T21:29:50Z","PodStatus":"Running","PodIp":"10.244.0.68","Computer":"aks-agentpool-17494674-0","ClusterId":"/subscriptions/72c8e8ca-dc16-47dc-b65c-6b5875eb600a/resourcegroups/dilipr-std/providers/Microsoft.ContainerService/managedClusters/dilipr-std","ClusterName":"dilipr-std","ServiceName":"addon-http-application-routing-default-http-backend","ControllerKind":"ReplicaSet","ControllerName":"addon-http-application-routing-default-http-backend-5ccb955659","PodRestartCount":0,"ContainerID":"9d5fd1a74326885af569c9eb0faa845c386cdbb27e00646566e8f27e7c3b5579","ContainerName":"db7393f1-0242-11e9-a2f8-62ffdcb08be4/addon-http-application-routing-default-http-backend","ContainerRestartCount":0,"ContainerStatus":"running","ContainerCreationTimeStamp":"2018-12-17T21:30:05Z"}]}

        def process_node_inventory_records(es)
            timestamp = DateTime.now
            
            begin
                node_ready_count = 0
                node_not_ready_count = 0
                records = []
                
                es.each{|time,record|
                    begin
                        timestamp = record['DataItems'][0]['CollectionTime']
                        node_status = record['DataItems'][0]['Status']
                        @log.info "Node Status: #{node_status}"
                        @log.info "Node Name: #{record['DataItems'][0]['Computer']}"
                        if node_status.downcase == @@node_status_ready.downcase
                            node_ready_count = node_ready_count+1
                        else
                            node_not_ready_count = node_not_ready_count + 1
                        end
                    rescue => e
                    end
                }

                @log.info "Node Ready Count : #{node_ready_count}"
                @log.info "Node Not Ready Count : #{node_not_ready_count}"
                @log.info "Timestamp for Node Metrics: #{timestamp}"

                ready_record = @@node_inventory_custom_metrics_template % {
                    timestamp: timestamp,
                    metricName: @@node_count_metric_name, 
                    statusValue: @@node_status_ready,
                    node_status_count: node_ready_count
                }
                records.push(JSON.parse(ready_record))
                
                not_ready_record = @@node_inventory_custom_metrics_template % {
                    timestamp: timestamp,
                    metricName: @@node_count_metric_name, 
                    statusValue: @@node_status_not_ready,
                    node_status_count: node_not_ready_count
                }
                records.push(JSON.parse(not_ready_record))
            rescue Exception => e
                @log.info "Error processing node inventory records Exception: #{e.class} Message: #{e.message}"
                ApplicationInsightsUtility.sendExceptionTelemetry(e.backtrace)
                return [],timestamp
            end
            return records,timestamp
        end

        def process_pod_inventory_records(es)
            timestamp = DateTime.now
            begin
                records = []
                es.each{|time,record|
                    
                    timestamp = record['DataItems'][0]['CollectionTime']
                    podNameDimValue = record['DataItems'][0]['Name']
                    podStatusDimValue = record['DataItems'][0]['PodStatus']
                    podNamespaceDimValue = record['DataItems'][0]['Namespace']
                    podServiceNameDimValue = record['DataItems'][0]['ServiceName']
                    podControllerNameDimValue = record['DataItems'][0]['ControllerName']
                    podNodeDimValue = record['DataItems'][0]['Computer']

                    record = @@pod_inventory_custom_metrics_template % {
                        timestamp: timestamp,
                        metricName: @@pod_count_metric_name,
                        podNameDimValue: podNameDimValue, 
                        phaseDimValue: podStatusDimValue,
                        namespaceDimValue: podNamespaceDimValue, 
                        nodeDimValue: podNodeDimValue, 
                        controllerNameDimValue: podControllerNameDimValue
                    }
                    records.push(JSON.parse(record))
                }
            rescue Exception => e
                @log.info "Error processing pod inventory record Exception: #{e.class} Message: #{e.message}"
                ApplicationInsightsUtility.sendExceptionTelemetry(e.backtrace)
                return [],timestamp
            end
            return records, timestamp
        end

        def filter_stream(tag, es)
            new_es = MultiEventStream.new
            filtered_records = []
            time = DateTime.now
            begin
                if @process_incoming_stream
                    @log.info 'Processing NODE inventory records in filter plugin to send to MDM'
                    if tag.downcase.start_with?(@@node_inventory_tag)
                        filtered_records, time = process_node_inventory_records(es)
                    elsif tag.downcase.start_with?(@@pod_inventory_tag)
                        @log.info 'Processing POD inventory records in filter plugin to send to MDM'
                        filtered_records, time = process_pod_inventory_records(es)
                    else 
                        filtered_records = []
                    end
                end
                filtered_records.each {|filtered_record| 
                    new_es.add(time, filtered_record) if filtered_record
                } if filtered_records
            rescue => e
                @log.info "Exception in filter_stream #{e}"
            end
            new_es
        end
    end
end

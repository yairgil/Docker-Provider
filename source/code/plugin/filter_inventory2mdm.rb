# Copyright (c) Microsoft Corporation.  All rights reserved.

# frozen_string_literal: true

module Fluent
    require 'logger'
    require 'json'
    require_relative 'oms_common'
    require_relative 'CustomMetricsUtils'

	class Inventory2MdmFilter < Filter
		Fluent::Plugin.register_filter('filter_inventory2mdm', self)
		
		config_param :enable_log, :integer, :default => 0
        config_param :log_path, :string, :default => '/var/opt/microsoft/docker-cimprov/log/filter_inventory2mdm.log'
        config_param :custom_metrics_azure_regions, :string
        
        @@node_count_metric_name = 'nodesCount'
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
                        "namespace": "insights.container/pods", 
                        "dimNames": [ 
                        "phase", 
                        "Kubernetes namespace", 
                        "node", 
                        "controllerName"
                        ], 
                        "series": [ 
                        { 
                            "dimValues": [ 
                            "%{phaseDimValue}", 
                            "%{namespaceDimValue}", 
                            "%{nodeDimValue}", 
                            "%{controllerNameDimValue}"
                            ], 
                            "min": %{podCountMetricValue},
                            "max": %{podCountMetricValue}, 
                            "sum": %{podCountMetricValue}, 
                            "count": 1 
                        } 
                        ] 
                    } 
                } 
            }'
        
        @@pod_phase_values = ['Running', 'Pending', 'Succeeded', 'Failed', 'Unknown']
        
        @process_incoming_stream = true

		def initialize
            super
		end

		def configure(conf)
			super
			@log = nil
			
			if @enable_log
				@log = Logger.new(@log_path, 0, 10000000) #don't save prior logs
				@log.debug {'Starting filter_inventory2mdm plugin'}
			end
		end

        def start
            super
            @process_incoming_stream = CustomMetricsUtils.check_custom_metrics_availability(@custom_metrics_azure_regions)
            @log.debug "After check_custom_metrics_availability process_incoming_stream #{@process_incoming_stream}"
        end

		def shutdown
			super
        end
        
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
                        if node_status.downcase == @@node_status_ready.downcase
                            node_ready_count = node_ready_count+1
                        else
                            node_not_ready_count = node_not_ready_count + 1
                        end
                    rescue => e
                    end
                }

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
            pod_count_hash = Hash.new
            no_phase_dim_values_hash = Hash.new
            begin
                records = []
                es.each{|time,record|
                    
                    timestamp = record['DataItems'][0]['CollectionTime']
                    podPhaseDimValue = record['DataItems'][0]['PodStatus']
                    podNamespaceDimValue = record['DataItems'][0]['Namespace']
                    podControllerNameDimValue = record['DataItems'][0]['ControllerName']
                    podNodeDimValue = record['DataItems'][0]['Computer']
                    
                    # group by distinct dimension values
                    pod_key = [podNodeDimValue, podNamespaceDimValue, podControllerNameDimValue, podPhaseDimValue].join('~~')
                    
                    if pod_count_hash.key?(pod_key) 
                        pod_count = pod_count_hash[pod_key]
                        pod_count = pod_count + 1
                        pod_count_hash[pod_key] = pod_count
                    else
                        pod_count = 1
                        pod_count_hash[pod_key] = pod_count
                    end

                    # Collect all possible combinations of dimension values other than pod phase
                    key_without_phase_dim_value = [podNodeDimValue, podNamespaceDimValue, podControllerNameDimValue].join('~~')
                    if no_phase_dim_values_hash.key?(key_without_phase_dim_value)
                        @log.info "#{key_without_phase_dim_value} already present in #{no_phase_dim_values_hash}"
                        next
                    else
                        @log.info "Adding #{key_without_phase_dim_value} to #{no_phase_dim_values_hash}"
                        no_phase_dim_values_hash[key_without_phase_dim_value] = true
                    end
                }

                # generate all possible values of non_phase_dim_values X pod Phases and zero-fill the ones that are not already present
                no_phase_dim_values_hash.each {|key, value|
                    @@pod_phase_values.each{|phase|
                        pod_key = [key, phase].join('~~')
                        if !pod_count_hash.key?(pod_key)
                            pod_count_hash[pod_key] = 0
                            @log.info "Zero filled #{pod_key}"
                        else
                            next
                        end
                    }
                }

                pod_count_hash.each {|key, value|

                    key_elements = key.split('~~')
                    if key_elements.length != 4
                        next
                    end

                    # get dimension values by key
                    podNodeDimValue = key_elements[0]
                    podNamespaceDimValue = key_elements[1]
                    podControllerNameDimValue = key_elements[2]
                    podPhaseDimValue = key_elements[3]

                    record = @@pod_inventory_custom_metrics_template % {
                        timestamp: timestamp,
                        metricName: @@pod_count_metric_name,
                        phaseDimValue: podPhaseDimValue,
                        namespaceDimValue: podNamespaceDimValue, 
                        nodeDimValue: podNodeDimValue, 
                        controllerNameDimValue: podControllerNameDimValue, 
                        podCountMetricValue: value
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

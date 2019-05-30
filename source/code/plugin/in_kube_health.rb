#!/usr/local/bin/ruby
# frozen_string_literal: true

include HealthModel

module Fluent
  class KubeHealthInput < Input
    Plugin.register_input("kubehealth", self)

    @@clusterCpuCapacity = 0.0
    @@clusterMemoryCapacity = 0.0

    def initialize
      super
      require "yaml"
      require "json"

      require_relative "KubernetesApiClient"
      require_relative "oms_common"
      require_relative "omslog"
      require_relative "ApplicationInsightsUtility"
      require_relative 'HealthMonitorUtils'
      require_relative 'HealthMonitorState'
      require_relative 'health/health_model_constants'
    end
    include HealthModel
    config_param :run_interval, :time, :default => "1m"
    config_param :tag, :string, :default => "oms.api.KubeHealth.ReplicaSet"

    def configure(conf)
      super
    end

    def start
      if @run_interval
        @finished = false
        @condition = ConditionVariable.new
        @mutex = Mutex.new
        @thread = Thread.new(&method(:run_periodic))

        @@clusterName = KubernetesApiClient.getClusterName
        @@clusterId = KubernetesApiClient.getClusterId
        @@clusterRegion = KubernetesApiClient.getClusterRegion
        cluster_capacity = HealthMonitorUtils.getClusterCpuMemoryCapacity
        @@clusterCpuCapacity = cluster_capacity[0]
        @@clusterMemoryCapacity = cluster_capacity[1]
        @@healthMonitorConfig = HealthMonitorUtils.getHealthMonitorConfig
        @@hmlog = HealthMonitorUtils.getLogHandle
        @@hmlog.info "Cluster CPU Capacity: #{@@clusterCpuCapacity} Memory Capacity: #{@@clusterMemoryCapacity}"
        ApplicationInsightsUtility.sendCustomEvent("in_kube_health Plugin Start", {})
      end
    end

    def shutdown
      if @run_interval
        @mutex.synchronize {
          @finished = true
          @condition.signal
        }
        @thread.join
      end
    end

    def enumerate
      begin
        currentTime = Time.now
        emitTime = currentTime.to_f
        batchTime = currentTime.utc.iso8601
        health_monitor_records = []
        eventStream = MultiEventStream.new

        hmlog = HealthMonitorUtils.getLogHandle
        HealthMonitorUtils.refreshKubernetesApiData(@@hmlog, nil)
        # we do this so that if the call fails, we get a response code/header etc.
        node_inventory_response = KubernetesApiClient.getKubeResourceInfo("nodes")
        node_inventory = JSON.parse(node_inventory_response.body)
        pod_inventory_response = KubernetesApiClient.getKubeResourceInfo("pods")
        pod_inventory = JSON.parse(pod_inventory_response.body)

        deployment_inventory = JSON.parse(KubernetesApiClient.getKubeResourceInfo("deployments", api_version: "extensions/v1beta1").body)

        if node_inventory_response.code.to_i != 200
          record = process_kube_api_up_monitor("fail", node_inventory_response)
          health_monitor_records.push(record) if record
        else
          record = process_kube_api_up_monitor("pass", node_inventory_response)
          health_monitor_records.push(record) if record
        end

        if !pod_inventory.nil?
          record = process_cpu_oversubscribed_monitor(pod_inventory)
          health_monitor_records.push(record) if record
          record = process_memory_oversubscribed_monitor(pod_inventory)
          health_monitor_records.push(record) if record
          pods_ready_hash = HealthMonitorUtils.getPodsReadyHash(pod_inventory, deployment_inventory)

          system_pods = pods_ready_hash.select{|k,v| v['namespace'] == 'kube-system'}
          workload_pods = pods_ready_hash.select{|k,v| v['namespace'] != 'kube-system'}

          system_pods_ready_percentage_records = process_pods_ready_percentage(system_pods, HealthMonitorConstants::USER_WORKLOAD_PODS_READY_MONITOR_ID)
          system_pods_ready_percentage_records.each do |record|
            health_monitor_records.push(record) if record
          end

          workload_pods_ready_percentage_records = process_pods_ready_percentage(workload_pods, HealthMonitorConstants::SYSTEM_WORKLOAD_PODS_READY_MONITOR_ID)
          workload_pods_ready_percentage_records.each do |record|
            health_monitor_records.push(record) if record
          end
        else
            hmlog.info "POD INVENTORY IS NIL"
        end

        if !node_inventory.nil?
          node_condition_records = process_node_condition_monitor(node_inventory)
          node_condition_records.each do |record|
            health_monitor_records.push(record) if record
          end
        else
            hmlog.info "NODE INVENTORY IS NIL"
        end

        #@@hmlog.debug "Health Monitor Records Size #{health_monitor_records.size}"

        health_monitor_records.each do |record|
          eventStream.add(emitTime, record)
        end
        router.emit_stream(@tag, eventStream) if eventStream
      rescue => errorStr
        @@hmlog.warn("error in_kube_health: #{errorStr.to_s}")
        @@hmlog.debug "backtrace Input #{errorStr.backtrace}"
        ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
      end
    end

    def process_cpu_oversubscribed_monitor(pod_inventory)
      timestamp = Time.now.utc.iso8601
      subscription = HealthMonitorUtils.getResourceSubscription(pod_inventory,"cpu", @@clusterCpuCapacity)
      state =  subscription > @@clusterCpuCapacity ? "fail" : "pass"
      #@@hmlog.debug "CPU Oversubscribed Monitor State : #{state}"

      #CPU
      monitor_id = HealthMonitorConstants::WORKLOAD_CPU_OVERSUBSCRIBED_MONITOR_ID
      health_monitor_record = {"timestamp" => timestamp, "state" => state, "details" => {"clusterCpuCapacity" => @@clusterCpuCapacity/1000000.to_f, "clusterCpuRequests" => subscription/1000000.to_f}}
      # @@hmlog.info health_monitor_record

      monitor_instance_id = HealthMonitorUtils.getMonitorInstanceId(@@hmlog, monitor_id, [@@clusterId])
      #hmlog.info "Monitor Instance Id: #{monitor_instance_id}"
      HealthMonitorState.updateHealthMonitorState(@@hmlog, monitor_instance_id, health_monitor_record, @@healthMonitorConfig[monitor_id])
      #record = HealthMonitorSignalReducer.reduceSignal(@@hmlog, monitor_id, monitor_instance_id, @@healthMonitorConfig[monitor_id])
      health_record = {}
      time_now = Time.now.utc.iso8601
      health_record[HealthMonitorRecordFields::MONITOR_ID] = monitor_id
      health_record[HealthMonitorRecordFields::MONITOR_INSTANCE_ID] = monitor_instance_id
      health_record[HealthMonitorRecordFields::DETAILS] = health_monitor_record
      health_record[HealthMonitorRecordFields::AGENT_COLLECTION_TIME] =  time_now
      health_record[HealthMonitorRecordFields::TIME_FIRST_OBSERVED] =  time_now
      health_record[HealthMonitorRecordFields::CLUSTER_ID] = KubernetesApiClient.getClusterId
      @@hmlog.info "Successfully processed process_cpu_oversubscribed_monitor"
      return health_record
    end

    def process_memory_oversubscribed_monitor(pod_inventory)
      timestamp = Time.now.utc.iso8601
      subscription = HealthMonitorUtils.getResourceSubscription(pod_inventory,"memory", @@clusterMemoryCapacity)
      state =  subscription > @@clusterMemoryCapacity ? "fail" : "pass"
      #@@hmlog.debug "Memory Oversubscribed Monitor State : #{state}"

      #CPU
      monitor_id = HealthMonitorConstants::WORKLOAD_MEMORY_OVERSUBSCRIBED_MONITOR_ID
      health_monitor_record = {"timestamp" => timestamp, "state" => state, "details" => {"clusterMemoryCapacity" => @@clusterMemoryCapacity.to_f, "clusterMemoryRequests" => subscription.to_f}}
      hmlog = HealthMonitorUtils.getLogHandle

      monitor_instance_id = HealthMonitorUtils.getMonitorInstanceId(@@hmlog, monitor_id, [@@clusterId])
      HealthMonitorState.updateHealthMonitorState(@@hmlog, monitor_instance_id, health_monitor_record, @@healthMonitorConfig[monitor_id])
      #record = HealthMonitorSignalReducer.reduceSignal(@@hmlog, monitor_id, monitor_instance_id, @@healthMonitorConfig[monitor_id])
      health_record = {}
      time_now = Time.now.utc.iso8601
      health_record[HealthMonitorRecordFields::MONITOR_ID] = monitor_id
      health_record[HealthMonitorRecordFields::MONITOR_INSTANCE_ID] = monitor_instance_id
      health_record[HealthMonitorRecordFields::DETAILS] = health_monitor_record
      health_record[HealthMonitorRecordFields::AGENT_COLLECTION_TIME] =  time_now
      health_record[HealthMonitorRecordFields::TIME_FIRST_OBSERVED] =  time_now
      health_record[HealthMonitorRecordFields::CLUSTER_ID] = KubernetesApiClient.getClusterId
      @@hmlog.info "Successfully processed process_memory_oversubscribed_monitor"
      return health_record
    end

    def process_kube_api_up_monitor(state, response)
      timestamp = Time.now.utc.iso8601

      monitor_id = HealthMonitorConstants::KUBE_API_STATUS
      details = response.each_header.to_h
      details['ResponseCode'] = response.code
      health_monitor_record = {"timestamp" => timestamp, "state" => state, "details" => details}
      hmlog = HealthMonitorUtils.getLogHandle
      #hmlog.info health_monitor_record

      monitor_instance_id = HealthMonitorUtils.getMonitorInstanceId(@@hmlog, monitor_id, [@@clusterId])
      #hmlog.info "Monitor Instance Id: #{monitor_instance_id}"
      HealthMonitorState.updateHealthMonitorState(@@hmlog, monitor_instance_id, health_monitor_record, @@healthMonitorConfig[monitor_id])
      #record = HealthMonitorSignalReducer.reduceSignal(@@hmlog, monitor_id, monitor_instance_id, @@healthMonitorConfig[monitor_id])
      health_record = {}
      time_now = Time.now.utc.iso8601
      health_record[HealthMonitorRecordFields::MONITOR_ID] = monitor_id
      health_record[HealthMonitorRecordFields::MONITOR_INSTANCE_ID] = monitor_instance_id
      health_record[HealthMonitorRecordFields::DETAILS] = health_monitor_record
      health_record[HealthMonitorRecordFields::AGENT_COLLECTION_TIME] =  time_now
      health_record[HealthMonitorRecordFields::TIME_FIRST_OBSERVED] =  time_now
      health_record[HealthMonitorRecordFields::CLUSTER_ID] = KubernetesApiClient.getClusterId
      @@hmlog.info "Successfully processed process_kube_api_up_monitor"
      return health_record
    end

    def process_pods_ready_percentage(pods_hash, config_monitor_id)
      monitor_config = @@healthMonitorConfig[config_monitor_id]
      hmlog = HealthMonitorUtils.getLogHandle

      records = []
      pods_hash.keys.each do |key|
        pod_aggregator = key
        total_pods = pods_hash[pod_aggregator]['totalPods']
        pods_ready = pods_hash[pod_aggregator]['podsReady']
        namespace = pods_hash[pod_aggregator]['namespace']
        pod_aggregator_kind = pods_hash[pod_aggregator]['kind']
        percent = pods_ready / total_pods * 100
        timestamp = Time.now.utc.iso8601

        state = HealthMonitorState.getState(@@hmlog, (100-percent), monitor_config)
        health_monitor_record = {"timestamp" => timestamp, "state" => state, "details" => {"totalPods" => total_pods, "podsReady" => pods_ready, "podAggregator" => pod_aggregator, "namespace" => namespace, "podAggregatorKind" => pod_aggregator_kind}}
        monitor_instance_id = HealthMonitorUtils.getMonitorInstanceId(@@hmlog, config_monitor_id, [@@clusterId, namespace, pod_aggregator])
        HealthMonitorState.updateHealthMonitorState(@@hmlog, monitor_instance_id, health_monitor_record, monitor_config)
        health_record = {}
        time_now = Time.now.utc.iso8601
        health_record[HealthMonitorRecordFields::MONITOR_ID] = config_monitor_id
        health_record[HealthMonitorRecordFields::MONITOR_INSTANCE_ID] = monitor_instance_id
        health_record[HealthMonitorRecordFields::DETAILS] = health_monitor_record
        health_record[HealthMonitorRecordFields::AGENT_COLLECTION_TIME] =  time_now
        health_record[HealthMonitorRecordFields::TIME_FIRST_OBSERVED] =  time_now
        health_record[HealthMonitorRecordFields::CLUSTER_ID] = KubernetesApiClient.getClusterId
        records.push(health_record)
      end
      @@hmlog.info "Successfully processed pods_ready_percentage for #{config_monitor_id} #{records.size}"
      return records
    end

    def process_node_condition_monitor(node_inventory)
      hmlog = HealthMonitorUtils.getLogHandle
      monitor_id = HealthMonitorConstants::NODE_CONDITION_MONITOR_ID
      timestamp = Time.now.utc.iso8601
      monitor_config = @@healthMonitorConfig[monitor_id]
      node_condition_monitor_records = []
      if !node_inventory.nil?
          node_inventory['items'].each do |node|
            node_name = node['metadata']['name']
            conditions = node['status']['conditions']
            state = HealthMonitorUtils.getNodeStateFromNodeConditions(conditions)
            #hmlog.debug "Node Name = #{node_name} State = #{state}"
            details = {}
            conditions.each do |condition|
              details[condition['type']] = {"Reason" => condition['reason'], "Message" => condition['message']}
            end
            health_monitor_record = {"timestamp" => timestamp, "state" => state, "details" => details}
            monitor_instance_id = HealthMonitorUtils.getMonitorInstanceId(@@hmlog, monitor_id, [@@clusterId, node_name])
            HealthMonitorState.updateHealthMonitorState(@@hmlog, monitor_instance_id, health_monitor_record, monitor_config)
            #record = HealthMonitorSignalReducer.reduceSignal(@@hmlog, monitor_id, monitor_instance_id, monitor_config, node_name: node_name)
            health_record = {}
            time_now = Time.now.utc.iso8601
            health_record[HealthMonitorRecordFields::MONITOR_ID] = monitor_id
            health_record[HealthMonitorRecordFields::MONITOR_INSTANCE_ID] = monitor_instance_id
            health_record[HealthMonitorRecordFields::DETAILS] = health_monitor_record
            health_record[HealthMonitorRecordFields::AGENT_COLLECTION_TIME] =  time_now
            health_record[HealthMonitorRecordFields::TIME_FIRST_OBSERVED] =  time_now
            health_record[HealthMonitorRecordFields::CLUSTER_ID] = KubernetesApiClient.getClusterId
            health_record[HealthMonitorRecordFields::NODE_NAME] = node_name
            node_condition_monitor_records.push(health_record)
          end
      end
      @@hmlog.info "Successfully processed process_node_condition_monitor #{node_condition_monitor_records.size}"
      return node_condition_monitor_records
    end

    def run_periodic
      @mutex.lock
      done = @finished
      until done
        @condition.wait(@mutex, @run_interval)
        done = @finished
        @mutex.unlock
        if !done
          begin
            @@hmlog.info("in_kube_health::run_periodic @ #{Time.now.utc.iso8601}")
            enumerate
          rescue => errorStr
            @@hmlog.warn "in_kube_health::run_periodic: enumerate Failed for kubeapi sourced data health: #{errorStr}"
            ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
          end
        end
        @mutex.lock
      end
      @mutex.unlock
    end
  end
end

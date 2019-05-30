# Copyright (c) Microsoft Corporation.  All rights reserved.

# frozen_string_literal: true

module Fluent
    require 'logger'
    require 'json'
    Dir[File.join(__dir__, './health', '*.rb')].each { |file| require file }


    class FilterHealthModelBuilder < Filter
        Fluent::Plugin.register_filter('filter_health_model_builder', self)

        config_param :enable_log, :integer, :default => 0
        config_param :log_path, :string, :default => '/var/opt/microsoft/docker-cimprov/log/filter_health_model_builder.log'
        config_param :model_definition_path, :default => '/etc/opt/microsoft/docker-cimprov/health_model_definition.json'
        config_param :health_signal_timeout, :default => 240
        attr_reader :buffer, :model_builder, :health_model_definition, :monitor_factory, :state_transition_processor, :state_finalizers, :monitor_set, :model_builder
        include HealthModel

        @@healthMonitorConfig = HealthMonitorUtils.getHealthMonitorConfig
        @@rewrite_tag = 'oms.api.KubeHealth.AgentCollectionTime'

        def initialize
            super
            @buffer = HealthModel::HealthModelBuffer.new
            @health_model_definition = HealthModel::HealthModelDefinition.new(HealthModel::HealthModelDefinitionParser.new(@model_definition_path).parse_file)
            @monitor_factory = HealthModel::MonitorFactory.new
            @state_transition_processor = HealthModel::StateTransitionProcessor.new(@health_model_definition, @monitor_factory)
            @state_finalizers = [HealthModel::NodeMonitorHierarchyReducer.new, HealthModel::AggregateMonitorStateFinalizer.new]
            @monitor_set = HealthModel::MonitorSet.new
            @model_builder = HealthModel::HealthModelBuilder.new(@state_transition_processor, @state_finalizers, @monitor_set)
        end

        def configure(conf)
            super
            @log = nil

            if @enable_log
                @log = Logger.new(@log_path, 'weekly')
                @log.info 'Starting filter_health_model_builder plugin'
            end
        end

        def start
            super
        end

        def shutdown
            super
        end

        def filter_stream(tag, es)
            new_es = MultiEventStream.new
            time = Time.now
            begin
                if tag.start_with?("oms.api.KubeHealth.DaemonSet")
                    records = []
                    if !es.nil?
                        es.each{|time, record|
                            HealthMonitorState.updateHealthMonitorState(@log,
                                record[HealthMonitorRecordFields::MONITOR_INSTANCE_ID],
                                record[HealthMonitorRecordFields::DETAILS],
                                @@healthMonitorConfig[record[HealthMonitorRecordFields::MONITOR_ID]])
                            records.push(record)
                        }
                        @buffer.add_to_buffer(records)
                    end
                    return []
                elsif tag.start_with?("oms.api.KubeHealth.ReplicaSet")
                    records = []
                    es.each{|time, record|
                        records.push(record)
                    }
                    @buffer.add_to_buffer(records)
                    records_to_process = @buffer.get_buffer
                    @buffer.reset_buffer
                    filtered_records = []
                    raw_records = []
                    records_to_process.each{|record|
                        monitor_id = record[HealthMonitorRecordFields::MONITOR_ID]
                        filtered_record = HealthMonitorSignalReducer.reduceSignal(@log, monitor_id,
                            record[HealthMonitorRecordFields::MONITOR_INSTANCE_ID],
                            @@healthMonitorConfig[monitor_id],
                            @health_signal_timeout,
                            node_name: record[HealthMonitorRecordFields::NODE_NAME]
                            )
                            filtered_records.push(MonitorStateTransition.new(
                                filtered_record[HealthMonitorRecordFields::MONITOR_ID],
                                filtered_record[HealthMonitorRecordFields::MONITOR_INSTANCE_ID],
                                filtered_record[HealthMonitorRecordFields::TIME_FIRST_OBSERVED],
                                filtered_record[HealthMonitorRecordFields::OLD_STATE],
                                filtered_record[HealthMonitorRecordFields::NEW_STATE],
                                filtered_record[HealthMonitorRecordFields::MONITOR_LABELS],
                                filtered_record[HealthMonitorRecordFields::MONITOR_CONFIG],
                                filtered_record[HealthMonitorRecordFields::DETAILS]
                            )) if filtered_record

                            raw_records.push(filtered_record) if filtered_record
                    }

                    @log.info "Filtered Records size = #{filtered_records.size}"

                    # File.open("/tmp/mock_data-#{Time.now.to_i}.json", "w") do |f|
                    #     f.write(JSON.pretty_generate(raw_records))
                    # end

                    @model_builder.process_state_transitions(filtered_records)
                    monitors = @model_builder.finalize_model
                    @log.debug "monitors map size = #{monitors.size}"

                    monitors.map {|monitor_instance_id, monitor|
                        record = {}

                        record[HealthMonitorRecordFields::MONITOR_ID] = monitor.monitor_id
                        record[HealthMonitorRecordFields::MONITOR_INSTANCE_ID] = monitor.monitor_instance_id
                        record[HealthMonitorRecordFields::MONITOR_LABELS] = monitor.labels.to_json
                        record[HealthMonitorRecordFields::CLUSTER_ID] = KubernetesApiClient.getClusterId
                        record[HealthMonitorRecordFields::OLD_STATE] = monitor.old_state
                        record[HealthMonitorRecordFields::NEW_STATE] = monitor.new_state
                        record[HealthMonitorRecordFields::DETAILS] = monitor.details.to_json if monitor.methods.include? :details
                        record[HealthMonitorRecordFields::MONITOR_CONFIG] = monitor.config if monitor.methods.include? :config
                        record[HealthMonitorRecordFields::AGENT_COLLECTION_TIME] = Time.now.utc.iso8601
                        record[HealthMonitorRecordFields::TIME_FIRST_OBSERVED] = monitor.transition_time

                        new_es.add(time, record)
                    }

                    router.emit_stream(@@rewrite_tag, new_es)
                    # return an empty event stream, else the match will throw a NoMethodError
                    return []
                elsif tag.start_with?("oms.api.KubeHealth.AgentCollectionTime")
                    # this filter also acts as a pass through as we are rewriting the tag and emitting to the fluent stream
                    es
                else
                    raise 'Invalid tag #{tag} received'
                end

            rescue => e
                 @log.warn "Message: #{e.message} Backtrace: #{e.backtrace}"
                 return nil
            end
        end
    end
end

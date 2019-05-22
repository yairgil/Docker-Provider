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
        attr_reader :buffer, :model_builder, :health_model_definition, :monitor_factory, :state_transition_processor, :state_finalizers, :monitor_set, :model_builder

        @@healthMonitorConfig = HealthMonitorUtils.getHealthMonitorConfig

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
            begin
                if tag.start_with?("oms.api.KubeHealth.DaemonSet")
                    records = []
                    if !es.nil?
                        es.each{|time, record|
                            HealthMonitorState.updateHealthMonitorState(@log,
                                record[HealthMonitorRecordFields::MONITOR_INSTANCE_ID],
                                record[HealthMonitorRecordFields::DETAILS],
                                @@healthMonitorConfig[record[HealthMonitorRecordFields::MONITOR_INSTANCE_ID]])
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
                            key: record[HealthMonitorRecordFields::CONTAINER_ID],
                            controller_name: record[HealthMonitorRecordFields::CONTROLLER_NAME],
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


                    # if raw_records.size > 0

                    #     raw_records.each{|record|
                    #         @log.debug "#{record}"
                    #     }

                    #     File.open("/tmp/mock_data-#{Time.now.to_i}.json", "w") do |f|
                    #         f.write(JSON.pretty_generate(raw_records))
                    #     end
                    # end


                    @model_builder.process_state_transitions(filtered_records)
                    monitors_map = @model_builder.finalize_model
                    @log.debug "monitors map size = #{monitors_map.size}"
                    # monitors_map.each{|key, value|
                    #     @log.debug "#{key} ==> #{value.state}"
                    # }


                    return []
                else
                    raise "Invalid tag #{tag} received"
                end
            rescue => e
                 @log.warn "Message: #{e.message} Backtrace: #{e.backtrace}"
                 return nil
            end
            es
        end
    end
end

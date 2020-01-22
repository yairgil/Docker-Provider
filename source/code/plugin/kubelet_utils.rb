# Copyright (c) Microsoft Corporation.  All rights reserved.
#!/usr/local/bin/ruby
# frozen_string_literal: true

require_relative 'CAdvisorMetricsAPIClient'

class KubeletUtils
    class << self
        def get_node_capacity
            base_uri = CAdvisorMetricsAPIClient.getBaseCAdvisorUri(winNode: nil)
            relative_uri = "/spec/"

            cadvisor_uri = "#{base_uri}#{relative_uri}"
            $log.info("Using #{cadvisor_uri} for CAdvisor Uri in Kubelet Utils")
            uri = URI.parse(cadvisor_uri)
            cpu_capacity = 1.0
            memory_capacity = 1.0
            Net::HTTP.start(uri.host, uri.port, :use_ssl => false, :open_timeout => 20, :read_timeout => 40 ) do |http|
                cadvisor_api_request = Net::HTTP::Get.new(uri.request_uri)
                response = http.request(cadvisor_api_request)
                if !response.nil? && !response.body.nil?
                    cpu_capacity = JSON.parse(response.body)["num_cores"].nil? ? 1.0 : (JSON.parse(response.body)["num_cores"] * 1000.0)
                    memory_capacity = JSON.parse(response.body)["memory_capacity"].nil? ? 1.0 : JSON.parse(response.body)["memory_capacity"].to_f
                    $log.info "CPU = #{cpu_capacity}mc Memory = #{memory_capacity/1024/1024}MB"
                    return [cpu_capacity, memory_capacity]
                end
            end
        end
    end
end
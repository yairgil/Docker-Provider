#!/usr/local/bin/ruby
# frozen_string_literal: true


# details is an array of records
# include monitor config details in the template

require_relative 'HealthEventsConstants'

class HealthEventTemplates
    HealthRecordTemplate = '{
        "Labels": %{labels},
        "MonitorId": "%{monitor_id}",
        "MonitorInstanceId": "%{monitor_instance_id}",
        "NewState": "%{new_state}",
        "OldState": "%{old_state}",
        "Details": %{monitor_details},
        "MonitorConfig": %{monitor_config},
        "CollectionTime": "%{collection_time}",
        "TimeObserved": "%{time_observed}"
    }'

    DetailsNodeMemoryTemplate = '{
        "NodeMemoryRssPercentage": %{memory_rss_percentage},
        "NodeMemoryRssBytes": %{memory_rss_bytes},
        "History": [%{prev_records}]
    }'


    DetailsNodeCpuTemplate = '{
        "NodeCpuUsagePercentage": %{cpu_percentage},
        "NodeCpuUsageMilliCores": %{cpu_usage},
        "PrevNodeCpuUsageDetails": %{prev_monitor_record_details},
        "PrevPrevNodeCpuUsageDetails": %{prev_prev_monitor_record_details}
    }'

    DetailsWorkloadCpuOversubscribedTemplate = '{
        "ClusterCpuCapacity": %{cluster_cpu_capacity},
        "ClusterCpuRequests": %{cluster_cpu_requests}
    }'

    DetailsWorkloadMemoryOversubscribedTemplate = '{
        "ClusterMemoryCapacity": %{cluster_memory_capacity},
        "ClusterMemoryRequests": %{cluster_memory_requests}
    }'

    DetailsWorkloadPodsReadyStatePercentage = '{
        "TimeStamp": "%{timestamp}",
        "PodsReady": %{pods_ready},
        "TotalPods": %{total_pods}
        "History": [%{prev_records}]
    }'

    DetailsWorkloadContainerCpuPercentage = '
        "TimeStamp": "%{timestamp}",
        "CpuLimit": %{cpu_limit},
        "CpuRequest": %{cpu_request},
        "CpuPercentage": %{cpu_percentage},
        "History": [%{prev_records}]
    }'

    DetailsWorkloadContainerMemoryPercentage = '
        "TimeStamp": "%{timestamp}",
        "MemoryLimit": %{memory_limit},
        "MemoryRequest": %{memory_request},
        "MemoryPercentage": %{memory_percentage},
        "History": [%{prev_records}]
    }'

    DETAILS_TEMPLATE_HASH = {
        HealthEventsConstants::NODE_CPU_MONITOR_ID => DetailsNodeCpuTemplate,
        HealthEventsConstants::NODE_MEMORY_MONITOR_ID => DetailsNodeMemoryTemplate,
        HealthEventsConstants::WORKLOAD_CONTAINER_CPU_PERCENTAGE_MONITOR_ID => DetailsWorkloadContainerCpuPercentage,
        HealthEventsConstants::WORKLOAD_CONTAINER_MEMORY_PERCENTAGE_MONITOR_ID => DetailsWorkloadContainerMemoryPercentage,
        HealthEventsConstants::WORKLOAD_CPU_OVERSUBSCRIBED_MONITOR_ID => DetailsWorkloadCpuOversubscribedTemplate,
        HealthEventsConstants::WORKLOAD_MEMORY_OVERSUBSCRIBED_MONITOR_ID => DetailsWorkloadMemoryOversubscribedTemplate,
        HealthEventsConstants::WORKLOAD_PODS_READY_PERCENTAGE_MONITOR_ID => DetailsWorkloadPodsReadyStatePercentage,
    }
end
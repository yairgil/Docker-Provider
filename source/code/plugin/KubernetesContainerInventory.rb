#!/usr/local/bin/ruby
# frozen_string_literal: true

class KubernetesContainerInventory
    require "yajl/json_gem"
    require "time"
    require_relative "omslog"
    # cache the container and cgroup parent process
    @@containerCGroupCache = Hash.new
  
    def initialize
    end
  
    class << self
      def getContainerInventoryRecords(podItem, batchTime, clusterCollectEnvironmentVar)
        containerInventoryRecords = Array.new
        begin
          containersInfoMap = getContainersInfoMap(podItem)
          podContainersStatuses = []
          if !podItem["status"]["containerStatuses"].nil? && !podItem["status"]["containerStatuses"].empty?
            podContainersStatuses = podItem["status"]["containerStatuses"]
          end
          if !podItem["status"]["initContainerStatuses"].nil? && !podItem["status"]["initContainerStatuses"].empty?
            podContainersStatuses = podContainersStatuses + podItem["status"]["initContainerStatuses"]
          end
          containerInventoryRecord = {}
          if !podContainersStatuses.empty?
            podContainersStatuses.each do |containerStatus|
              containerInventoryRecord["CollectionTime"] = batchTime #This is the time that is mapped to become TimeGenerated
              containerName = containerStatus["name"]
              # containeId format is <containerRuntime>://<containerId>
              containerRuntime = ""
              containerId = ""
              if !containerStatus["containerID"].nil?
                containerRuntime = containerStatus["containerID"].split(":")[0]
                containerId = containerStatus["containerID"].split("//")[1]
                containerInventoryRecord["InstanceID"] = containerId
              else
                # for containers that have image issues (like invalid image/tag etc..) this will be empty. do not make it all 0
                containerInventoryRecord["InstanceID"] = containerId
              end
              # imagedId is of the format - repo@sha256:imageid
              imageIdValue = containerStatus["imageID"]
              if !imageIdValue.nil? && !imageIdValue.empty?
                atLocation = imageIdValue.index("@")
                if !atLocation.nil?
                  containerInventoryRecord["ImageId"] = imageIdValue[(atLocation + 1)..-1]
                end
              end
              # image is of the format - repository/image:imagetag
              imageValue = containerStatus["image"]
              if !imageValue.nil? && !imageValue.empty?
                # Find delimiters in the string of format repository/image:imagetag
                slashLocation = imageValue.index("/")
                colonLocation = imageValue.index(":")
                if !colonLocation.nil?
                  if slashLocation.nil?
                    # image:imagetag
                    containerInventoryRecord["Image"] = imageValue[0..(colonLocation - 1)]
                  else
                    # repository/image:imagetag
                    containerInventoryRecord["Repository"] = imageValue[0..(slashLocation - 1)]
                    containerInventoryRecord["Image"] = imageValue[(slashLocation + 1)..(colonLocation - 1)]
                  end
                  containerInventoryRecord["ImageTag"] = imageValue[(colonLocation + 1)..-1]
                end
              elsif !imageIdValue.nil? && !imageIdValue.empty?
                # Getting repo information from imageIdValue when no tag in ImageId
                if !atLocation.nil?
                  containerInventoryRecord["Repository"] = imageIdValue[0..(atLocation - 1)]
                end
              end
              containerInventoryRecord["ExitCode"] = 0
              if !containerStatus["state"].nil? && !containerStatus["state"].empty?
                containerState = containerStatus["state"]
                if containerState.key?("running")
                  containerInventoryRecord["State"] = "Running"
                  containerInventoryRecord["StartedTime"] = containerState["running"]["startedAt"]
                elsif containerState.key?("terminated")
                  containerInventoryRecord["State"] = "Terminated"
                  containerInventoryRecord["StartedTime"] = containerState["terminated"]["startedAt"]
                  containerInventoryRecord["FinishedTime"] = containerState["terminated"]["finishedAt"]
                  containerInventoryRecord["ExitCode"] = containerState["terminated"]["exitCode"]
                elsif containerState.key?("waiting")
                  containerInventoryRecord["State"] = "Waiting"
                end
              end
              containerInfoMap = containersInfoMap[containerName]
              containerInventoryRecord["ElementName"] = containerInfoMap["ElementName"]
              containerInventoryRecord["Computer"] = containerInfoMap["Computer"]
              containerInventoryRecord["ContainerHostname"] = containerInfoMap["ContainerHostname"]
              containerInventoryRecord["CreatedTime"] = containerInfoMap["CreatedTime"]
              containerInventoryRecord["EnvironmentVar"] = containerInfoMap["EnvironmentVar"]
              containerInventoryRecord["Ports"] = containerInfoMap["Ports"]
              containerInventoryRecord["Command"] = containerInfoMap["Command"]
              if !clusterCollectEnvironmentVar.nil? && !clusterCollectEnvironmentVar.empty? && clusterCollectEnvironmentVar.casecmp("false") == 0
                containerInventoryRecord["EnvironmentVar"] = ["AZMON_CLUSTER_COLLECT_ENV_VAR=FALSE"]
              else
                if containerId.nil? || containerId.empty? || containerRuntime.nil? || containerRuntime.empty?
                  containerInventoryRecord["EnvironmentVar"] = ""
                elsif containerRuntime.casecmp("cri-o") == 0
                  # crio containers have conmon as parent process and we only to need get container main process envvars
                  containerInventoryRecord["EnvironmentVar"] = obtainContainerEnvironmentVars("crio-#{containerId}")
                else
                  containerInventoryRecord["EnvironmentVar"] = obtainContainerEnvironmentVars(containerId)
                end
              end
              containerInventoryRecords.push containerInventoryRecord
            end
          end
        rescue => error
          @log.warn("in_container_inventory::getContainerInventoryRecords : Get Container Inventory Records failed: #{error}")
        end
        return containerInventoryRecords
      end
  
      def getContainersInfoMap(podItem)
        containersInfoMap = {}
        begin
          nodeName = (!podItem["spec"]["nodeName"].nil?) ? podItem["spec"]["nodeName"] : ""
          createdTime = podItem["metadata"]["creationTimestamp"]
          if !podItem.nil? && !podItem.empty? && podItem.key?("spec") && !podItem["spec"].nil? && !podItem["spec"].empty?
            podContainers = []
            if !podItem["spec"]["containers"].nil? && !podItem["spec"]["containers"].empty?
              podContainers = podItem["spec"]["containers"]
            end
            if !podItem["spec"]["initContainers"].nil? && !podItem["spec"]["initContainers"].empty?
              podContainers = podContainers + podItem["spec"]["initContainers"]
            end
            if !podContainers.empty?
              podContainers.each do |container|
                containerInfoMap = {}
                containerName = container["name"]
                containerInfoMap["ElementName"] = containerName
                containerInfoMap["Computer"] = nodeName
                containerInfoMap["ContainerHostname"] = nodeName
                containerInfoMap["CreatedTime"] = createdTime
                portsValue = container["ports"]
                portsValueString = (portsValue.nil?) ? "" : portsValue.to_s
                containerInfoMap["Ports"] = portsValueString
                cmdValue = container["command"]
                cmdValueString = (cmdValue.nil?) ? "" : cmdValue.to_s
                containerInfoMap["Command"] = cmdValueString
  
                containersInfoMap[containerName] = containerInfoMap
              end
            end
          end
        rescue => error
          @log.warn("in_container_inventory::getContainersInfoMap : Get Container Info Maps failed: #{error}")
        end
        return containersInfoMap
      end
  
      def obtainContainerEnvironmentVars(containerId)
        $log.info("in_container_inventory::obtainContainerEnvironmentVars @ #{Time.now.utc.iso8601}")
        envValueString = ""
        begin
          unless @@containerCGroupCache.has_key?(containerId)
            $log.info("in_container_inventory::fetching cGroup parent pid @ #{Time.now.utc.iso8601}")
            Dir["/hostfs/proc/*/cgroup"].each do |filename|
              if File.file?(filename) && File.foreach(filename).grep(/#{containerId}/).any?
                # file full path is /hostfs/proc/<cGroupPid>/cgroup
                $log.info("in_container_inventory::fetching cGroup parent  filename @ #{filename}")
                cGroupPid = filename.split("/")[3]
                if @@containerCGroupCache.has_key?(containerId)
                  tempCGroupPid = @@containerCGroupCache[containerId]
                  if tempCGroupPid > cGroupPid
                    @@containerCGroupCache[containerId] = cGroupPid
                  end
                else
                  @@containerCGroupCache[containerId] = cGroupPid
                end
              end
            end
          end
          cGroupPid = @@containerCGroupCache[containerId]
          if !cGroupPid.nil?
            environFilePath = "/hostfs/proc/#{cGroupPid}/environ"
            if File.exist?(environFilePath)
              # Skip environment variable processing if it contains the flag AZMON_COLLECT_ENV=FALSE
              # Check to see if the environment variable collection is disabled for this container.
              if File.foreach(environFilePath).grep(/AZMON_COLLECT_ENV=FALSE/i).any?
                envValueString = ["AZMON_COLLECT_ENV=FALSE"]
                $log.warn("Environment Variable collection for container: #{containerId} skipped because AZMON_COLLECT_ENV is set to false")
              else
                # Restricting the ENV string value to 200kb since the size of this string can go very high
                envVars = File.read(environFilePath, 200000)
                if !envVars.nil? && !envVars.empty?
                  envVars = envVars.split("\0")
                  envValueString = envVars.to_s
                  envValueStringLength = envValueString.length
                  $log.info("in_container_inventory::environment vars filename @ #{environFilePath} envVars size @ #{envValueStringLength}")
                  if envValueStringLength >= 200000
                    lastIndex = envValueString.rindex("\", ")
                    if !lastIndex.nil?
                      envValueStringTruncated = envValueString.slice(0..lastIndex) + "]"
                      envValueString = envValueStringTruncated
                    end
                  end
                end
              end
            end
          end
        rescue => error
          @log.warn("in_container_inventory::obtainContainerEnvironmentVars : obtain Container Environment vars failed: #{error} for containerId: #{containerId}")
        end
        return envValueString
      end
    end
  end
    
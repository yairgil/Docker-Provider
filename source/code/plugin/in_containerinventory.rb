#!/usr/local/bin/ruby
# frozen_string_literal: true

module Fluent
  class Container_Inventory_Input < Input
    Plugin.register_input("containerinventory", self)

    @@PluginName = "ContainerInventory"
    @@RunningState = "Running"
    @@FailedState = "Failed"
    @@StoppedState = "Stopped"
    @@PausedState = "Paused"

    def initialize
      super
      require 'yajl/json_gem'
      require "time"
      require_relative "DockerApiClient"
      require_relative "ContainerInventoryState"
      require_relative "ApplicationInsightsUtility"
      require_relative "omslog"
      require_relative 'CAdvisorMetricsAPIClient'
    end

    config_param :run_interval, :time, :default => 60
    config_param :tag, :string, :default => "oms.containerinsights.containerinventory"

    def configure(conf)
      super
    end

    def start
      if @run_interval
        @finished = false
        @condition = ConditionVariable.new
        @mutex = Mutex.new
        @thread = Thread.new(&method(:run_periodic))
        @@telemetryTimeTracker = DateTime.now.to_time.to_i
        # cache the container and cgroup parent process
        @containerCGroupCache = Hash.new
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

    def obtainContainerConfig(instance, container, clusterCollectEnvironmentVar)
      begin
        configValue = container["Config"]
        if !configValue.nil?
          instance["ContainerHostname"] = configValue["Hostname"]
          # Check to see if the environment variable collection is disabled at the cluster level - This disables env variable collection for all containers.
          if !clusterCollectEnvironmentVar.nil? && !clusterCollectEnvironmentVar.empty? && clusterCollectEnvironmentVar.casecmp("false") == 0
            instance["EnvironmentVar"] = ["AZMON_CLUSTER_COLLECT_ENV_VAR=FALSE"]
          else
            envValue = configValue["Env"]
            envValueString = (envValue.nil?) ? "" : envValue.to_s
            # Skip environment variable processing if it contains the flag AZMON_COLLECT_ENV=FALSE
            # Check to see if the environment variable collection is disabled for this container.
            if /AZMON_COLLECT_ENV=FALSE/i.match(envValueString)
              envValueString = ["AZMON_COLLECT_ENV=FALSE"]
              $log.warn("Environment Variable collection for container: #{container["Id"]} skipped because AZMON_COLLECT_ENV is set to false")
            end
            # Restricting the ENV string value to 200kb since the size of this string can go very high
            if envValueString.length > 200000
              envValueStringTruncated = envValueString.slice(0..200000)
              lastIndex = envValueStringTruncated.rindex("\", ")
              if !lastIndex.nil?
                envValueStringTruncated = envValueStringTruncated.slice(0..lastIndex) + "]"
              end
              instance["EnvironmentVar"] = envValueStringTruncated
            else
              instance["EnvironmentVar"] = envValueString
            end
          end

          cmdValue = configValue["Cmd"]
          cmdValueString = (cmdValue.nil?) ? "" : cmdValue.to_s
          instance["Command"] = cmdValueString

          instance["ComposeGroup"] = ""
          labelsValue = configValue["Labels"]
          if !labelsValue.nil? && !labelsValue.empty?
            instance["ComposeGroup"] = labelsValue["com.docker.compose.project"]
          end
        else
          $log.warn("Attempt in ObtainContainerConfig to get container: #{container["Id"]} config information returned null")
        end
      rescue => errorStr
        $log.warn("Exception in obtainContainerConfig: #{errorStr}")
      end
    end

    def obtainContainerState(instance, container)
      begin
        stateValue = container["State"]
        if !stateValue.nil?
          exitCodeValue = stateValue["ExitCode"]
          # Exit codes less than 0 are not supported by the engine
          if exitCodeValue < 0
            exitCodeValue = 128
            $log.info("obtainContainerState::Container: #{container["Id"]} returned negative exit code")
          end
          instance["ExitCode"] = exitCodeValue
          if exitCodeValue > 0
            instance["State"] = @@FailedState
          else
            # Set the Container status : Running/Paused/Stopped
            runningValue = stateValue["Running"]
            if runningValue
              pausedValue = stateValue["Paused"]
              # Checking for paused within running is true state because docker returns true for both Running and Paused fields when the container is paused
              if pausedValue
                instance["State"] = @@PausedState
              else
                instance["State"] = @@RunningState
              end
            else
              instance["State"] = @@StoppedState
            end
          end
          instance["StartedTime"] = stateValue["StartedAt"]
          instance["FinishedTime"] = stateValue["FinishedAt"]
        else
          $log.info("Attempt in ObtainContainerState to get container: #{container["Id"]} state information returned null")
        end
      rescue => errorStr
        $log.warn("Exception in obtainContainerState: #{errorStr}")
      end
    end

    def obtainContainerHostConfig(instance, container)
      begin
        hostConfig = container["HostConfig"]
        if !hostConfig.nil?
          links = hostConfig["Links"]
          instance["Links"] = ""
          if !links.nil?
            linksString = links.to_s
            instance["Links"] = (linksString == "null") ? "" : linksString
          end
          portBindings = hostConfig["PortBindings"]
          instance["Ports"] = ""
          if !portBindings.nil?
            portBindingsString = portBindings.to_s
            instance["Ports"] = (portBindingsString == "null") ? "" : portBindingsString
          end
        else
          $log.info("Attempt in ObtainContainerHostConfig to get container: #{container["Id"]} host config information returned null")
        end
      rescue => errorStr
        $log.warn("Exception in obtainContainerHostConfig: #{errorStr}")
      end
    end

    def inspectContainer(id, nameMap, clusterCollectEnvironmentVar)
      containerInstance = {}
      begin
        container = DockerApiClient.dockerInspectContainer(id)
        if !container.nil? && !container.empty?
          containerInstance["InstanceID"] = container["Id"]
          containerInstance["CreatedTime"] = container["Created"]
          containerName = container["Name"]
          if !containerName.nil? && !containerName.empty?
            # Remove the leading / from the name if it exists (this is an API issue)
            containerInstance["ElementName"] = (containerName[0] == "/") ? containerName[1..-1] : containerName
          end
          imageValue = container["Image"]
          if !imageValue.nil? && !imageValue.empty?
            repoImageTagArray = nameMap[imageValue]
            if nameMap.has_key? imageValue
              containerInstance["Repository"] = repoImageTagArray[0]
              containerInstance["Image"] = repoImageTagArray[1]
              containerInstance["ImageTag"] = repoImageTagArray[2]
              # Setting the image id to the id in the remote repository
              containerInstance["ImageId"] = repoImageTagArray[3]
            end
          end
          obtainContainerConfig(containerInstance, container, clusterCollectEnvironmentVar)
          obtainContainerState(containerInstance, container)
          obtainContainerHostConfig(containerInstance, container)
        end
      rescue => errorStr
        $log.warn("Exception in inspectContainer: #{errorStr} for container: #{id}")
      end
      return containerInstance
    end

    def getContainerInventoryRecords(batchTime, clusterCollectEnvironmentVar)
      containerInventoryRecords = Array.new
      begin
          response = CAdvisorMetricsAPIClient.getPodsFromCAdvisor(winNode: nil)
          if !response.nil? && !response.body.nil?
              podList = JSON.parse(response.body)
              if !podList.nil? && !podList.empty? && podList.key?("items") && !podList["items"].nil? && !podList["items"].empty?
                  podList["items"].each do |item|
                      containersInfoMap = getContainersInfoMap(item)
                      containerInventoryRecord = {}
                      if !item["status"].nil? && !item["status"].empty?
                          if !item["status"]["containerStatuses"].nil? && !item["status"]["containerStatuses"].empty?
                              item["status"]["containerStatuses"].each do |containerStatus|
                               containerInventoryRecord["CollectionTime"] = batchTime #This is the time that is mapped to become TimeGenerated
                               containerName = containerStatus["name"]
                               containerId = containerStatus["containerID"].split('//')[1]
                               containerInventoryRecord["InstanceID"] = containerId
                               # imagedId is of the format - repo@sha256:imageid
                               imageIdValue =  containerStatus["imageID"]
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
                                  containerInventoryRecord["EnvironmentVar"]  = obtainContainerEnvironmentVars(containerId)
                               end
                               containerInventoryRecords.push containerInventoryRecord
                              end
                          end
                      end
                  end
              end
          end
      rescue => error
          @log.warn("in_container_inventory::getContainerInventoryRecords : Get Container Inventory Records failed: #{error}")
      end
      return containerInventoryRecords
  end

  def getContainersInfoMap(item)
      containersInfoMap = {}
      begin
          nodeName = (!item["spec"]["nodeName"].nil?) ? item["spec"]["nodeName"] : ""
          createdTime = item["metadata"]["creationTimestamp"]
          if !item.nil? && !item.empty? && item.key?("spec") && !item["spec"].nil? && !item["spec"].empty?
              if !item["spec"]["containers"].nil? && !item["spec"]["containers"].empty?
                  item["spec"]["containers"].each do |container|
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
          unless @containerCGroupCache.has_key?(containerId)
              $log.info("in_container_inventory::fetching cGroup parent pid @ #{Time.now.utc.iso8601}")
              Dir["/hostfs/proc/*/cgroup"].each do| filename|
                  if  File.file?(filename) && File.foreach(filename).grep(/#{containerId}/).any?
                      # file full path is /hostfs/proc/<cGroupPid>/cgroup
                      $log.info("in_container_inventory::fetching cGroup parent  filename @ #{filename}")
                      cGroupPid = filename.split("/")[3]
                      if @containerCGroupCache.has_key?(containerId)
                          tempCGroupPid = containerCgroupCache[containerId]
                          if tempCGroupPid > cGroupPid
                             @containerCgroupCache[containerId] = cGroupPid
                          end
                      else
                          @containerCGroupCache[containerId] = cGroupPid
                      end
                  end
              end
          end
          cGroupPid = @containerCGroupCache[containerId]
          if !cGroupPid.nil?
              environFilePath = "/hostfs/proc/#{cGroupPid}/environ"
              if File.exist?(environFilePath)
                  # Skip environment variable processing if it contains the flag AZMON_COLLECT_ENV=FALSE
                  # Check to see if the environment variable collection is disabled for this container.
                  if File.foreach(environFilePath).grep(/AZMON_COLLECT_ENV=FALSE/i).any?
                      envValueString = ["AZMON_COLLECT_ENV=FALSE"]
                      $log.warn("Environment Variable collection for container: #{containerName} skipped because AZMON_COLLECT_ENV is set to false")
                  else
                      fileSize = File.size(environFilePath)
                      $log.info("in_container_inventory::environment vars filename @ #{filename} filesize @ #{fileSize}")
                      # Restricting the ENV string value to 200kb since the size of this string can go very high
                      envVars = File.read(environFilePath, 200000).split(" ")
                      envValueString = envVars.to_s
                      if fileSize > 200000
                          lastIndex = envValueString.rindex("\", ")
                          if !lastIndex.nil?
                             envValueStringTruncated = envValueString.slice(0..lastIndex) + "]"
                             envValueString = envValueStringTruncated
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

  def enumerate
      currentTime = Time.now
      emitTime = currentTime.to_f
      batchTime = currentTime.utc.iso8601
      containerInventory = Array.new
      eventStream = MultiEventStream.new
      hostName = ""
      $log.info("in_container_inventory::enumerate : Begin processing @ #{Time.now.utc.iso8601}")
      begin
        containerRuntimeEnv = ENV["CONTAINER_RUN_TIME"]
        $log.info("in_container_inventory::enumerate : container runtime : #{containerRuntimeEnv}")
        clusterCollectEnvironmentVar = ENV["AZMON_CLUSTER_COLLECT_ENV_VAR"]
        if !containerRuntimeEnv.nil? && !containerRuntimeEnv.empty? && containerRuntimeEnv.casecmp("docker") == 0
            $log.info("in_container_inventory::enumerate : using docker apis since container runtime is docker")
            hostName = DockerApiClient.getDockerHostName
            containerIds = DockerApiClient.listContainers
            if !containerIds.nil? && !containerIds.empty?
              nameMap = DockerApiClient.getImageIdMap
              if !clusterCollectEnvironmentVar.nil? && !clusterCollectEnvironmentVar.empty? && clusterCollectEnvironmentVar.casecmp("false") == 0
                $log.warn("Environment Variable collection disabled for cluster")
              end
              containerIds.each do |containerId|
                inspectedContainer = {}
                inspectedContainer = inspectContainer(containerId, nameMap, clusterCollectEnvironmentVar)
                inspectedContainer["Computer"] = hostName
                inspectedContainer["CollectionTime"] = batchTime #This is the time that is mapped to become TimeGenerated
                containerInventory.push inspectedContainer
                ContainerInventoryState.writeContainerState(inspectedContainer)
              end
              # Update the state for deleted containers
              deletedContainers = ContainerInventoryState.getDeletedContainers(containerIds)
              if !deletedContainers.nil? && !deletedContainers.empty?
                deletedContainers.each do |deletedContainer|
                  container = ContainerInventoryState.readContainerState(deletedContainer)
                  if !container.nil?
                    container.each { |k, v| container[k] = v }
                    container["State"] = "Deleted"
                    containerInventory.push container
                  end
                end
              end
            end
        else
            $log.info("in_container_inventory::enumerate : using cadvisor apis since non docker container runtime : #{containerRuntimeEnv}")
            containerInventoryRecords = getContainerInventoryRecords(batchTime, clusterCollectEnvironmentVar)
            containerIds = Array.new
            containerInventoryRecords.each do |containerRecord|
              ContainerInventoryState.writeContainerState(containerRecord)
              if hostName.empty? && !containerRecord["Computer"].empty?
                 hostName = containerRecord["Computer"]
              end
              containerIds.push containerRecord["InstanceID"]
              containerInventory.push containerRecord
            end
            # Update the state for deleted containers
            deletedContainers = ContainerInventoryState.getDeletedContainers(containerIds)
            if !deletedContainers.nil? && !deletedContainers.empty?
                deletedContainers.each do |deletedContainer|
                  container = ContainerInventoryState.readContainerState(deletedContainer)
                    if !container.nil?
                      container.each { |k, v| container[k] = v }
                      container["State"] = "Deleted"
                      @containerCGroupCache.delete(container["InstanceID"])
                      containerInventory.push container
                    end
                end
            end
        end

        containerInventory.each do |record|
          wrapper = {
            "DataType" => "CONTAINER_INVENTORY_BLOB",
            "IPName" => "ContainerInsights",
            "DataItems" => [record.each { |k, v| record[k] = v }],
          }
          eventStream.add(emitTime, wrapper) if wrapper
        end
        router.emit_stream(@tag, eventStream) if eventStream
        @@istestvar = ENV["ISTEST"]
        if (!@@istestvar.nil? && !@@istestvar.empty? && @@istestvar.casecmp("true") == 0 && eventStream.count > 0)
          $log.info("containerInventoryEmitStreamSuccess @ #{Time.now.utc.iso8601}")
        end
        $log.info("in_container_inventory::enumerate : Processing complete - emitted stream @ #{Time.now.utc.iso8601}")
        timeDifference = (DateTime.now.to_time.to_i - @@telemetryTimeTracker).abs
        timeDifferenceInMinutes = timeDifference / 60
        if (timeDifferenceInMinutes >= 5)
          @@telemetryTimeTracker = DateTime.now.to_time.to_i
          telemetryProperties = {}
          telemetryProperties["Computer"] = hostName
          telemetryProperties["ContainerCount"] = containerInventory.length
          ApplicationInsightsUtility.sendTelemetry(@@PluginName, telemetryProperties)
        end
      rescue => errorStr
        $log.warn("Exception in enumerate container inventory: #{errorStr}")
        ApplicationInsightsUtility.sendExceptionTelemetry(errorStr)
      end
    end

    def run_periodic
      @mutex.lock
      done = @finished
      @nextTimeToRun = Time.now
      @waitTimeout = @run_interval
      until done
        @nextTimeToRun = @nextTimeToRun + @run_interval
        @now = Time.now
        if @nextTimeToRun <= @now
          @waitTimeout = 1
          @nextTimeToRun = @now
        else
          @waitTimeout = @nextTimeToRun - @now
        end
        @condition.wait(@mutex, @waitTimeout)
        done = @finished
        @mutex.unlock
        if !done
          begin
            $log.info("in_container_inventory::run_periodic.enumerate.start @ #{Time.now.utc.iso8601}")
            enumerate
            $log.info("in_container_inventory::run_periodic.enumerate.end @ #{Time.now.utc.iso8601}")
          rescue => errorStr
            $log.warn "in_container_inventory::run_periodic: Failed in enumerate container inventory: #{errorStr}"
          end
        end
        @mutex.lock
      end
      @mutex.unlock
    end
  end # Container_Inventory_Input
end # module

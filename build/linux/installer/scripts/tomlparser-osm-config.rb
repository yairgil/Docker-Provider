#!/usr/local/bin/ruby

#this should be require relative in Linux and require in windows, since it is a gem install on windows
@os_type = ENV["OS_TYPE"]
if !@os_type.nil? && !@os_type.empty? && @os_type.strip.casecmp("windows") == 0
  require "tomlrb"
else
  require_relative "tomlrb"
end

require_relative "ConfigParseErrorLogger"

@configMapMountPath = "/etc/config/osm-settings"
@configSchemaVersion = ""
@tgfConfigFileSidecar = "/etc/opt/microsoft/docker-cimprov/telegraf-prom-side-car.conf"
@osmMetricNamespaces = []

#Configurations to be used for the auto-generated input prometheus plugins for namespace filtering
@metricVersion = 2
@monitorKubernetesPodsVersion = 2
@fieldPassSetting = "[\"envoy_cluster_upstream_rq_xx\", \"envoy_cluster_upstream_rq\"]"
@urlTag = "scrapeUrl"
@bearerToken = "/var/run/secrets/kubernetes.io/serviceaccount/token"
@responseTimeout = "15s"
@tlsCa = "/var/run/secrets/kubernetes.io/serviceaccount/ca.crt"
@insecureSkipVerify = true

# Use parser to parse the configmap toml file to a ruby structure
def parseConfigMap
  begin
    # Check to see if config map is created
    if (File.file?(@configMapMountPath))
      puts "config::configmap container-azm-ms-osmconfig for osm metrics found, parsing values"
      parsedConfig = Tomlrb.load_file(@configMapMountPath, symbolize_keys: true)
      puts "config::Successfully parsed mounted config map for osm metrics"
      return parsedConfig
    else
      puts "config::configmap container-azm-ms-osmconfig for osm metrics not mounted, using defaults"
      return nil
    end
  rescue => errorStr
    ConfigParseErrorLogger.logError("Exception while parsing config map for osm metrics: #{errorStr}, using defaults, please check config map for errors")
    return nil
  end
end

def checkForTypeArray(arrayValue, arrayType)
  if (arrayValue.nil? || (arrayValue.kind_of?(Array) && ((arrayValue.length == 0) || (arrayValue.length > 0 && arrayValue[0].kind_of?(arrayType)))))
    return true
  else
    return false
  end
end

# Use the ruby structure created after config parsing to set the right values to be used as environment variables
def populateSettingValuesFromConfigMap(parsedConfig)
  begin
    if !parsedConfig.nil? &&
       !parsedConfig[:osm_metric_collection_configuration].nil? &&
       !parsedConfig[:osm_metric_collection_configuration][:settings].nil?
      osmPromMetricNamespaces = parsedConfig[:osm_metric_collection_configuration][:settings][:monitor_namespaces]
      puts "config::osm::got:osm_metric_collection_configuration.settings.monitor_namespaces='#{osmPromMetricNamespaces}'"

      # Check to see if osm_metric_collection_configuration.settings has a valid setting for monitor_namespaces to enable scraping for specific namespaces
      # Adding nil check here as well since checkForTypeArray returns true even if setting is nil to accomodate for other settings to be able -
      # - to use defaults in case of nil settings
      if !osmPromMetricNamespaces.nil? && checkForTypeArray(osmPromMetricNamespaces, String)
        # Adding a check to see if an empty array is passed for kubernetes namespaces
        if (osmPromMetricNamespaces.length > 0)
          @osmMetricNamespaces = osmPromMetricNamespaces
        end
      end
    end
  rescue => errorStr
    puts "config::osm::error:Exception while reading config settings for osm configuration settings - #{errorStr}, using defaults"
    @osmMetricNamespaces = []
  end
end

@osmConfigSchemaVersion = ENV["AZMON_OSM_CFG_SCHEMA_VERSION"]
puts "****************Start OSM Config Processing********************"
if !@osmConfigSchemaVersion.nil? && !@osmConfigSchemaVersion.empty? && @osmConfigSchemaVersion.strip.casecmp("v1") == 0 #note v1 is the only supported schema version , so hardcoding it
  configMapSettings = parseConfigMap
  if !configMapSettings.nil?
    populateSettingValuesFromConfigMap(configMapSettings)
  end
else
  if (File.file?(@configMapMountPath))
    ConfigParseErrorLogger.logError("config::osm::unsupported/missing config schema version - '#{@osmConfigSchemaVersion}' , using defaults, please use supported schema version")
  end
end

#replace place holders in configuration file
tgfConfig = File.read(@tgfConfigFileSidecar) #read returns only after closing the file

if @osmMetricNamespaces.length > 0
  osmPluginConfigsWithNamespaces = ""
  @osmMetricNamespaces.each do |namespace|
    if !namespace.nil?
      #Stripping namespaces to remove leading and trailing whitespaces
      namespace.strip!
      if namespace.length > 0
        osmPluginConfigsWithNamespaces += "\n[[inputs.prometheus]]
monitor_kubernetes_pods = true
monitor_kubernetes_pods_version = #{@monitorKubernetesPodsVersion}
monitor_kubernetes_pods_namespace = \"#{namespace}\"
fieldpass = #{@fieldPassSetting}
metric_version = #{@metricVersion}
url_tag = \"#{@urlTag}\"
bearer_token = \"#{@bearerToken}\"
response_timeout = \"#{@responseTimeout}\"
tls_ca = \"#{@tlsCa}\"
insecure_skip_verify = #{@insecureSkipVerify}\n"
      end
    end
  end
  tgfConfig = tgfConfig.gsub("$AZMON_SIDECAR_OSM_PROM_PLUGINS", @osmPluginConfigsWithNamespaces)
else
  puts "Using defaults for OSM configuration since there was an error in OSM config map or no namespaces were set"
  tgfConfig = tgfConfig.gsub("$AZMON_SIDECAR_OSM_PROM_PLUGINS", "")
end

File.open(@tgfConfigFileSidecar, "w") { |file| file.puts tgfConfig } # 'file' will be closed here after it goes out of scope
puts "config::osm::Successfully substituted the OSM placeholders in #{@tgfConfigFileSidecar} file in sidecar container"

# Write the telemetry to file, so that they can be set as environment variables
telemetryFile = File.open("integration_osm_config_env_var", "w")

if !telemetryFile.nil?
  telemetryFile.write("export TELEMETRY_OSM_CONFIGURATION_NAMESPACES_COUNT=#{@osmMetricNamespaces.length}\n")
  # Close file after writing all environment variables
  telemetryFile.close
else
  puts "config::npm::Exception while opening file for writing OSM telemetry environment variables"
  puts "****************End OSM Config Processing********************"
end

#!/usr/local/bin/ruby
# frozen_string_literal: true

class LogRotationCounter
  require "logger"
  require_relative "ApplicationInsightsUtility"

  @log_path = "/var/opt/microsoft/docker-cimprov/log/logRotationCounter.log"
  $log = Logger.new(@log_path, 1, 5000000)

  while true
    cache = {}
    lines = File.readlines('/opt/file-rotation')
    lines.each do |line|
      matches = line.match(/(\S*\/0.log) : (\S*) : (\S*)/)
      if matches != nil
        filename, event, time = matches.captures
        puts filename
        if cache.key?(filename) && event == "CREATE"
          cache[filename] += 1
        elsif event == "CREATE"
          cache[filename] = 1
        end
      end
    end

    File.truncate('/opt/file-rotation', 0)

    key_value = cache.max_by{|k,v| v}
    telemetryProps = {}

    if !key_value.nil? && key_value.length == 2
      ApplicationInsightsUtility.sendMetricTelemetry("maxRotationsForFileTailed", key_value[1], telemetryProps)
    end

    stdout_exclude_paths = ENV["AZMON_STDOUT_EXCLUDED_NAMESPACES"].split(',')
    stderr_exclude_paths = ENV["AZMON_STDERR_EXCLUDED_NAMESPACES"].split(',')
    exclude_names = stdout_exclude_paths & stderr_exclude_paths
    exclude_paths = exclude_names.map {|name| sprintf("_%s_", name)}
    filecount = Dir.glob(File.join("/var/log/containers", '**', '*')).select { |file| File.file?(file) && !(exclude_paths.any? { |path| file.include?(path) }) }.count

    ApplicationInsightsUtility.sendMetricTelemetry("fileTailCount", filecount, telemetryProps)

    sleep 120
  end
end
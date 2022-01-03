#!/usr/local/bin/ruby
# frozen_string_literal: true

require "net/http"
require "net/https"
require "yajl/json_gem"
require "logger"
require "time"

WATCH_ARGUMENTS = {
  "labelSelector" => :label_selector,
  "fieldSelector" => :field_selector,
  "resourceVersion" => :resource_version,
  "allowWatchBookmarks" => :allow_watch_bookmarks,
  "timeoutSeconds" => :timeout_seconds,
}.freeze

# HTTP Stream used to watch changes on entities
class WatchStream
  def initialize(uri, http_options, http_headers, logger)
    @uri = uri
    @http_client = nil
    @http_options = http_options
    @http_headers = http_headers
    @logger = logger
    @logger.info "WatchStream:initialize @ #{Time.now.utc.iso8601}"
  end

  def each
    @finished = false
    buffer = +""
    @logger.info "WatchStream: Opening TCP session  @ #{Time.now.utc.iso8601}"
    @http_client = Net::HTTP.start(@uri.host, @uri.port, @http_options)
    path = @uri.path
    if !@uri.query.nil? && !@uri.query.empty?
      path += "?" + @uri.query
    end
    @logger.info "WatchStream: Making GET API call for Watch with path: #{path}  @ #{Time.now.utc.iso8601}"
    @http_client.request_get(path, @http_headers) do |response|
      if !response.nil? && response.code.to_i > 300
        raise "WatchStream: watch connection failed with an http status code: #{response.code}"
      end
      response.read_body do |chunk|
        buffer << chunk
        while (line = buffer.slice!(/.+\n/))
          yield(Yajl::Parser.parse(StringIO.new(line.chomp)))
        end
      end
    end
  rescue => e
    raise e
  end

  def finish
    begin
      @finished = true
      @logger.info "WatchStream:finish HTTP session @ #{Time.now.utc.iso8601}"
      @http_client.finish if !@http_client.nil? && @http_client.started?
    rescue => error
      @logger.warn "WatchStream:finish failed with an error: #{error} @ #{Time.now.utc.iso8601}"
    end
  end
end

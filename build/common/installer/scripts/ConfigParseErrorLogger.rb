#!/usr/local/bin/ruby
# frozen_string_literal: true

class ConfigParseErrorLogger
  require_relative "microsoft/omsagent/plugin/oj/oj"

  def initialize
  end

  class << self
    def logError(message)
      begin
        errorMessage = "config::error::" + message
        jsonMessage = Oj.dump(errorMessage)
        STDERR.puts jsonMessage
      rescue => errorStr
        puts "Error in ConfigParserErrorLogger::logError: #{errorStr}"
      end
    end
  end
end

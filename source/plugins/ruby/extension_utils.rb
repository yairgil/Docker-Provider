# Copyright (c) Microsoft Corporation.  All rights reserved.
#!/usr/local/bin/ruby
# frozen_string_literal: true

require_relative "extension"
require_relative "constants"

class ExtensionUtils
    class << self
        def getOutputStreamId(dataType)
          outputStreamId = ""
          begin
            if !dataType.nil? && !dataType.empty?
              outputStreamId = Extension.instance.get_output_stream_id(dataType)
              $log.info("ExtensionUtils::getOutputStreamId: got streamid: #{outputStreamId} for datatype: #{dataType}")
            else
              $log.warn("ExtensionUtils::getOutputStreamId: dataType shouldnt be nil or empty")
            end
          rescue => errorStr
            $log.warn("ExtensionUtils::getOutputStreamId: failed with an exception: #{errorStr}")
          end
          return outputStreamId
        end
        def isAADMSIAuthMode()
          return !ENV["AAD_MSI_AUTH_MODE"].nil? && !ENV["AAD_MSI_AUTH_MODE"].empty? && ENV["AAD_MSI_AUTH_MODE"].downcase == "true"
        end
        def getIMDSEndpointHost()
           imdsEndpointHost = Constants::DEFAULT_IMDS_ENDPOINT_HOST
            os_type = ENV["OS_TYPE"]
            if !os_type.nil? && !os_type.empty? && os_type.strip.casecmp("windows") == 0
                imdsEndpointHost = Constants::IMDS_ENDPOINT_HOST_WINDOWS_SIDECAR
            end
           return imdsEndpointHost
        end
    end
end

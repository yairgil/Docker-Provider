# Copyright (c) Microsoft Corporation.  All rights reserved.
#!/usr/local/bin/ruby
# frozen_string_literal: true

class ExtensionUtils
    class << self        
        def getOutputStreamId(dataType)  
          tag = "" 
          begin
            if !dataType.nil? && !dataType.empty?
              tag = Extension.instance.get_output_stream_id(dataType)            
            else           
              $log.warn("ExtensionUtils::getOutputStreamId: dataType shouldnt nil or empty")
            end            
          rescue => errorStr
            $log.warn("ExtensionUtils::getOutputStreamId: failed with an exception: #{errorStr}")
          end    
          return tag     
        end 
        def isAADMSIAuthMode() 
          return !ENV["AAD_MSI_AUTH_MODE"].nil? && !ENV["AAD_MSI_AUTH_MODE"].empty? && ENV["AAD_MSI_AUTH_MODE"].downcase == "true"
        end        
    end
end
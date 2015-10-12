# Copyright (c) Microsoft Corporation.  All rights reserved.

module Fluent
	require 'logger'

	class ContainerFilter < Filter
		Fluent::Plugin.register_filter('filter_container', self)
		
		config_param :enable_log, :integer, :default => 0
		config_param :log_path, :string, :default => '/var/opt/microsoft/omsagent/log/filter_container.log'
		
		def initialize
        	super
    	end

    	def configure(conf)
      		super
			@log = nil
			
			if @enable_log
				@log = Logger.new(@log_path, 'weekly')
				@log.debug {'Starting filter_container plugin'}
			end
    	end

    	def start
     		super
    	end

    	def shutdown
      		super
    	end

    	def filter(tag, time, record)
			dataType = nil
			validItems = Array.new
		
			record.each do |r|
				# Work around until engine uses Computer instead of Host
            	r["Host"] = r["Computer"]

				if dataType == nil
					dataType = case r["ClassName"]
		                when "Container_ImageInventory" then "CONTAINER_IMAGE_INVENTORY_BLOB"
		                when "Container_ContainerInventory" then "CONTAINER_INVENTORY_BLOB"
		                when "Container_DaemonEvent" then "CONTAINER_SERVICE_LOG_BLOB"
	            	end
					
					validItems.push(r)
				else
					if r["ClassName"].eql?(dataType)
						validItems.push(r)
					else
						if @log != nil
							@log.warn {'The object with InstanceID ' + r["InstanceID"] + ' has a type mismatch'}
						end
					end
				end
			end
				
	      	wrapper = {
	        	"DataType"=>dataType,
	        	"IPName"=>"Containers",
	        	"DataItems"=>validItems
	      	}

      		wrapper
    	end
  	end
end

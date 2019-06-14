module HealthModel
    class HealthStateDeserializer

        attr_reader :deserialize_path

        def initialize(path)
            @deserialize_path = path
        end

        def deserialize
            file = File.read("C:/AzureMonitor/ContainerInsights/Docker-Provider/inventory/state.json") #File.read(@deserialize_path)
            records = JSON.parse(file)

            #TODO: even though we call JSON.parse, records is still a string. Do JSON.parse again to return it as a hash
            return JSON.parse(records)
        end
    end
end
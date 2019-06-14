module HealthModel
    class HealthStateSerializer

        attr_reader :serialized_path
        def initialize(path)
            @serialized_path = path
        end

        def serialize(state)
            File.open("C:/AzureMonitor/ContainerInsights/Docker-Provider/inventory/state.json", 'w') do |f| #File.open(@serialized_path, 'w')
            f.write(JSON.pretty_generate(state.to_h.to_json))
           end
        end
    end
end
module HealthModel
    class HealthStateSerializer

        attr_reader :serialized_path
        def initialize(path)
            @serialized_path = path
        end

        def serialize(state)
            File.open(@serialized_path, 'w') do |f|
                states = state.to_h
                states_hash = {}
                states.each{|id, value|
                    states_hash[id] = value.to_h
                }
                f.write(JSON.pretty_generate(states_hash))
           end
        end
    end
end
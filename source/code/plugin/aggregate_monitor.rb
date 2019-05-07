class AggregateMonitor
    attr_accessor :name, :id
    def initialize(name, id) 
        @name = name
        @id = id
    end
    
    def getName
        @name
    end
    
end
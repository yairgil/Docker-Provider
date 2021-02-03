
class PropsToBrackets

    @protobuf_object = nil

    def initialize(protobuf_obj)
        @protobuf_object = protobuf_obj
    end

    def [](ind)
        child_obj = eval("@protobuf_object." + ind)
        if child_obj.class == 1.class || child_obj.class == 1.1.class || child_obj.class == "a".class || child_obj.class == true.class 
            return props_to_brackets(child_obj)
        end
        return child_obj
    end

    def empty?
        return @protobuf_object.empty?
    end

end





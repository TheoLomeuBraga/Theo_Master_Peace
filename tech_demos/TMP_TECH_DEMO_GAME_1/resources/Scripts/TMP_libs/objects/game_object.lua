game_object = {}
function game_object:new(object_ptr)
    obj = {}
    obj.object_ptr = object_ptr
    obj.components = {}
    obj.father = ""
    obj.childrens_size = 0
    obj.childrens = {}
    function obj:add_component(component_name)
        add_component(self.object_ptr,component_name)
        self.components[component_name] = component_map[component_name]:new(self.object_ptr)
    end
    function obj:remove_component(component_name)
        remove_component(self.object_ptr,component_name)
        self.components[component_name]:delet()
    end
    function obj:have_component(comp)
        ret = false
        if have_component(self.object_ptr,comp) == true then
            ret = true
        end
        return ret
    end
    function obj:get_components()
        for key, value in pairs(components) do
            if self:have_component(value) then
                self.components[value] = component_map[value]:new(self.object_ptr)
            end
        end
    end
    function obj:get_family()
        j = get_object_family_json(self.object_ptr)
        self.father = j.father
        self.childrens = deepcopyjson(j.childrens)
    end
    function obj:get()
        self:get_family()
        --self:get_components()
    end
    function obj:set()
    end
    function obj:delet()
        for key,value in ipairs(self.components) do 
            value:delet()
        end
        self.components = nil
        self = nil
    end
    function obj:remove()
        remove_object(self.object_ptr)
    end
    return obj
end





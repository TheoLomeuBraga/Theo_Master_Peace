require("TMP_libs.components.component_index")

transform_component = {}
function transform_component:new(object_ptr)
    t = {}
    t.object_ptr = object_ptr
    t.is_ui = false
    t.position = Vec3:new(0,0,0)
    t.rotation = Vec3:new(0,0,0)
    t.scale = Vec3:new(1,1,1)
    function t:get()
        j = json.decode(get_transform_json(self.object_ptr))
        self.is_ui = j.is_ui
        self.position = deepcopyjson(j.position)
        self.rotation = deepcopyjson(j.rotation)
        self.scale = deepcopyjson(j.scale)
        
    end
    function t:set()

        j = {}
        j.is_ui = self.is_ui
        j.position = deepcopyjson(self.position)
        j.rotation = deepcopyjson(self.rotation)
        j.scale = deepcopyjson(self.scale)
        set_transform_json(self.object_ptr,json.encode(j))
        
        
    end
    function t:get_global_position()
        ret = Vec3:new(0,0,0)
        ret.x, ret.y,ret.z = get_global_position(self.object_ptr)
        return ret
    end
    function t:get_global_rotation()
        ret = Vec3:new(0,0,0)
        ret.x, ret.y,ret.z = get_global_rotation(self.object_ptr)
        return ret
    end
    function t:change_position(x,y,z)
        change_transfotm_position(self.object_ptr,x,y,z)
    end
    function t:change_rotation(x,y,z)
        change_transfotm_rotation(self.object_ptr,x,y,z)
    end
    function t:change_scale(x,y,z)
        change_transfotm_scale(self.object_ptr,x,y,z)
    end
    function t:delet()
        self.position = nil
        self.rotation = nil
        self.scale = nil
        self = nil
    end
    return t
end
component_map[components.transform] = transform_component

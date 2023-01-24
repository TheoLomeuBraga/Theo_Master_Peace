require("TMP_libs.components.component_index")


render_shader_component = {}
function render_shader_component:new(object_ptr)
    rs = {}
    rs.object_ptr = object_ptr
    rs.layer = 1
    rs.material = matreial:new()
    function rs:get()
        --layer
        self.layer = get_render_layer(self.object_ptr)
        --material
        self.material:get(self.object_ptr,0)
        
    end
    function rs:set()
        --layer
        set_render_layer(self.object_ptr,self.layer)
        --material
        self.material:set(self.object_ptr,0)

    end
    function rs:delet()
        self.material:delet()
        self = nil
    end
    return rs
end
component_map[components.render_shader] = render_shader_component
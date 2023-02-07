require("TMP_libs.components.component_index")


function get_render_shader(object)
end
function set_render_shader(object,json)
end


render_shader_component = {}
function render_shader_component:new(object_ptr)
    rs = {}
    rs.object_ptr = object_ptr
    rs.layer = 1
    rs.material = matreial:new()
    function rs:get()
        j = json.decode(get_render_shader(self.object_ptr))
        self.layer =  j.layer
        self.material = deepcopyjson(j.material) 
    end
    function rs:set()
        j = {}
        j.layer = self.layer
        j.material = deepcopyjson(self.material)
        
        print(json.encode(j))
        set_render_shader(self.object_ptr,json.encode(j))
    end
    function rs:delet()
        
    end
    return rs
end
component_map[components.render_shader] = render_shader_component
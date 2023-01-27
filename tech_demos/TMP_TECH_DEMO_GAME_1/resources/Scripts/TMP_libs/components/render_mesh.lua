require("TMP_libs.components.component_index")

--mesh
function get_mesh_json(object)
end
function set_mesh_json(object,json)
end

mesh_location = {}
function mesh_location:new(file,name)
    ret = {}
    ret.file = file
    ret.name = name
    return ret
end

render_mesh_component = {}
function render_mesh_component:new(object_ptr)
    rm = {}
    rm.object_ptr = object_ptr
    rs.layer = 1
    rm.use_oclusion = false
    --rm.meshes_cout = 0
    rm.normal_direction = 0
    rm.meshes = {}
    rm.materials = {}
    function rm:get()
        j = json.decode(get_mesh_json(self.object_ptr));
        self.layer = j.layer
        self.use_oclusion = j.use_oclusion
        self.normal_direction = j.normal_direction
        self.meshes = deepcopyjson(j.meshes) 
        self.materials = deepcopyjson(j.materials) 
        
        

    end
    --buscar por erros
    function rm:set()
        j = {}
        j.layer = self.layer
        j.use_oclusion = self.use_oclusion
        j.normal_direction = self.normal_direction
        j.meshes = deepcopyjson(self.meshes)
        j.materials = deepcopyjson(self.materials)
        set_mesh_json(self.object_ptr,json.encode(j));

        
    end
    return rm
end
component_map[components.render_mesh] = render_mesh_component
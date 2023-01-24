require("TMP_libs.core")
require("TMP_libs.vectors")
require("TMP_libs.assets")
require("TMP_libs.functions")

require("math")

json = require("libs.json")


--this object ptr
this_object_ptr = ""

matreial = {}
function matreial:new()
    m = {}
    m.shader = ""
    m.color = {}
    m.color.r = 1
    m.color.g = 1
    m.color.b = 1
    m.color.a = 1
    m.position_scale = Vec4:new(0,0,1,1)
    m.metallic = 0 
    m.softness = 0
    m.textures = {}
    m.inputs = {}
    function m:get(object,material_id)
        self.shader = get_shader(object,material_id)
        self.color.r,self.color.a,self.color.b,self.color.a = get_color(object,material_id)
        self.position_scale.x,self.position_scale.y,self.position_scale.z,self.position_scale.w = get_material_position_scale(object,material_id)
        i = 0
        while i < get_max_textures_material() do
            self.textures[i] = get_texture(object,material_id,i)
            i = i+1
        end
        i = 0
        while i < get_max_inputs_material() do
            self.inputs[i] = get_material_input(object,material_id,i)
            i = i+1
        end
    end
    function m:set(object,material_id)
        set_shader(object,material_id,self.shader)
        set_color(object,material_id,self.color.r,self.color.a,self.color.b,self.color.a)
        set_material_position_scale(object,material_id,self.position_scale.x,self.position_scale.y,self.position_scale.z,self.position_scale.w)
        i = 0
        while i < get_max_textures_material() do
            if self.textures[i] ~= nil then
                set_texture(object,material_id,i,self.textures[i])
            end
            
            i = i+1
        end
        i = 0
        while i < get_max_inputs_material() do
            if self.inputs[i] ~= nil then
                set_material_input(object,material_id,i,self.inputs[i])
            end
            
            i = i+1
        end
    end
    function m.delet()
        self.color = nil
        self.textures = nil
        self.inputs = nil
        self.position_scale = nil
        self = nil
    end
    return m
end

mesh_location = {}
function mesh_location:new(file,name)
    ret = {}
    ret.file = file
    ret.name = name
    return ret
end

colision_layer_info = {}
function colision_layer_info:new()
    cli = {}
    cli.layer = 1
    cli.layers_can_colide = {}
    cli.layers_can_colide[0] = 0
    return cli
end

Time = {}
Time.time = 0
Time.delta = 0
Time.scale = 0
function Time:set_speed(speed)
    set_time_scale(speed)
end
function Time:get()
    self.time, self.delta,self.sacale = get_time()
end

--animation
animation = {}
function animation:new()
    a = {}
    a.time = 0
    a.duration = 0
    a.loop = true
    a.data = {}
    a.data.size = 0
    function a:reset()
        self.time = 0
    end
    function a:to_progress(speed)
        Time:get()
        self.time = self.time + (Time.delta * speed)
        if self.loop == true and self.time > self.duration then
            self.time = self.time - self.duration
        end
        if self.loop == false and self.time > self.duration then
            self.time = self.duration
        end
    end
    --this function may causes errors
    function a:get_frame()
        f = 0
        --duration  size
        --time      x
        f = self.data[math.floor((self.time * self.data.size) / self.duration)]
        return f
    end
    return a
end


--componentes
component_map = {}

--transform
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

--camera
camera_component = {}
function camera_component:new(object_ptr)
    c = {}
    c.object_ptr = object_ptr
    c.type = ""
    c.size = Vec2:new(20,20)
    c.zoom = 90
    c.resolution = Vec2:new(100,100)
    c.fcp = 0
    c.ncp = 0
    function c:get()
        self.type = get_camera(self.object_ptr)
        if self.type == camera_view_types.ortho then 
            self.type , self.size.x , self.size.y , self.ncp , self.fcp  = get_camera(self.object_ptr)
        end
        if self.type == camera_view_types.perspective then 
            self.type , self.zoom , self.resolution.x , self.resolution.y , self.ncp , self.fcp  = get_camera(self.object_ptr)
        end
    end
    function c:set()
        if self.type == camera_view_types.ortho then 
            set_camera(self.object_ptr, self.size.x,self.size.y,self.ncp,self.fcp)
        end
        if self.type == camera_view_types.perspective then 
            set_camera(self.object_ptr, self.zoom, self.resolution.x,self.resolution.y,self.ncp,self.fcp)
        end
    end
    function c:delet()
        self.size = nil
        self.resolution = nil
        self = nil
    end
    return c
end
component_map[components.camera] = camera_component

--physics_2D
physics_2D_component = {}
function physics_2D_component:new(object_ptr)
    p = {}
    p.object_ptr = object_ptr
    p.scale = Vec2:new(1,1)
    p.boady_dynamic = boady_dynamics.static
    p.colision_shape = colision_shapes.tile
    p.rotate = true
    p.triger = false
    p.friction = 1
    p.objects_coliding = {}
    p.colision_layer = colision_layer_info:new()
    function p:get()
        
        j = json.decode(get_physic_2D_json(self.object_ptr))
        self.scale = deepcopyjson(j.scale)
        self.boady_dynamic = j.boady_dynamic
        self.colision_shape = j.colision_shape
        self.rotate = j.rotate
        self.triger = j.triger
        self.friction = j.friction
        self.objects_coliding = deepcopyjson(j.objects_coliding)
        self.colision_layer = deepcopyjson(j.colision_layer)
        
    end
    function p:set()
        j = {}
        j.scale = deepcopyjson(self.scale)
        j.boady_dynamic = self.boady_dynamic
        j.colision_shape = self.colision_shape
        j.rotate = self.rotate
        j.triger = self.triger
        j.friction = self.friction
        set_physic_2D_json(self.object_ptr,json.encode(j)) 
    end
    function p:to_move(speed_x,speed_y)
        to_move(self.object_ptr,speed_x,speed_y)
    end
    function p:add_force(force_x,force_y)
        add_force(self.object_ptr,force_x,force_y)
    end
    function p:delet()
        self.scale = nil
        self.objects_coliding = nil
        self = nil
    end
    return p
end
component_map[components.physics_2D] = physics_2D_component

--character_physics_2D
character_physics_2D_component = {}
function character_physics_2D_component:new(object_ptr)
    p = {}
    p.object_ptr = object_ptr
    p.floor = false
    p.ceiling = false
    function p:get()
        self.floor = in_floor(self.object_ptr)
        self.ceiling = in_ceiling(self.object_ptr)
    end
    function p:set()
    end
    function p:delet()
        self = nil
    end
    return p
end
component_map[components.character_physics_2D] = character_physics_2D_component

--audio
audio_component = {}
function audio_component:new(object_ptr)
    a = {}
    a.object_ptr = object_ptr
    a.path = ""
	a.pause = false
	a.loop = false
	a.time = 0.001
	a.speed = 1
    a.volume = 5

    function a:set()
        set_audio(self.object_ptr,self.path,self.pause,self.loop,self.time,self.speed,self.volume)
    end
    function a:get()
        self.path,self.pause,self.loop,self.time,self.speed,self.volume = get_audio(self.object_ptr)
    end
    function a:delet()
        self = nil
    end

    return a
end
component_map[components.audio_source] = audio_component

--render_shader
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

--render_sprite
render_sprite_component = {}
function render_sprite_component:new(object_ptr)
    rs = {}
    rs.object_ptr = object_ptr
    rs.layer = 1
    rs.selected_tile = 0
    rs.tile_set_local = ""
    rs.material = matreial:new()
    function rs:get()
        j = json.decode(get_sprite_render_json(self.object_ptr))
        self.layer = j.layer
        self.selected_tile = j.selected_tile
        self.tile_set_local = j.tile_set_local
        self.material = deepcopyjson(j.material)

        --[[
        self.selected_tile = get_tile(self.object_ptr)
        --layer
        self.layer = get_render_layer(self.object_ptr)
        --material
        self.material:get(self.object_ptr,0)
        
        self.tile_set_local = get_tileset(self.object_ptr)
        ]]

    end
    function rs:set()
        j = {}
        j.layer = self.layer
        j.selected_tile = self.selected_tile
        j.tile_set_local = self.tile_set_local
        j.material = deepcopyjson(self.material)

        set_sprite_render_json(self.object_ptr,json.encode(j))

        --[[
        set_tile(self.object_ptr,self.selected_tile)
        --layer
        set_render_layer(self.object_ptr,self.layer)
        --material
        self.material:set(self.object_ptr,0)

        set_tileset(self.object_ptr,rs.tile_set_local)
        ]]

    end
    function rs:delet()
        self.material:delet()
        self = nil
    end
    return rs
end
component_map[components.render_sprite] = render_sprite_component

--render_sprite
render_text_component = {}
function render_text_component:new(object_ptr)
    rs = {}
    rs.object_ptr = object_ptr
    rs.layer = 1
    rs.font = ""
    rs.text = ""
    rs.line_size = 34
    rs.space_betwen_chars = 0
    rs.max_space_betwen_chars = 3
    rs.min_space_betwen_chars = 0
    rs.material = matreial:new()
    function rs:get()

        j = {}
        j = deepcopyjson(json.decode(get_text_json(self.object_ptr)))

        self.layer = j.layer
        self.font = j.font
        self.text = j.text
        self.line_size = j.line_size
        self.space_betwen_chars = j.space_betwen_chars
        self.max_space_betwen_chars = j.max_space_betwen_chars
        self.min_space_betwen_chars = j.min_space_betwen_chars
        self.material = j.material

        --[[
        self.text = get_text(self.object_ptr)
        --layer
        self.layer = get_render_layer(self.object_ptr)
        --material
        self.material:get(self.object_ptr,0)
        self.font = get_font(self.object_ptr)
        ]]
    end
    function rs:set()

        j = {}

        j.layer = self.layer
        j.font = self.font
        j.text = self.text
        j.line_size = self.line_size
        
        j.space_betwen_chars = self.space_betwen_chars
        j.max_space_betwen_chars = self.max_space_betwen_chars
        j.min_space_betwen_chars = self.min_space_betwen_chars
        j.material = deepcopyjson(self.material)

        set_text_json(self.object_ptr,json.encode(j))
        
        --[[
        set_text(self.object_ptr,self.text)
        --layer
        set_render_layer(self.object_ptr,self.layer)
        --material
        self.material:set(self.object_ptr,0)
        set_font(self.object_ptr,self.font)
        ]]

    end
    function rs:delet()
        self.material:delet()
        self = nil
    end
    return rs
end
component_map[components.render_text] = render_text_component

render_tile_map_component = {}
function render_tile_map_component:new(object_ptr)
    rtm = {}
    rtm.object_ptr = object_ptr
    rtm.layer = 1
    rtm.material = matreial:new()
    rtm.render_tilemap_only_layer = -1
    rtm.tile_set_local = ""
    rtm.tile_map_local = ""
    function rtm:get()
        j = json.decode(get_render_tilemap_json(rtm.object_ptr))
        self.layer = j.layer
        self.material = deepcopyjson(j.material)
        self.render_tilemap_only_layer = j.render_tilemap_only_layer
        self.tile_set_local = j.tile_set_local
        self.tile_map_local = j.tile_map_local
        --[[
        self.layer = get_render_layer(self.object_ptr)
        self.material:get(self.object_ptr,0)
        self.render_tilemap_only_layer = get_render_only_tilemap_layer(self.object_ptr)
        self.tile_set_local = get_tileset(self.object_ptr)
        self.tile_map_local = get_tilemap(self.object_ptr)
        ]]
    end
    function rtm:set()
        j = {}
        j.layer = self.layer
        j.material = deepcopyjson(self.material)
        j.render_tilemap_only_layer = self.render_tilemap_only_layer
        j.tile_set_local = self.tile_set_local
        j.tile_map_local = self.tile_map_local
        set_render_tilemap_json(rtm.object_ptr,json.encode(j))
        --[[
        set_render_layer(self.object_ptr,self.layer)
        self.material:set(self.object_ptr,0)
        set_render_only_tilemap_layer(self.object_ptr,self.render_tilemap_only_layer)
        set_tileset(self.object_ptr,self.tile_set_local)
        set_tilemap(self.object_ptr,self.tile_map_local)
        ]]
        
    end
    function rtm:delet()
        self.material:delet()
        self = nil
    end
    return rtm
end
component_map[components.render_tile_map] = render_tile_map_component

--render mesh
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

lua_scripts_component = {}
function lua_scripts_component:new(object_ptr)
    ls = {}
    ls.object_ptr = object_ptr
    ls.scripts = {}
    function ls:get()
        self.scripts = nil
        self.scripts = {}
        i = 0
        while i < get_script_size(self.object_ptr) do
            self.scripts[i] = get_script_name(self.object_ptr,i)
            i = i + 1
        end
    end
    function ls:set()
    end
    function ls:add_script(script_name)
        add_script_lua(self.object_ptr,script_name)
    end
    function ls:remove_script(script_name)
        remove_script(self.object_ptr,script_name)
    end
    function ls:get_variable(script_name,variable_type,variable_name)
        return get_script_var(self.object_ptr,script_name,variable_name,variable_type)
    end
    function ls:set_variable(script_name,variable_name,variable_type,variable_value)
        set_script_var(self.object_ptr,script_name,variable_name,variable_type,variable_value)
    end
    function ls:call_function(script_name,function_name)
        call_script_function(self.object_ptr,script_name,function_name)
    end
    
    function ls:delet()
        self.scripts = nil
        self = nil
    end
    return ls
end
component_map[components.lua_scripts] = lua_scripts_component

--game_object
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





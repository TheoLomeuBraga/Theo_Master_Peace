
function deepcopy(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[deepcopy(orig_key)] = deepcopy(orig_value)
        end
        setmetatable(copy, deepcopy(getmetatable(orig)))
    else -- number, string, boolean, etc
        copy = orig
    end
    return copy
end

function deepcopyjson(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[deepcopyjson(orig_key)] = deepcopyjson(orig_value)
        end
        setmetatable(copy, deepcopyjson(getmetatable(orig)))
    else 
        if orig_type ~= 'function' then
            copy = orig
        end
    end
    return copy
end



--time
function get_time()
end
function set_time_scale(speed)
end

--memory
function clear_memory()
end

function asset_is_load(asset_type,path)
end

--asset tiles
function get_tile_set_size(path,image_folder)
end
function get_tile_set_tile(path,image_folder,tile_id)
end

function get_tilemap_size(path)
end
function get_tilemap_layer_size(path)
end
function get_tilemap_data(path,layer,id)
end



--asset 3D scene
--adicionar lua cena 3D
function get_scene_3D_json(path)
end

--object
function create_object()
end
function get_object_with_name(name)
end
function create_object(parent)
end
function remove_object(object)
end
function add_component(object,component)
end
function remove_component(object,component)
end
function get_object_family_json(object)
end



--moovement
function to_move(object,x,y)
end 
function to_move(object,x,y,z)
end 
function in_ceiling(object)
end
function in_floor(object)
end

--gravity
function get_gravity()
end
function set_gravity(x,y,z)
end



--transform

function get_transform_json(object)
end
function set_transform_json(object,json)
end

function move_transform(object,x,y,z)
end
function rotate_transform(object,x,y,z)
end

function change_transfotm_position(object,x,y,z)
end
function change_transfotm_rotation(object,x,y,z)
end
function change_transfotm_scale(object,x,y,z)
end

--physic
function get_physic_2D(object)
end
function set_physic_2D(object,scale_x,scale_y,dynamic,shape,rotate,is_triger)
end
function add_force(object,force_x,force_y)
end

function get_physic_layer(object)
end
function set_physic_layer(object,layer)
end
function get_layers_can_colide_size(object)
end
function set_layers_can_colide_size(object,size)
end

function get_colisions_number(object)
end
function get_colision_object(object,id)
end

function set_layers_can_colide(object,id_layer,layer)
end
function get_layers_can_colide(object,id_layer)
end

function get_physic_2D_json(object) 
end
function set_physic_2D_json(object,json) 
end

--render
function set_resolution(x,y)
end
function get_render_layer(object)
end
function set_render_layer(object,layer)
end

--sprite
function set_tileset(object,tileset)
end
function get_tileset(object)
end

function get_sprite_render_json(object)
end
function set_sprite_render_json(object,json)
end


--tilemap

function get_render_tilemap_json(object)
end
function set_render_tilemap_json(object,json)
end

--text

function set_font(object,font)
end
function get_font(object)
end
function set_text(object,text)
end
function get_text(object)
end

function get_text_json(object)
end
function set_text_json(object,json)
end


--material
function get_max_textures_material()
end
function get_max_inputs_material()
end

function set_shader(object,material_id,shader)
end
function get_shader(object,material_id)
end

function get_color(object,material_id)
end
function set_color(object,material_id,r,g,b,a)
end

function get_material_position_scale(object,material_id)
end
function set_material_position_scale(object,material_id,position_X,position_Y,scale_X,scale_Y)
end

function set_texture(object,material_id,texture_id,texture)
end
function get_texture(object,material_id,texture_id)
end

function set_material_input(object,material_id,input_id,input)
end
function get_material_input(object,material_id,input_id)
end

--camera
function set_camera(object, sizeX,sizeY,ncp,fcp)
end
function set_camera(object, zoom, resX,resY,ncp,fcp)
end
function get_camera(object)
end

--audio
function set_audio(object,path,pause,loop,time,speed,volume)
end
function get_audio(object)
end

--scripts
function get_script_size(object)
end
function get_script_name(object,script_id)
end
function have_script(object,script)
end
function add_script(object,script)
end
function remove_script(object,script)
end
function get_var_script(object,script,variable,type)
end
function have_component(object,component)
end
--function set_var_script(object,script,variable,type,value)
--end
--function get_script_var(object,script,variable,type)
--end

function set_script_var(object,script,variable,type,value)
end
function call_script_function(object,script_name,function_name)
end

--mesh
function get_mesh_json(object)
end
function set_mesh_json(object,json)
end

--post-procesing

function get_post_processing_json()
end
function set_post_processing_json(json)
end

--render layers
function get_render_layer_instruction_json()
end
function set_render_layer_instruction_json(json)
end
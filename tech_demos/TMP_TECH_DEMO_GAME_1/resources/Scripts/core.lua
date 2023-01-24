require("TMP_libs.definitions")
require("TMP_libs.components.component_table")
require("TMP_libs.components.index")
require("TMP_libs.stystems")
require("TMP_libs.input")
require("TMP_libs.layers_table")

require("math")
json = require("libs.json")


require("TMP_libs.load_2D_map")

--layer_list = layers_table:new()

leyers = layers_table:new_2D()--{}


hud = nil
background_image = nil
background_3D = nil
background_3D2 = nil
background = nil
cenary = nil
character = nil
camera = nil







function clean_layer(ptr)
    obj = game_object:new(ptr)
    obj:get_family()
    --obj:get()
    i = 0
    print("childrens_size",obj.childrens_size)
    while i < obj.childrens_size do
        remove_object(obj.childrens[i])
        i = i + 1
    end
    
end





function initialize_layers()
    leyers:create()

    background_image = game_object:new(leyers.background_image)
    background_3D = game_object:new(create_object(leyers.background_3D))
    background_3D2 = game_object:new(create_object(leyers.background_3D))
    hud = game_object:new(leyers.hud)
    hud:add_component(components.transform)
    hud.components[components.transform].is_ui = true
    hud.components[components.transform].position = Vec3:new(-0.9, 0.9, 0)
    hud.components[components.transform].scale = Vec3:new(0.05, 0.05, 0.05)
    hud.components[components.transform]:set()
    hud:add_component(components.render_text)
    hud.components[components.render_text].layer = 2
    hud.components[components.render_text].material.shader = "resources/Shaders/text"
    hud.components[components.render_text].font = "resources/Fonts/pixel_snas.json"
    hud.components[components.render_text].text = "ola mundo"
    hud.components[components.render_text]:set()
end










function load_level(map,background_image_path,music_path)
    
    


    

    --background
    cenary = game_object:new(leyers.cenary)
    cenary:add_component(components.transform)
    cenary.components[components.transform].position = Vec3:new(10,0,0)
    cenary.components[components.transform].rotation = Vec3:new(0,90,0)
    cenary.components[components.transform].scale = Vec3:new(0.1,0.1,0.1)
    cenary.components[components.transform]:set()
    cenary:add_component(components.render_tile_map)
    cenary.components[components.render_tile_map].material.shader = "resources/Shaders/sprite"
    cenary.components[components.render_tile_map].render_only_tilemap_layer = 0
    cenary.components[components.render_tile_map].tile_set_local = "resources/Leveis 2D/teste/tile_set_test.json"
    cenary.components[components.render_tile_map].tile_set_image_folder = "resources/Leveis 2D/teste"
    cenary.components[components.render_tile_map].tile_map_local = map
    cenary.components[components.render_tile_map]:set()

    
    

    
    
    
    --background_image
    
    
    background_image:add_component(components.transform)
    background_image.components[components.transform].scale = Vec3:new(10,10,10)
    background_image.components[components.transform]:set()
    background_image:add_component(components.render_shader)
    background_image.components[components.render_shader].material.shader = "resources/Shaders/fundo"
    background_image.components[components.render_shader].material.textures[0] = background_image_path
    background_image.components[components.render_shader]:set()

    
    
    


    background_3D:add_component(components.transform)
    background_3D.components[components.transform].position = Vec3:new(10,5,0)
    background_3D.components[components.transform].rotation = Vec3:new(0,90,0)
    background_3D.components[components.transform].scale = Vec3:new(2,2,2)
    background_3D.components[components.transform]:set()
    
    background_3D:add_component(components.render_mesh)
    background_3D.components[components.render_mesh].layer = 1
    background_3D.components[components.render_mesh].meshes_cout = 1
    background_3D.components[components.render_mesh].meshes = {mesh_location:new("resources/3D Models/cube_sphere.obj","Cube")}
    background_3D.components[components.render_mesh].materials = {matreial:new()}
    background_3D.components[components.render_mesh].materials[1].textures[1] = background_image_path
    background_3D.components[components.render_mesh].materials[1].color.g = 0
    background_3D.components[components.render_mesh].materials[1].color.b = 0
    background_3D.components[components.render_mesh].materials[1].shader = "resources/Shaders/test_geometry_shader"
    background_3D.components[components.render_mesh]:set()
    
    

    scena_3D_teste = scene_3D:new()
    scena_3D_teste:get("resources/3D Models/cube_sphere.obj")
    

    
    background_3D2:add_component(components.transform)
    background_3D2.components[components.transform].position = Vec3:new(10,-3,0)
    background_3D2.components[components.transform].rotation = Vec3:new(0,0,0)
    background_3D2.components[components.transform].scale = Vec3:new(2,2,2)
    background_3D2.components[components.transform]:set()

    

    background_3D2:add_component(components.render_sprite)
    background_3D2.components[components.render_sprite].layer = 1
    background_3D2.components[components.render_sprite].material.shader = "resources/Shaders/sprite"
    background_3D2.components[components.render_sprite].selected_tile = 2
    background_3D2.components[components.render_sprite].tile_set_local = "resources/Leveis 2D/personajem.json"
    background_3D2.components[components.render_sprite]:set()

    
    camera.components[components.audio_source].path = music_path
    camera.components[components.audio_source].loop = true
    camera.components[components.audio_source].volume = 5
    camera.components[components.audio_source]:set()

    --character.components[components.lua_scripts]:call_function("resources/Scripts/script_character.lua","wen_scene_is_loaded")
    clear_memory()
    
end


level = {}
function level:new(map,background_image_path,music_path)
    l = {}
    l.map = map
    l.background_image_path = background_image_path
    l.music_path = music_path
    function l:load()
        load_level(self.map,self.background_image_path,self.music_path)
    end
    return l
end 



leveis = {}
leveis.curent_level = 0
leveis[1] = level:new("resources/Leveis 2D/teste/tile_map_test.json","resources/Textures/fundo A.png","resources/Audio/teste de audio.wav")


function leveis:next()
    self.curent_level = self.curent_level + 1
    self[self.curent_level]:load()
end
function next_level()
    leveis:next()
end


function initialize_render_layers()
    --[[
    window.res.x = 720
    window.res.y = 720
    window:set()
    --]]

    renders_layers_instructions.layers_size = 4

    renders_layers_instructions.layers[1] = render_layer_instructions:new()
    renders_layers_instructions.layers[1].end_render = false
    renders_layers_instructions.layers[1].use_deeph = true
    renders_layers_instructions.layers[1].clean_color = false
    renders_layers_instructions.layers[1].start_render = true

    renders_layers_instructions.layers[2] = render_layer_instructions:new()
    renders_layers_instructions.layers[2].end_render = false
    renders_layers_instructions.layers[2].use_deeph = true
    renders_layers_instructions.layers[2].clean_color = false
    renders_layers_instructions.layers[2].start_render = false

    renders_layers_instructions.layers[3] = render_layer_instructions:new()
    renders_layers_instructions.layers[3].end_render = true
    renders_layers_instructions.layers[3].use_deeph = true
    renders_layers_instructions.layers[3].clean_color = false
    renders_layers_instructions.layers[3].start_render = false

    renders_layers_instructions:set()
    
end

t = tile:new()
tm = tile_map:new()
function START()
    
	print("core iniciando")
    

    initialize_render_layers()
    

    

    initialize_layers()
    
    --camera
    camera = game_object:new(create_object(leyers.camera))
    camera:add_component(components.camera)
    camera:add_component(components.transform)
    camera:add_component(components.audio_source)
    
    camera.components[components.transform].position = Vec3:new(0, 0, 0)
    

    
    camera.components[components.camera].type = camera_view_types.perspective
    camera.components[components.camera].resolution = Vec2:new(720,720)
    camera.components[components.camera].zoom = 90
    camera.components[components.camera].size = Vec2:new(20,20)

    camera.components[components.camera].type = camera_view_types.ortho
    
    
    
    
    camera.components[components.camera].ncp = 0.1
    camera.components[components.camera].fcp = 100

    
    
    

    
    camera.components[components.transform]:set()
    camera.components[components.camera]:set()
    
    

    --give camera to char
    --character.components[components.lua_scripts]:set_variable("resources/Scripts/script_character.lua","camera",variable_types.string,camera.object_ptr)
    
    
    
    leveis:next()
    
    

end


pos_cam = Vec3:new()


rotation_value = 0
rotation_speed = 50
function UPDATE()
    Time:get()
    background_3D.components[components.transform]:change_rotation(0,rotation_value,0)
    rotation_value = rotation_value + (Time.delta * rotation_speed)
end

function COLIDE(Colision)
end

function END()
end







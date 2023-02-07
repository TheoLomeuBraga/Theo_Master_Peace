require("TMP_libs.definitions")
require("TMP_libs.components.component_table")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.objects.game_object")
require("TMP_libs.objects.time")
require("TMP_libs.stystems")
require("TMP_libs.input")
require("TMP_libs.layers_table")
require("TMP_libs.objects.render_layer")


require("math")
json = require("libs.json")


require("TMP_libs.short_cuts.load_2D_map")
require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.create_camera")

require("TMP_libs.material")


leyers = layers_table:new_2D()

cenary_info = nil

hud = nil
background_image = nil
background_3D = nil
background_3D2 = nil
background = nil
cenary = nil
character = nil
camera = nil
audio = nil






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
    background_3D2 = game_object:new(create_object(leyers.background_3D))


    hud_material = matreial:new()
    hud_material.shader = "resources/Shaders/text"
    hub = create_text(leyers.hud, true, Vec3:new(-0.9, 0.9, 0),Vec3:new(0.0, 0.0, 0.0),Vec3:new(0.05, 0.05, 0.05), hud_material, 2, "ola mundo", "resources/Fonts/pixel_snas.json")
end










function load_level(map,background_image_path,music_path)
    
    


    

    

    
    cenary_mat = matreial:new()
    cenary_mat.shader = "resources/Shaders/sprite"
    cenari_all = load_2D_map(leyers.cenary,Vec3:new(10,0,0),Vec3:new(0,90,0),Vec3:new(0.1,0.1,0.1),map,"resources/Leveis 2D/teste/tile_set_test.json","resources/Leveis 2D/teste",cenary_mat)
    cenary = cenari_all.map_object
    cenary_info = cenari_all.map_info

    
    
    
    --background_image
    
    
    background_image:add_component(components.transform)
    background_image.components[components.transform].scale = Vec3:new(10,10,10)
    background_image.components[components.transform]:set()
    background_image:add_component(components.render_shader)
    background_image.components[components.render_shader].material.shader = "resources/Shaders/fundo"
    background_image.components[components.render_shader].material.textures[0] = background_image_path
    background_image.components[components.render_shader]:set()

    
    
    

    --[[
    background_3D:add_component(components.transform)
    background_3D.components[components.transform].position = Vec3:new(10,5,0)
    background_3D.components[components.transform].rotation = Vec3:new(0,90,0)
    background_3D.components[components.transform].scale = Vec3:new(2,2,2)
    background_3D.components[components.transform]:set()
    ]]

    
    background_3D_material = material:new()
    background_3D_material.textures[1] = background_image_path
    background_3D_material.color.g = 0
    background_3D_material.color.b = 0
    background_3D_material.shader = "resources/Shaders/test_geometry_shader"


    --[[
    background_3D:add_component(components.render_mesh)
    background_3D.components[components.render_mesh].layer = 1
    background_3D.components[components.render_mesh].meshes_cout = 1
    background_3D.components[components.render_mesh].meshes = {mesh_location:new("resources/3D Models/cube_sphere.obj","Cube")}
    background_3D.components[components.render_mesh].materials = {background_3D_material}
    background_3D.components[components.render_mesh]:set()
    ]]
    background_3D = create_mesh(leyers.background_3D,false,Vec3:new(10,5,0),Vec3:new(0,90,0),Vec3:new(2,2,2),{background_3D_material},{mesh_location:new("resources/3D Models/cube_sphere.obj","Cube")})

    scena_3D_teste = scene_3D:new()
    scena_3D_teste:get("resources/3D Models/cube_sphere.obj")
    

    
    background_3D2:add_component(components.transform)
    background_3D2.components[components.transform].position = Vec3:new(10,-3,0)
    background_3D2.components[components.transform].rotation = Vec3:new(0,0,0)
    background_3D2.components[components.transform].scale = Vec3:new(1,2,1)
    background_3D2.components[components.transform]:set()

    

    background_3D2:add_component(components.render_sprite)
    background_3D2.components[components.render_sprite].layer = 1
    background_3D2.components[components.render_sprite].material.shader = "resources/Shaders/sprite"
    background_3D2.components[components.render_sprite].selected_tile = 2
    background_3D2.components[components.render_sprite].tile_set_local = "resources/Leveis 2D/personajem.json"
    background_3D2.components[components.render_sprite]:set()
    
    --audio = create_audio(music_path,true,100)

    
    audio = create_3D_audio(create_object(),Vec3:new(6,0,0),music_path,true,100,5,10)
    
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

    renders_layers.layers_size = 4

    renders_layers.layers[1] = render_layer:new()
    renders_layers.layers[1].end_render = false
    renders_layers.layers[1].use_deeph = true
    renders_layers.layers[1].clean_color = false
    renders_layers.layers[1].start_render = true

    renders_layers.layers[2] = render_layer:new()
    renders_layers.layers[2].end_render = false
    renders_layers.layers[2].use_deeph = true
    renders_layers.layers[2].clean_color = false
    renders_layers.layers[2].start_render = false

    renders_layers.layers[3] = render_layer:new()
    renders_layers.layers[3].end_render = true
    renders_layers.layers[3].use_deeph = true
    renders_layers.layers[3].clean_color = false
    renders_layers.layers[3].start_render = false

    renders_layers:set()
    
end

t = tile:new()
tm = tile_map:new()
function START()
    
	print("core iniciando")
    

    initialize_render_layers()
    

    

    initialize_layers()
    
    
    camera = create_camera_ortho(leyers.camera,Vec3:new(-1, 0, 0),Vec3:new(0, 0, 0),20,20,720,720,0.1,100)
    camera:add_component(components.audio_source)
    set_lisener_object(camera.object_ptr)

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







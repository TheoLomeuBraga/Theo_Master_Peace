--short cuts
require("TMP_libs.short_cuts.load_2D_map")
require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.create_camera")
require("TMP_libs.short_cuts.fps_counter")
require("TMP_libs.short_cuts.create_render_shader")

require("TMP_libs.components.component_all")

require("TMP_libs.layers_table")



map_name_list = {
    "test",
}


this_sceane = {}
this_sceane.camera = nil
this_sceane.tile_map_info = nil
this_sceane.background = nil
this_sceane.objects_layesrs = nil

function this_sceane:unload()
end










sceanes_db = {}



sceanes_db.test = {}
function sceanes_db.test:load()
    print("loading m1")
    this_sceane.objects_layesrs = layers_table:new_2D()
    this_sceane.objects_layesrs:create()
    --background
    background_material = material:new()
    background_material.shader = "resources/Shaders/fundo"
    background_material.textures[1] = "resources/Textures/fundo A.png"
    this_sceane.background = create_render_shader(this_sceane.objects_layesrs.background_image,false,Vec3:new(0, 0, 0),Vec3:new(0, 0, 0),Vec3:new(1, 1, 1),1,background_material)

    --camera
    this_sceane.camera = create_camera_ortho(this_sceane.objects_layesrs.camera,Vec3:new(-1, 0, 0),Vec3:new(0, 0, 0),5,5,720,720,0.1,100)
    set_lisener_object(this_sceane.camera.object_ptr)

    --tilemap
    tile_map_material = material:new()
    tile_map_material.shader = "resources/Shaders/sprite"
    this_sceane.tile_map_info = load_2D_map(this_sceane.objects_layesrs.cenary,Vec3:new(0,0,0),Vec3:new(0,90,0),Vec3:new(0.1,0.1,0.1),"resources/Leveis 2D/tilemaps/tilemap.json","resources/Leveis 2D/tilesets/tileset.json","resources/Leveis 2D/tilesets",tile_map_material)
    this_sceane.tile_map_info.map_object.components[components.render_tile_map].render_tilemap_only_layer = -1
    this_sceane.tile_map_info.map_object.components[components.render_tile_map]:set()

    --this_sceane.tile_map_info.tile_map_info
    --this_sceane.tile_map_info.tile_set_info

    print(this_sceane.tile_map_info.tile_map_info.layers)

    tile_map_info_size = {
        x=this_sceane.tile_map_info.tile_map_info.width,
        y=this_sceane.tile_map_info.tile_map_info.height,
        tile_x=this_sceane.tile_map_info.tile_map_info.tilewidth,
        tile_y=this_sceane.tile_map_info.tile_map_info.tileheight,
    }
    tile_map_info_map = {}
    for l_id,l in  ipairs(this_sceane.tile_map_info.tile_map_info.layers) do
        tile_map_info_map[l.name] = deepcopyjson(l)
    end

    
    tile_set_info_map = {}
    for t_id,t in ipairs(this_sceane.tile_map_info.tile_set_info.tiles) do
        tile_set_info_map[t.id] = deepcopyjson(t)
    end
    


    
end

function sceanes_db.test:unload()
    print("unloading test")
    this_sceane.background:remove()
    this_sceane.background = nil

    this_sceane.camera:remove()
    this_sceane.camera = nil

    this_sceane.tile_map_info.map_object:remove()
    this_sceane.tile_map_info = nil
end


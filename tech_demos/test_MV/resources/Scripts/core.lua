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
require("TMP_libs.short_cuts.fps_counter")
require("TMP_libs.short_cuts.create_render_shader")

require("TMP_libs.material")
require("all_sceanes")









function set_res()
    window.res.x = 720
    window.res.y = 720
    window:set()
end




function START()
    
	print("core iniciando")
    set_res()

    
    --objects_layesrs:create()
    
    
    
    
    
    sceanes_db["test"]:load()
    this_sceane:unload()

    print("divisoria")
    sceanes_db["test"]:load()
   
    
    
    
    
    
    
    
    

end



function UPDATE()
    
    
end

function COLIDE(Colision)
end

function END()
end







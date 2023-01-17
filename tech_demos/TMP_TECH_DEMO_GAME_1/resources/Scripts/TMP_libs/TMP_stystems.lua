require("TMP_libs.TMP_functions")
require("TMP_libs.TMP_core")

json = require "libs.json"

gravity = {x = 0,y = 0, z = 0}
function gravity:get()
    self.x,self.y,self.z = get_gravity()
end
function gravity:set()
    set_gravity(self.x,self.y,self.z)
end

window = {res = {x = 256,y = 224},full_screen = false}
function window:get()
    self.res.x,self.res.y,self.full_screen = get_window()
end
function window:set()
    set_window(self.res.x,self.res.y,self.full_screen)
end



pos_processing = {}
pos_processing.shader = ""
pos_processing.color = {}
pos_processing.color.r = 1
pos_processing.color.g = 1
pos_processing.color.b = 1
pos_processing.color.a = 1
pos_processing.position_scale = {}
pos_processing.position_scale.x = 0
pos_processing.position_scale.y = 0
pos_processing.position_scale.z = 1
pos_processing.position_scale.w = 1
pos_processing.textures = {}
pos_processing.inputs = {}
function pos_processing:get()
        j = json.decode(get_post_processing_json())
        self.shader = j.shader
        self.color = deepcopy(j.color)
        self.textures = deepcopy(j.textures)
        self.inputs = deepcopy(j.inputs)
end

function pos_processing:set()  
        j = {}
        j.shader = self.shader
        j.color = deepcopy(self.color)
        j.position_scale = deepcopy(self.position_scale)
        j.textures = deepcopy(self.textures)
        j.inputs = deepcopy(self.inputs)
        set_post_processing_json(json.encode(j))
end

render_layer_instructions = {}
function render_layer_instructions:new()
    ret = {}
    ret.camera_selected = 0
    ret.start_render = true
    ret.clean_color = true
    ret.clean_deph = true
    ret.enable = true
    ret.end_render = true
    ret.use_deeph = true
    
    return ret
end

renders_layers_instructions = {}
renders_layers_instructions.layers_size = 0
renders_layers_instructions.layers = {render_layer_instructions:new()}
function renders_layers_instructions:get()
    j = json.decode(get_render_layer_instruction_json())
    self.layers = deepcopy(j["layers"])
end
function renders_layers_instructions:set()
    layers = {layers = {}}
    for key1, value1 in pairs(self.layers) do
        layers.layers[key1] = {}
        for key2, value2 in pairs(value1) do
            layers.layers[key1][key2] = value2
        end
    end

    j = json.encode(layers)
    set_render_layer_instruction_json(j)
end


function get_post_processing_json()
end
function set_post_processing_json(json)
end
function get_set_render_layer_instruction()
    
end

render_layer = {}
function render_layer:new()
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

renders_layers = {}
renders_layers.layers_size = 0
renders_layers.layers = {render_layer:new()}
function renders_layers:get()
    j = json.decode(get_render_layer_instruction_json())
    self.layers = deepcopy(j["layers"])
end
function renders_layers:set()
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
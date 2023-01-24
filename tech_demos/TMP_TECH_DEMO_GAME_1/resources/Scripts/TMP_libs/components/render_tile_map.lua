require("TMP_libs.components.component_index")


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
    end
    function rtm:set()
        j = {}
        j.layer = self.layer
        j.material = deepcopyjson(self.material)
        j.render_tilemap_only_layer = self.render_tilemap_only_layer
        j.tile_set_local = self.tile_set_local
        j.tile_map_local = self.tile_map_local
        set_render_tilemap_json(rtm.object_ptr,json.encode(j))
        
    end
    function rtm:delet()
        self.material:delet()
        self = nil
    end
    return rtm
end
component_map[components.render_tile_map] = render_tile_map_component
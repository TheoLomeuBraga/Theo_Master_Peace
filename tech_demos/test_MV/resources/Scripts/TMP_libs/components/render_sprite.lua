require("TMP_libs.components.component_index")

--sprite
function set_tileset(object,tileset)
end
function get_tileset(object)
end

function get_sprite_render_json(object)
end
function set_sprite_render_json(object,json)
end

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

    end
    function rs:set()
        j = {}
        j.layer = self.layer
        j.selected_tile = self.selected_tile
        j.tile_set_local = self.tile_set_local
        j.material = deepcopyjson(self.material)

        set_sprite_render_json(self.object_ptr,json.encode(j))

    end
    function rs:delet()
        
    end
    return rs
end
component_map[components.render_sprite] = render_sprite_component
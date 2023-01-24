require("TMP_libs.components.component_index")

colision_layer_info = {}
function colision_layer_info:new()
    cli = {}
    cli.layer = 1
    cli.layers_can_colide = {}
    cli.layers_can_colide[0] = 0
    return cli
end

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
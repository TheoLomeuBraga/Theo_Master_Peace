
require("TMP_libs.definitions")
require("TMP_libs.components.component_index")
layers_table = {}
function layers_table:new_2D()
    ret = {}
    ret.camera = ""
    ret.background_image = ""
    ret.background_3D = ""
    ret.background_cenary = ""
    ret.cenary = ""
    ret.charter = ""
    ret.top = ""
    ret.hud = ""
    ret.sound = ""
    function ret:create()
        self.camera = create_object()
        self.background_image = create_object()
        self.background_3D = create_object()
        self.background_cenary = create_object()
        self.cenary = create_object()
        self.charter = create_object()
        self.top = create_object()
        self.hud = create_object()
        self.sound = create_object()
    end
    function ret:destroy()
        remove_object(self.camera)
        remove_object(self.background_image)
        remove_object(self.background_3D)
        remove_object(self.background_cenary)
        remove_object(self.cenary)
        remove_object(self.charter)
        remove_object(self.top)
        remove_object(self.hud)
        remove_object(self.sound)
    end
    return ret
end
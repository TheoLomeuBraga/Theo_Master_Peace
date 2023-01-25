
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
    function ret:create()
        self.camera = create_object()
        self.background_image = create_object()
        self.background_3D = create_object()
        self.background_cenary = create_object()
        self.cenary = create_object()
        self.charter = create_object()
        self.top = create_object()
        self.hud = create_object()
    end
    return ret
end
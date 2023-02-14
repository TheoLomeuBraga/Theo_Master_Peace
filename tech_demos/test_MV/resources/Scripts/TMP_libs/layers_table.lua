
require("TMP_libs.definitions")
require("TMP_libs.components.component_index")
layers_table = {}
function layers_table:new_2D()
    ret = {}
    ret.main = ""
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
        self.main = create_object()
        self.camera = create_object(self.main)
        self.background_image = create_object(self.main)
        self.background_3D = create_object(self.main)
        self.background_cenary = create_object(self.main)
        self.cenary = create_object(self.main)
        self.charter = create_object(self.main)
        self.top = create_object(self.main)
        self.hud = create_object(self.main)
        self.sound = create_object(self.main)
    end
    function ret:destroy()
        remove_object(self.main)
        
    end
    return ret
end
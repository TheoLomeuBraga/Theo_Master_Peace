require("TMP_libs.functions")
require("TMP_libs.definitions")
require("TMP_libs.components.component_index")

json = require "libs.json"

gravity = {x = 0,y = 0, z = 0}
function gravity:get()
    self.x,self.y,self.z = get_gravity()
end
function gravity:set()
    set_gravity(self.x,self.y,self.z)
end

function get_set_window(get_set,object)
end
 
window = {resolution = {x = 256,y = 224},full_screen = false}
function window:get()
    --self.resolution.x,self.resolution.y,self.full_screen = get_window()
    new_window = get_set_window(get_lua)
    self.resolution.x = new_window.resolution.x
    self.resolution.y = new_window.resolution.y
    self.full_screen = new_window.full_screen
end
function window:set()
    --set_window(self.resolution.x,self.resolution.y,self.full_screen)
    get_set_window(set_lua,deepcopyjson(sel))
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
pos_processing.texturesolution = {}
pos_processing.inputs = {}
function pos_processing:get()
        j = json.decode(get_post_processing_json())
        self.shader = j.shader
        self.color = deepcopy(j.color)
        self.texturesolution = deepcopy(j.texturesolution)
        self.inputs = deepcopy(j.inputs)
end

function pos_processing:set()  
        j = {}
        j.shader = self.shader
        j.color = deepcopy(self.color)
        j.position_scale = deepcopy(self.position_scale)
        j.texturesolution = deepcopy(self.texturesolution)
        j.inputs = deepcopy(self.inputs)
        set_post_processing_json(json.encode(j))
end






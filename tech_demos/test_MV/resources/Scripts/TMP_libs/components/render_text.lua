require("TMP_libs.components.component_index")

--text

function set_font(object,font)
end
function get_font(object)
end
function set_text(object,text)
end
function get_text(object)
end

function get_text_json(object)
end
function set_text_json(object,json)
end

render_text_component = {}
function render_text_component:new(object_ptr)
    rs = {}
    rs.object_ptr = object_ptr
    rs.layer = 1
    rs.font = ""
    rs.text = ""
    rs.line_size = 34
    rs.space_betwen_chars = 0
    rs.max_space_betwen_chars = 3
    rs.min_space_betwen_chars = 0
    rs.material = matreial:new()
    function rs:get()

        j = {}
        j = deepcopyjson(json.decode(get_text_json(self.object_ptr)))

        self.layer = j.layer
        self.font = j.font
        self.text = j.text
        self.line_size = j.line_size
        self.space_betwen_chars = j.space_betwen_chars
        self.max_space_betwen_chars = j.max_space_betwen_chars
        self.min_space_betwen_chars = j.min_space_betwen_chars
        self.material = j.material

        --[[
        self.text = get_text(self.object_ptr)
        --layer
        self.layer = get_render_layer(self.object_ptr)
        --material
        self.material:get(self.object_ptr,0)
        self.font = get_font(self.object_ptr)
        ]]
    end
    function rs:set()

        j = {}

        j.layer = self.layer
        j.font = self.font
        j.text = self.text
        j.line_size = self.line_size
        
        j.space_betwen_chars = self.space_betwen_chars
        j.max_space_betwen_chars = self.max_space_betwen_chars
        j.min_space_betwen_chars = self.min_space_betwen_chars
        j.material = deepcopyjson(self.material)

        set_text_json(self.object_ptr,json.encode(j))
        
        --[[
        set_text(self.object_ptr,self.text)
        --layer
        set_render_layer(self.object_ptr,self.layer)
        --material
        self.material:set(self.object_ptr,0)
        set_font(self.object_ptr,self.font)
        ]]

    end
    function rs:delet()
    end
    return rs
end
component_map[components.render_text] = render_text_component
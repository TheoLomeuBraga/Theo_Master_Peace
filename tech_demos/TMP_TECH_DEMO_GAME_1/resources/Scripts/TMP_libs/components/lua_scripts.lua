require("TMP_libs.components.component_index")


lua_scripts_component = {}
function lua_scripts_component:new(object_ptr)
    ls = {}
    ls.object_ptr = object_ptr
    ls.scripts = {}
    function ls:get()
        self.scripts = nil
        self.scripts = {}
        i = 0
        while i < get_script_size(self.object_ptr) do
            self.scripts[i] = get_script_name(self.object_ptr,i)
            i = i + 1
        end
    end
    function ls:set()
    end
    function ls:add_script(script_name)
        add_script_lua(self.object_ptr,script_name)
    end
    function ls:remove_script(script_name)
        remove_script(self.object_ptr,script_name)
    end
    function ls:get_variable(script_name,variable_type,variable_name)
        return get_script_var(self.object_ptr,script_name,variable_name,variable_type)
    end
    function ls:set_variable(script_name,variable_name,variable_type,variable_value)
        set_script_var(self.object_ptr,script_name,variable_name,variable_type,variable_value)
    end
    function ls:call_function(script_name,function_name)
        call_script_function(self.object_ptr,script_name,function_name)
    end
    
    function ls:delet()
        self.scripts = nil
        self = nil
    end
    return ls
end
component_map[components.lua_scripts] = lua_scripts_component
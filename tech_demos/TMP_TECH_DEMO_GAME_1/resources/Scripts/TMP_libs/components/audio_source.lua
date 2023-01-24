require("TMP_libs.components.component_index")


audio_component = {}
function audio_component:new(object_ptr)
    a = {}
    a.object_ptr = object_ptr
    a.path = ""
	a.pause = false
	a.loop = false
	a.time = 0.001
	a.speed = 1
    a.volume = 5

    function a:set()
        set_audio(self.object_ptr,self.path,self.pause,self.loop,self.time,self.speed,self.volume)
    end
    function a:get()
        self.path,self.pause,self.loop,self.time,self.speed,self.volume = get_audio(self.object_ptr)
    end
    function a:delet()
        self = nil
    end

    return a
end
component_map[components.audio_source] = audio_component
require("TMP_libs.components.component_index")



--audio
function set_audio(object,path,pause,loop,time,speed,volume,min_distance,atenuation)
end
function get_audio(object)
end
function set_lisener_object(object)
end

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
    a.min_distance = 1
    a.atenuation = 10

    function a:set()
        set_audio(self.object_ptr,self.path,self.pause,self.loop,self.time,self.speed,self.volume,self.min_distance,self.atenuation)
    end
    function a:get()
        self.path,self.pause,self.loop,self.time,self.speed,self.volume,self.min_distance,self.atenuation = get_audio(self.object_ptr)
    end
    function a:delet()
        self = nil
    end

    return a
end
component_map[components.audio_source] = audio_component
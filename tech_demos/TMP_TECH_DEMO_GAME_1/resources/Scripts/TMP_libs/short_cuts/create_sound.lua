

function create_audio(music_path,loop,volume)
    ret = game_object:new(create_object()) 
    ret:add_component(components.audio_source)
    ret.components[components.audio_source].path = music_path
    ret.components[components.audio_source].loop = loop
    ret.components[components.audio_source].volume = volume
    ret.components[components.audio_source]:set()
    return ret
end


function create_3D_audio(father,pos,rot,sca,music_path,loop,volume)
    ret = game_object:new(create_object(father)) 
    ret:add_component(components.transform)
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.transform].scale = deepcopy(sca)
    ret.components[components.transform]:set()
    ret:add_component(components.audio_source)
    ret.components[components.audio_source].path = music_path
    ret.components[components.audio_source].loop = loop
    ret.components[components.audio_source].volume = volume
    ret.components[components.audio_source]:set()
    return ret
end


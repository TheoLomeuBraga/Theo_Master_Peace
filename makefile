compiler=g++

engine_cpp_windows_path=./src/Theo_Master_Peace/EngineTMasterPece.cpp
engine_cpp_linux_path=./src/Theo_Master_Peace/TMP_LINUX.cpp

font_reader_cpp_path=./src/Font_Reader/leitor_de_fontes_TMP_linux.cpp

tags=-std=c++20 -pipe

includes=-I./src/Font_Reader -I./src/Theo_Master_Peace -I./src/Theo_Master_Peace_Core -I./include -I./include/freetype

libs_path_windows=-L./libs/windows

libs_path_linux=-L./libs/linux

libs_windows=

libs_linux=-llua -lglfw -lGLEW -lglut -lGL -lGLU -lfreetype -lbox2d -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

built_path=./built

windows:
	${compiler} ${tags} ${engine_cpp_windows_path}  ${includes} ${libs_path_windows} ${libs_windows} -o ${built_path}/engine_theo_master_peace.exe 
	${compiler} ${tags} ${font_reader_cpp_path}  ${includes} ${libs_path_windows} ${libs_windows} -o ${built_path}/font_reader.exe
	
linux:
	${compiler} ${tags} ${engine_cpp_linux_path}  ${includes} ${libs_path_linux} ${libs_linux} -o ${built_path}/engine_theo_master_peace 
	${compiler} ${tags} ${font_reader_cpp_path}  ${includes} ${libs_path_linux} ${libs_linux} -o ${built_path}/font_reader
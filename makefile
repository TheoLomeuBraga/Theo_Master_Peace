compiler_windows=g++
compiler_linux=g++

engine_cpp_windows_path=./src/Theo_Master_Peace/EngineTMasterPece.cpp
engine_cpp_linux_path=./src/Theo_Master_Peace/TMP_LINUX.cpp

font_reader_cpp_path=./src/Font_Reader/leitor_de_fontes_TMP_linux.cpp

tags=-std=c++17 -pipe -Wa,-mbig-obj

includes=-I./src/Font_Reader -I./src/Theo_Master_Peace -I./src/Theo_Master_Peace_Core -I./include -I./include/freetype

built_path=./built



libs_path_windows=-L./libs/windows

libs_windows=-llua54 -lglfw3dll -lglew32mx.dll -lbox2d.dll -lfreetype -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system

windows:
	${compiler_windows} ${tags} ${engine_cpp_windows_path}  ${includes} ${libs_path_windows} ${libs_windows} -o ${built_path}/engine_theo_master_peace.exe 
	${compiler_windows} ${tags} ${font_reader_cpp_path}  ${includes} ${libs_path_windows} ${libs_windows} -o ${built_path}/font_reader.exe
#

libs_path_linux=-L./libs/linux

libs_linux=-llua -lglfw -lGLEW -lglut -lGL -lGLU -lfreetype -lbox2d -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

linux:
	${compiler_linux} ${tags} ${engine_cpp_linux_path}  ${includes} ${libs_path_linux} ${libs_linux} -o ${built_path}/engine_theo_master_peace 
	${compiler_linux} ${tags} ${font_reader_cpp_path}  ${includes} ${libs_path_linux} ${libs_linux} -o ${built_path}/font_reader
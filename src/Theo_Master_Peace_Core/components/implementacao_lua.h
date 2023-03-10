#pragma once

#include "RecursosT.h"
#include "ManusearArquivos.h"
#include "LoopPrincipal.h"
#include "input.h"
#include "Tempo.h"
using namespace Tempo;
#include "sceane.h"
#include "box_2d.h"
#include "sfml_audio.h"
#include "lua/lua.hpp"
#include "components/render_mesh.h"
#include "projetil.h"
#include "components/render_sprite.h"
#include "components/render_tilemap.h"
#include "components/reder_marching_cubes.h"

#include "args.h"
#include "game_object.h"



#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

#include "table.h"
#include "table_conversors.h"

const int get_lua = 0;
const int set_lua = 1;

bool isNumber(const std::string& str) {
    std::istringstream iss(str);
    double num;
    iss >> std::noskipws >> num;
    return iss.eof() && !iss.fail();
}

float stringToFloat(const std::string& str) {
    std::istringstream iss(str);
    float result;
    iss >> result;
    return result;
}


std::string floatToString(float value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}





void lua_pushtable(lua_State* L, Table t){
    lua_newtable(L);
    for(std::pair<std::string, float> p : t.m_floatMap){
        if(isNumber(p.first)){
            lua_pushnumber(L, stringToFloat(p.first));
        }else{
            lua_pushstring(L, p.first.c_str());
        }
        lua_pushnumber(L,p.second);
        lua_settable(L, -3);
    }
    for(std::pair<std::string, std::string> p : t.m_stringMap){
        if(isNumber(p.first)){
            lua_pushnumber(L, stringToFloat(p.first));
        }else{
            lua_pushstring(L, p.first.c_str());
        }
        lua_pushstring(L,p.second.c_str());
        lua_settable(L, -3);
    }
    for(std::pair<std::string, Table> p : t.m_tableMap){
        if(isNumber(p.first)){
            lua_pushnumber(L, stringToFloat(p.first));
        }else{
            lua_pushstring(L, p.first.c_str());
        }
        lua_pushtable(L,p.second);
        lua_settable(L, -3);
    }
    

    
}



Table lua_totable(lua_State* L,int index){
    Table t;
    // Make sure the argument at tableIndex is a table
    luaL_checktype(L, index, LUA_TTABLE);

    // Iterate over the table and extract its keys and values
    lua_pushnil(L);  // Push the first key
    while (lua_next(L, index) != 0) {
        std::string key;
		
        // At this point, the stack contains the key at index -2 and the value at index -1
        
       
		 if (lua_type(L, -2) == LUA_TNUMBER) {
            // The value is a number
            float value = lua_tonumber(L, -2);
			key = floatToString(value);
            
        }
		else if (lua_type(L, -2) == LUA_TSTRING) {
            // The value is a number
            std::string value = lua_tostring(L, -2);
			key = value;
            
        }
        else if (lua_type(L, -2) == LUA_TBOOLEAN) {
            // The value is a number
            float value = lua_toboolean(L, -2);
            key = std::to_string(value);
        }
		
        
		if (lua_type(L, -1) == LUA_TNUMBER) {
            // The value is a number
            float value = lua_tonumber(L, -1);
			t.setFloat(key,value);
            
        }
        else if (lua_type(L, -1) == LUA_TSTRING) {
            // The value is a number
            std::string value = lua_tostring(L, -1);
			t.setString(key,value);
            
        }
        else if (lua_type(L, -1) == LUA_TBOOLEAN) {
            // The value is a number
            float value = lua_toboolean(L, -1);
            t.setFloat(key,value);
        }
        else if (lua_type(L, -1) == LUA_TTABLE) {
            // The value is a table, recurse into it
            t.setTable(key,lua_totable(L, lua_gettop(L)));
        }

        // Pop the value, but leave the key for the next iteration
        lua_pop(L, 1);
    }
    return t;
}











using lua_function = int(*)(lua_State*);

mapeamento_assets<string> mapeamento_scripts_lua;

shared_ptr<string> carregar_script_lua(string local) {
	if (mapeamento_scripts_lua.pegar(local) == NULL) {
		mapeamento_scripts_lua.aplicar(local, ManuseioDados::Carregar_string(local));
	}
	return mapeamento_scripts_lua.pegar(local);
	return NULL;
}

void carregar_script_lua_thread(string local, shared_ptr<string>* ret) {
	*ret = carregar_script_lua(local);
}

json material_json(Material mat){
	vector<string> textures;
	for (shared_ptr<imagem> i : mat.texturas) {
		if (i != NULL) {
			textures.push_back(i->local);
		}
		else
		{
			textures.push_back("");
		}
	}

	vector<float> inputs;
	for (float i : mat.inputs) {
		inputs.push_back(i);
	}

	json JSON = {
		{"shader",mat.shad},
		{"color",{{"r",mat.cor.x},{"g",mat.cor.y},{"b",mat.cor.z},{"a",mat.cor.w}}},
		{"position_scale",{{"x",mat.uv_pos_sca.x},{"y",mat.uv_pos_sca.y},{"z",mat.uv_pos_sca.z},{"w",mat.uv_pos_sca.w}}},
		{"metallic",mat.metalico},
		{"softness",mat.suave},
		{"textures",textures},
		{"inputs",inputs},
		
	};
	return JSON;
}

Material json_material(json JSON) {
	
	Material ret = Material();
	json color = JSON["color"].get<json>(), position_scale = JSON["position_scale"].get<json>();
	vector<string> textures = JSON["textures"].get<vector<string>>();
	vector<float> inputs = JSON["inputs"].get<vector<float>>();
	
	
	ret.shad = JSON["shader"].get<string>();
	ret.cor = vec4(color["r"].get<float>(), color["g"].get<float>(), color["b"].get<float>(), color["a"].get<float>());
	ret.uv_pos_sca = vec4(position_scale["x"].get<float>(), position_scale["y"].get<float>(), position_scale["z"].get<float>(), position_scale["w"].get<float>());
	ret.metalico = JSON["metallic"].get<float>();
	ret.suave = JSON["softness"].get<float>();

	for (int i = 0; i < textures.size(); i++) {
		ret.texturas[i] = ManuseioDados::carregar_Imagem(textures[i]);
	}
	for (int i = 0; i < inputs.size(); i++) {
		ret.inputs[i] = inputs[i];
	}

	return ret;
}




//aqui






int add_component(lua_State* L);
int remove_component(lua_State* L);
int get_component_size(lua_State* L);
int have_component(lua_State* L);


//scripts
int get_script_size(lua_State* L);

int get_script_name(lua_State* L);

int have_script(lua_State* L);

int add_script_lua(lua_State* L);

int remove_script(lua_State* L);

int get_script_var(lua_State* L);

int set_script_var(lua_State* L);

int call_script_function(lua_State* L);

int get_lua_component(lua_State* L);




namespace funcoes_ponte {

	//exemplos get set
	/*
	int get_set_example(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;

			lua_pushtable(L,ret);
			return 1;
		}else{
			Table t = lua_totable(L,2);
			return 0;
		}
	}
	int get_set_transform(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));

			lua_pushtable(L,ret);
			return 1;
		}else{
			Table t = lua_totable(L,2);
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			
			return 0;
		}
	}
	*/

	int hello_world(lua_State* L) {
		cout << "hello world\n";
		int ret = 0;
		lua_pushinteger(L, ret);
		return ret;
	}

	//screen

	int get_set_window(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;
			ret.setTable("resolution",vec2_table(gerente_janela->pegar_res()));
			ret.setFloat("full_screen",gerente_janela->e_janela_cheia());
			lua_pushtable(L,ret);
			return 1;
		}else{
			Table t = lua_totable(L,2);
			Table res = t.getTable("resolution");
			loop_principal::mudar_res((int)res.getFloat("x"), (int)res.getFloat("y"));
			loop_principal::setar_tela_inteira_como((bool)t.getFloat("full_screen"));
			return 0;
		}
	}

	//time
	int get_time(lua_State* L) {
		lua_pushnumber(L, Tempo::tempo);
		lua_pushnumber(L, Tempo::varTempRender);
		lua_pushnumber(L, Tempo::velocidadeTempo);
		return 3;
	}

	int set_time_scale(lua_State* L) {
		Tempo::velocidadeTempo = lua_tointeger(L, 1);
		return 0;
	}

	//object
	int create_object(lua_State* L) {
		int argumentos = lua_gettop(L);
		shared_ptr<objeto_jogo> obj = novo_objeto_jogo();
		if (argumentos == 0) {
			cena_objetos_selecionados->adicionar_objeto(obj);
		}
		else if (argumentos == 1) {
			cena_objetos_selecionados->adicionar_objeto(string_ponteiro<objeto_jogo>(lua_tostring(L, 1)), obj);
		}


		lua_pushstring(L, ponteiro_string<objeto_jogo>(obj.get()).c_str());
		return 1;
	}

	int get_object_with_name(lua_State* L) {
		string output = "";
		shared_ptr<objeto_jogo> obj = (*cena_objetos_selecionados)[lua_tostring(L, 1)];
		output = ponteiro_string(obj.get());
		lua_pushstring(L, output.c_str());
		return 1;
	}

	int remove_object(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos == 1) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
			//remover_objeto();
			obj->excluir();
		}

		return 0;
	}

	int reset_components(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
			obj->limpar_componentes();
		}
		return 0;
	}

	

	int get_object_family_json(lua_State* L) {
		int argumentos = lua_gettop(L);
		string output = "";
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (obj != NULL) {
			vector<string> criancas;

			for (shared_ptr<objeto_jogo> p : obj->filhos) {
				criancas.push_back(ponteiro_string(p.get()));
			}

			json JSON = { 
				{"father",ponteiro_string(obj->pai)},
				{"childrens",criancas},
			};

			output = JSON.dump();
		}
		
		lua_pushstring(L, output.c_str());
		return 1;
	}

	int get_object_family(lua_State* L){
		Table ret;
		int argumentos = lua_gettop(L);
		string output = "";
		objeto_jogo* obj = NULL;
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));

		ret.setString("father",ponteiro_string(obj->pai));

		vector<string> criancas;
		for (shared_ptr<objeto_jogo> p : obj->filhos) {
			criancas.push_back(ponteiro_string(p.get()));
		}

		ret.setTable("childrens",vString_table(criancas));
		lua_pushtable(L,ret);
		return 1;
	}

	//memory
	int clear_memory(lua_State* L) {
		buffers_som_sfml.limpar_lixo();
		ManuseioDados::mapeamento_fontes.limpar_lixo();
		ManuseioDados::mapeamento_imagems.limpar_lixo();
		ManuseioDados::mapeamento_tilesets.limpar_lixo();
		ManuseioDados::mapeamento_tile_map_infos.limpar_lixo();
		ManuseioDados::cenas_3D.limpar_lixo();
		mapeamento_scripts_lua.limpar_lixo();
		return 0;
	}

	int load_asset(lua_State* L) {
		int argumentos = lua_gettop(L);
		if (argumentos == 2 && string(lua_tostring(L, 1)).compare("font") == 0) {
			ManuseioDados::carregar_fonte(lua_tostring(L, 2));
		}
		else if (argumentos == 2 && string(lua_tostring(L, 1)).compare("image") == 0) {
			ManuseioDados::carregar_Imagem(lua_tostring(L, 2));
		}
		else if (argumentos == 3 && string(lua_tostring(L, 1)).compare("tile_set") == 0) {
			ManuseioDados::carregar_tile_set(lua_tostring(L, 2));
		}
		else if (argumentos == 2 && string(lua_tostring(L, 1)).compare("tile_map") == 0) {
			ManuseioDados::carregar_info_tile_map(lua_tostring(L, 2));
		}
		else if (argumentos == 2 && string(lua_tostring(L, 1)).compare("audio") == 0) {
			carregar_audio_buffer_sfml(lua_tostring(L, 2));
		}

		return 0;
	}

	int asset_is_load(lua_State* L) {
		bool output = false;
		int argumentos = lua_gettop(L);
		if (argumentos == 2) {
			if (string(lua_tostring(L, 1)).compare("font") == 0) {
				if (ManuseioDados::mapeamento_fontes.pegar(lua_tostring(L, 2)) != NULL) { output = true; }
			}
			else if (string(lua_tostring(L, 1)).compare("image") == 0) {
				if (ManuseioDados::mapeamento_imagems.pegar(lua_tostring(L, 2)) != NULL) { output = true; }
			}
			else if (string(lua_tostring(L, 1)).compare("tile_set") == 0) {
				if (ManuseioDados::mapeamento_tilesets.pegar(lua_tostring(L, 2)) != NULL) { output = true; }
			}
			else if (string(lua_tostring(L, 1)).compare("tile_map") == 0) {
				if (ManuseioDados::mapeamento_tile_map_infos.pegar(lua_tostring(L, 2)) != NULL) { output = true; }
			}
			else if (string(lua_tostring(L, 1)).compare("audio") == 0) {
				if (buffers_som_sfml.pegar(lua_tostring(L, 2)) != NULL) { output = true; }
			}
		}
		lua_pushboolean(L, output);
		return 1;
	}

	//read assets

	int get_tile_set_size(lua_State* L) {
		int output = 0;
		int argumentos = lua_gettop(L);
		if (argumentos == 1) {
			output = ManuseioDados::carregar_tile_set(lua_tostring(L, 1))->tiles.size();
		}
		lua_pushnumber(L, output);
		return 1;
	}

	int get_tile_set_tile(lua_State* L) {
		tile output;
		int argumentos = lua_gettop(L);



		if (argumentos == 3 && (int)lua_tonumber(L, 3) > 0 && lua_tonumber(L, 3) < ManuseioDados::carregar_tile_set(lua_tostring(L, 1))->tiles.size()) {
			output = ManuseioDados::carregar_tile_set(lua_tostring(L, 1))->tiles[(int)lua_tonumber(L, 3)];
		}


		lua_pushnumber(L, output.sprite_id);
		lua_pushstring(L, output.nome.c_str());
		//lua_pushnumber(L, output.tipo_colisao);
		//lua_pushboolean(L, output.visivel);
		//lua_pushboolean(L, output.interativel);
		//lua_pushstring(L, output.script.c_str());
		//lua_pushstring(L, output.comentario.c_str());
		//return 7;
		return 3;
	}

	int get_tilemap_size(lua_State* L) {
		int argumentos = lua_gettop(L);
		vec2 output;
		if (argumentos == 1) {
			output = ManuseioDados::carregar_info_tile_map(lua_tostring(L, 1))->res;
		}
		lua_pushnumber(L, output.x);
		lua_pushnumber(L, output.y);
		return 2;
	}

	int get_tilemap_layer_size(lua_State* L) {
		int argumentos = lua_gettop(L);
		int output = 0;
		if (argumentos == 1) {
			output = ManuseioDados::carregar_info_tile_map(lua_tostring(L, 1))->info.size();
		}
		lua_pushnumber(L, output);
		return 1;
	}

	int get_tilemap_data(lua_State* L) {
		int argumentos = lua_gettop(L);
		int output = 0;
		if (argumentos == 3) {
			output = ManuseioDados::carregar_info_tile_map(lua_tostring(L, 1))->info[lua_tonumber(L, 2)][lua_tonumber(L, 3)];
		}
		lua_pushnumber(L, output);
		return 1;
	}

	//adicionar lua cena 3D
	int get_scene_3D_json(lua_State* L) {
		int argumentos = lua_gettop(L);
		string output = "";
		if (argumentos == 1) {
			output = converter_cena_3D_para_json(ManuseioDados::carregar_modelo_3D(lua_tostring(L, 1)));
		}
		
		lua_pushstring(L, output.c_str());
		return 1;
	}

	//input

	

	void push_input_joystick(lua_State* L, string nome, float precao) {
		lua_pushstring(L, nome.c_str());
		lua_pushnumber(L, precao);
		lua_settable(L, -3);
	}

	void push_input_mouse_keyboard(lua_State* L, string nome, bool precao) {
		lua_pushstring(L, nome.c_str());
		lua_pushboolean(L, precao);
		lua_settable(L, -3);
	}
	int set_cursor_position(lua_State* L) {
		int argumentos = lua_gettop(L);
		if (argumentos == 2) {
			gerente_janela->mudar_pos_cursor(lua_tonumber(L,1), lua_tonumber(L, 2));
		}
		return 0;
	}
	int get_keyboard_input(lua_State* L) {
		lua_newtable(L);
		{
			for (pair<string, bool> p : Teclado.teclas) {
				push_input_mouse_keyboard(L, p.first, p.second);
			}
		}
		return 1;
	}
	int get_mouse_input(lua_State* L) {
		lua_newtable(L);
		{
			push_input_mouse_keyboard(L, "L", MOUSE.botao[0]);
			push_input_mouse_keyboard(L, "R", MOUSE.botao[1]);
			push_input_mouse_keyboard(L, "scroll", MOUSE.botao[2]);
			push_input_joystick(L, "scroll_rotation", MOUSE.scrolRot);

			lua_pushstring(L, "cursos_position");
			lua_newtable(L);
			{
				push_input_joystick(L, "x", MOUSE.cursorPos[0]);
				push_input_joystick(L, "y", MOUSE.cursorPos[1]);
				lua_settable(L, -3);
			}
			
		}
		return 1;
	}
	int get_joystick_input(lua_State* L) {

		lua_newtable(L);
		{
			push_input_joystick(L, "A", JOYSTICK[1].botoes[0]);
			push_input_joystick(L, "B", JOYSTICK[1].botoes[1]);
			push_input_joystick(L, "X", JOYSTICK[1].botoes[2]);
			push_input_joystick(L, "Y", JOYSTICK[1].botoes[3]);

			push_input_joystick(L, "RB", JOYSTICK[1].botoes[4]);
			push_input_joystick(L, "RT", JOYSTICK[1].botoes[5]);
			push_input_joystick(L, "LB", JOYSTICK[1].botoes[6]);
			push_input_joystick(L, "RT", JOYSTICK[1].botoes[7]);

			push_input_joystick(L, "START", JOYSTICK[1].botoes[8]);
			push_input_joystick(L, "BACK", JOYSTICK[1].botoes[9]);


			lua_pushstring(L, "D_PAD");
			lua_newtable(L);
			{
				push_input_joystick(L, "x", JOYSTICK[1].D_PAD.x);
				push_input_joystick(L, "y", JOYSTICK[1].D_PAD.y);
				

				lua_settable(L, -3);
			}

			lua_pushstring(L,"analogL");
			lua_newtable(L);
			{
				push_input_joystick(L, "x", JOYSTICK[1].analogio[0].x);
				push_input_joystick(L, "y", JOYSTICK[1].analogio[0].y);

				lua_settable(L, -3);
			}

			lua_pushstring(L, "analogR");
			lua_newtable(L);
			{
				push_input_joystick(L, "x", JOYSTICK[1].analogio[1].x);
				push_input_joystick(L, "y", JOYSTICK[1].analogio[1].y);

				lua_settable(L, -3);
			}
			
			
		}
		

		return 1;
	}

	int set_keyboard_text_input(lua_State* L) {
		int argumentos = lua_gettop(L);
		if (argumentos == 1) {
			TECLADO.pegar_input_texto = lua_toboolean(L, 1);
		}
		return 0;
	}
	int get_inputs(lua_State* L){
		return 0;
	}


	//char control

	int to_move(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (obj != NULL) {
			if (argumentos == 3) {
				shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
				if (b2d != NULL) {
					b2d->mover(vec2(lua_tonumber(L, 2), lua_tonumber(L, 3)));
				}
			}
		}
		return 0;
	}

	int in_ceiling(lua_State* L) {
		bool output = false;
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj->pegar_componente<fisica_char_B2D>() != NULL) {
			shared_ptr<fisica_char_B2D> fc = obj->pegar_componente<fisica_char_B2D>();
			if (fc != NULL) {
				output = fc->teto;
			}
		}

		lua_pushboolean(L, output);
		return 1;
	}

	int in_floor(lua_State* L) {
		bool output = false;
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj->pegar_componente<fisica_char_B2D>() != NULL) {
			shared_ptr<fisica_char_B2D> fc = obj->pegar_componente<fisica_char_B2D>();
			if (fc != NULL) {
				output = fc->chao;
			}
		}

		lua_pushboolean(L, output);
		return 1;
	}

	//gravity

	int get_gravity(lua_State* L) {
		lua_pushnumber(L, gravidade.x);
		lua_pushnumber(L, gravidade.y);
		lua_pushnumber(L, gravidade.z);
		return 3;
	}

	int set_gravity(lua_State* L) {
		int argumentos = lua_gettop(L);
		if (argumentos == 3) {
			gravidade = vec3(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3));
		}
		return 0;
	}

	//transform
	int get_set_transform(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
			ret.setFloat("is_ui",tf->UI);
			ret.setTable("position",vec3_table(tf->pos) );
			ret.setTable("rotation",vec3_table(tf->pegar_angulo_graus()));
			ret.setTable("scale",vec3_table(tf->esca));
			lua_pushtable(L,ret);
			return 1;
		}else{
			Table t = lua_totable(L,2);
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
			if(tf != NULL){
				tf->UI = t.getFloat("is_ui");
				tf->pos = table_vec3(t.getTable("position"));
				tf->mudar_angulo_graus(table_vec3(t.getTable("rotation")));
				tf->esca = table_vec3(t.getTable("scale"));
			}
			
			return 0;
		}
	}

	int move_transform(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
		if (tf != NULL) {
			vec3 v3 = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			tf->pos += v3;
		}
		return 0;
	}

	int rotate_transform(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
		if (tf != NULL) {
			
			vec3 v3a = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4)),v3b = tf->pegar_angulo_graus(),v3c = vec3(v3a.x + v3b.x, v3a.y + v3b.y, v3a.z + v3b.z);
			tf->mudar_angulo_graus(v3c);
		}
		return 0;
	}

	int change_transfotm_position(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
		if (tf != NULL) {
			vec3 v3 = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			tf->pos = v3;
		}
		shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
		if (b2d != NULL) {
			vec3 v3 = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			b2d->mudar_pos(vec2(v3.x,v3.y));
		}
		return 0;
	}

	int change_transfotm_rotation(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
		if (tf != NULL) {
			vec3 v3 = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			tf->mudar_angulo_graus(v3);
		}
		shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
		if (b2d != NULL) {
			vec3 v3 = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			b2d->mudar_rot(v3.x);
		}
		return 0;
	}

	int change_transfotm_scale(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		shared_ptr<transform_> tf = obj->pegar_componente<transform_>();
		if (tf != NULL) {
			vec3 v3 = vec3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			tf->esca = v3;
		}
		return 0;
	}

	
	//sprite render

	int get_set_sprite_render(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<render_sprite> rs = obj->pegar_componente<render_sprite>();
			ret.setFloat("layer",rs->camada);
			ret.setFloat("selected_tile",rs->tile_selecionado);
			ret.setString("tile_set_local",rs->tiles->local);
			ret.setTable("material",material_table(rs->mat));
			lua_pushtable(L,ret);
			return 1;
		}else{
			Table t = lua_totable(L,2);
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<render_sprite> rs = obj->pegar_componente<render_sprite>();
			rs->camada = t.getFloat("layer");
			rs->tile_selecionado = t.getFloat("selected_tile");
			rs->tiles = ManuseioDados::carregar_tile_set( t.getString("tile_set_local"));
			rs->mat = table_material(t.getTable("material"));
			return 0;
		}
	}


	//render tilemap


	int get_tilemap(lua_State* L) {
		int argumentos = lua_gettop(L);
		string output = "";
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 1 && obj != NULL && obj->pegar_componente<render_tilemap>() != NULL && obj->pegar_componente<render_tilemap>()->map_info != NULL) {
			output = obj->pegar_componente<render_tilemap>()->map_info->local;
		}
		lua_pushstring(L, output.c_str());
		return 1;
	}

	int set_tilemap(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL && obj->pegar_componente<render_tilemap>() != NULL) {
			ManuseioDados::carregar_info_tile_map_thread(lua_tostring(L, 2), &obj->pegar_componente<render_tilemap>()->map_info);
			//obj->pegar_componente<render_tilemap>()->map_info = ManuseioDados::carregar_info_tile_map(lua_tostring(L, 2));
		}
		return 0;
	}

	int get_render_only_tilemap_layer(lua_State* L) {
		int argumentos = lua_gettop(L);
		int output = -1;
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL && obj->pegar_componente<render_tilemap>() != NULL) {
			output = obj->pegar_componente<render_tilemap>()->apenas_camada;
		}
		lua_pushnumber(L, output);
		return 1;
	}

	int set_render_only_tilemap_layer(lua_State* L) {
		int argumentos = lua_gettop(L);

		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 2 && obj != NULL && obj->pegar_componente<render_tilemap>() != NULL) {
			obj->pegar_componente<render_tilemap>()->apenas_camada = lua_tonumber(L, 2);
		}
		return 0;
	}



	

	int get_set_render_tilemap(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<render_tilemap> rtm = obj->pegar_componente<render_tilemap>();
			ret.setFloat("layer",rtm->camada);
			ret.setTable("material",material_table(rtm->mat));
			ret.setFloat("render_tilemap_only_layer",rtm->apenas_camada);
			ret.setString("tile_set_local",rtm->tiles->local);
			ret.setString("tile_map_local",rtm->map_info->local);
			lua_pushtable(L,ret);
			return 1;
		}else{
			Table t = lua_totable(L,2);
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<render_tilemap> rtm = obj->pegar_componente<render_tilemap>();
			rtm->camada = t.getFloat("layer");
			rtm->mat = table_material(t.getTable("material"));
			rtm->apenas_camada = t.getFloat("render_tilemap_only_layer");
			rtm->tiles = ManuseioDados::carregar_tile_set( t.getString("tile_set_local"));
			rtm->map_info = ManuseioDados::carregar_info_tile_map(t.getString("tile_map_local"));
			return 0;
		}
	}

	//texto
	int get_set_render_text(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<render_texto> rt = obj->pegar_componente<render_texto>();
			ret.setFloat("layer",rt->camada);
			ret.setString("font",rt->font->local);
			ret.setString("text",rt->texto);
			ret.setFloat("line_size",rt->tamanho_max_linha);
			ret.setFloat("space_betwen_chars",rt->espaco_entre_letras);
			ret.setFloat("max_space_betwen_chars",rt->espaco_entre_letras_max);
			ret.setFloat("min_space_betwen_chars",rt->espaco_entre_letras_min);
			ret.setTable("material",material_table(rt->mat));
			lua_pushtable(L,ret);
			return 1;
		}else{
			Table t = lua_totable(L,2);
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<render_texto> rt = obj->pegar_componente<render_texto>();
			rt->camada = t.getFloat("layer");
			rt->font = ManuseioDados::carregar_fonte(t.getString("font"));
			rt->texto = t.getString("text");
			rt->tamanho_max_linha = t.getFloat("line_size");
			rt->espaco_entre_letras = t.getFloat("space_betwen_chars");
			rt->espaco_entre_letras_max = t.getFloat("max_space_betwen_chars");
			rt->espaco_entre_letras_min = t.getFloat("min_space_betwen_chars");
			rt->mat = table_material(t.getTable("material"));
			return 0;
		}
	}

	int get_set_render_shader(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<render_shader> rt = obj->pegar_componente<render_shader>();
			ret.setFloat("layer",rt->camada);
			ret.setFloat("vertex_size",rt->tamanho);
			ret.setTable("material",material_table(rt->mat));
			lua_pushtable(L,ret);
			return 1;
		}else{
			Table t = lua_totable(L,2);
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<render_shader> rt = obj->pegar_componente<render_shader>();
			rt->camada = t.getFloat("layer");
			rt->tamanho = t.getFloat("vertex_size");
			rt->mat = table_material(t.getTable("material"));
			return 0;
		}
	}

	//fisica

	int add_force(lua_State* L) {
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		if (argumentos == 3 && obj != NULL) {
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			if (b2d != NULL) {
				b2d->adicionar_forca(vec2(lua_tonumber(L, 2), lua_tonumber(L, 3)));
			}
		}
		return 0;
	}

	

	

	int get_set_physic_2D(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			ret.setTable("scale",vec2_table(b2d->escala));
			ret.setFloat("boady_dynamic",b2d->dinamica);
			ret.setFloat("colision_shape",b2d->forma);
			ret.setFloat("rotate",b2d->rotacionar);
			ret.setFloat("triger",b2d->gatilho);
			ret.setFloat("friction",b2d->atrito);
			vector<string> objects_coliding;
			for(shared_ptr<objeto_jogo> obj : b2d->objs_colidindo){
				objects_coliding.push_back(ponteiro_string(obj.get()));
			}
			ret.setTable("objects_coliding",vString_table(objects_coliding));
			ret.setTable("colision_layer",info_camada_table(b2d->camada));
			vector<Table> vertex;
			for(vec2 v2 : b2d->vertices){
				vertex.push_back(vec2_table(v2));
			}
			ret.setTable("vertex",vTable_table(vertex));
			lua_pushtable(L,ret);
			return 1;
		}else{
			Table t = lua_totable(L,2);
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<box_2D> b2d = obj->pegar_componente<box_2D>();
			b2d->escala = table_vec2(t.getTable("scale"));
			b2d->dinamica = t.getFloat("boady_dynamic");
			b2d->forma = t.getFloat("colision_shape");
			b2d->rotacionar = t.getFloat("rotate");
			b2d->gatilho = t.getFloat("triger");
			b2d->atrito = t.getFloat("friction");
			b2d->camada = table_info_camada(t.getTable("colision_layer"));
			vector<vec2> vertex;
			for(Table tvec2 : table_vTable(t.getTable("vertex"))){
				vertex.push_back(table_vec2(tvec2));
			}
			b2d->vertices = vertex;
			b2d->aplicar();
			return 0;
		}
	}

	

	//camerareturn 

	int get_set_camera(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<camera> cam = obj->pegar_componente<camera>();
			ret.setFloat("orthographc",cam->ortografica);
			ret.setTable("size",vec2_table(cam->tamanho));
			ret.setFloat("zoom",cam->zoom);
			ret.setTable("resolution",vec2_table(cam->res));
			ret.setFloat("fcp",cam->fcp);
			ret.setFloat("ncp",cam->ncp);
			lua_pushtable(L,ret);
			return 1;
		}else{
			Table t = lua_totable(L,2);
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<camera> cam = obj->pegar_componente<camera>();
			if(t.getFloat("orthographc")){
				vec2 size = table_vec2( t.getTable("size"));
				cam->configurar_camera(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), size.x, size.y, t.getFloat("ncp"), t.getFloat("fcp"));
			}else{
				vec2 res = table_vec2( t.getTable("resolution"));
				cam->configurar_camera(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f),t.getFloat("zoom"),res.x,res.y,t.getFloat("ncp"), t.getFloat("fcp"));
			}
			return 0;
		}
	}

	//audio
	int get_set_audio(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<sfml_audio> au = obj->pegar_componente<sfml_audio>();
			ret.setString("path",au->info.nome);
			ret.setFloat("pause",au->info.pausa);
			ret.setFloat("loop",au->info.loop);
			ret.setFloat("time",au->info.tempo);
			ret.setFloat("speed",au->info.velocidade);
			ret.setFloat("volume",au->info.volume);
			ret.setFloat("min_distance",au->info.min_distance);
			ret.setFloat("atenuation",au->info.atenuation);
			lua_pushtable(L,ret);
			return 1;
		}else{
			Table t = lua_totable(L,2);
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<sfml_audio> au = obj->pegar_componente<sfml_audio>();
			
			au->info.nome = t.getString("path");
			au->info.pausa = t.getFloat("pause");
			au->info.loop = t.getFloat("loop");
			au->info.tempo = t.getFloat("time");
			au->info.velocidade = t.getFloat("speed");
			au->info.volume = t.getFloat("volume");
			au->info.min_distance = t.getFloat("min_distance");
			au->info.atenuation = t.getFloat("atenuation");
			au->aplicar_info();
			return 0;
		}
	}

	int set_lisener_object(lua_State* L){
		int argumentos = lua_gettop(L);
		objeto_jogo* obj = NULL;
		audio_info output;
		if (argumentos > 0) {
			obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
		}
		listener_transform = obj->pegar_componente<transform_>();
		return 0;
	}
	//geral render

	
	

	

	




	//post_processing
	int get_set_post_processing(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;
			lua_pushtable(L,material_table(api_grafica->pos_processamento_info));
			return 1;
		}else{
			Table t = lua_totable(L,2);
			api_grafica->pos_processamento_info = table_material(t);
			return 0;
		}
	}

	//render layers

	

	
	int get_set_render_layer_instruction(lua_State* L){
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;
			vector<Table> vt;
			for(instrucoes_render ir : api_grafica->info_render){
				vt.push_back(table_instrucoes_render(ir));
			}
			lua_pushtable(L,vTable_table(vt));
			return 1;
		}else{
			Table t = lua_totable(L,2);
			vector<Table> vt = table_vTable(t);
			vector<instrucoes_render> irs;
			for(Table t2 : vt){
				irs.push_back(table_instrucoes_render(t2));
			}
			api_grafica->info_render.clear();
			api_grafica->info_render = irs;
			return 0;
		}
	}
	

	

	int get_set_render_mesh(lua_State* L) {
		if(lua_tonumber(L, 1) == get_lua){
			Table ret;
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
			shared_ptr<render_malha> mesh = obj->pegar_componente<render_malha>();
			ret.setFloat("layer",mesh->camada);
			ret.setFloat("use_oclusion",mesh->usar_oclusao);
			ret.setFloat("normal_direction",mesh->lado_render);
			vector<Table> meshes;
			for(shared_ptr<malha> m : mesh->malhas){
				Table mesh_info;
				mesh_info.setString("file",m->arquivo_origem);
				mesh_info.setString("name",m->nome);
				meshes.push_back(mesh_info);
			}
			ret.setTable("meshes",vTable_table(meshes));
			vector<Table> materials;
			for(Material m : mesh->mats){
				materials.push_back(material_table(m));
			}
			ret.setTable("materials",vTable_table(materials));
			lua_pushtable(L,ret);
			return 1;
		}else{
			
			Table t = lua_totable(L,2);
			objeto_jogo* obj = string_ponteiro<objeto_jogo>(t.getString("object_ptr"));
			shared_ptr<render_malha> mesh = obj->pegar_componente<render_malha>();
			mesh->camada = t.getFloat("layer");
			mesh->usar_oclusao = t.getFloat("use_oclusion");
			mesh->lado_render = t.getFloat("normal_direction");
			vector<Table> vt = table_vTable(t.getTable("meshes"));
			vector<shared_ptr<malha>> meshes;
			for(Table mesh : vt){
				cout << "file: " << mesh.getString("file") << "name: " << mesh.getString("name") << endl;
				meshes.push_back(ManuseioDados::carregar_malha(mesh.getString("file"),mesh.getString("name")));
			}
			mesh->malhas = meshes;
			vt = table_vTable(t.getTable("materials"));
			vector<Material> materials;
			for(Table mat : vt){
				materials.push_back(table_material(mat));
			}
			mesh->mats = materials;
			return 0;
		}
	}


	
	

	map<string, lua_function> funcoes_ponte_map = {

		//input
		pair<string, lua_function>("get_keyboard_input", funcoes_ponte::get_keyboard_input),
		pair<string, lua_function>("get_mouse_input", funcoes_ponte::get_mouse_input),
		pair<string, lua_function>("get_joystick_input", funcoes_ponte::get_joystick_input),
		pair<string, lua_function>("set_cursor_position", funcoes_ponte::set_cursor_position),

		pair<string, lua_function>("set_keyboard_text_input", funcoes_ponte::set_keyboard_text_input),
		pair<string, lua_function>("get_inputs", funcoes_ponte::get_inputs),

		
		

		//tempo
		pair<string, lua_function>("get_time", funcoes_ponte::get_time),
		pair<string, lua_function>("set_time_scale", funcoes_ponte::set_time_scale),

		//memoria
		pair<string, lua_function>("clear_memory", funcoes_ponte::clear_memory),
		pair<string, lua_function>("load_asset", funcoes_ponte::load_asset),
		pair<string, lua_function>("asset_is_load", funcoes_ponte::asset_is_load),

		//assets
		pair<string, lua_function>("get_tile_set_size", funcoes_ponte::get_tile_set_size),
		pair<string, lua_function>("get_tile_set_tile", funcoes_ponte::get_tile_set_tile),
		pair<string, lua_function>("get_tilemap_size", funcoes_ponte::get_tilemap_size),
		pair<string, lua_function>("get_tilemap_layer_size", funcoes_ponte::get_tilemap_layer_size),
		pair<string, lua_function>("get_tilemap_data", funcoes_ponte::get_tilemap_data),

		pair<string, lua_function>("get_scene_3D_json", funcoes_ponte::get_scene_3D_json),
		

		//objeto
		pair<string, lua_function>("create_object", funcoes_ponte::create_object),
		pair<string, lua_function>("get_object_with_name", funcoes_ponte::get_object_with_name),
		pair<string, lua_function>("remove_object", funcoes_ponte::remove_object),
		pair<string, lua_function>("add_component", add_component),
		pair<string, lua_function>("remove_component", remove_component),
		pair<string, lua_function>("reset_components", funcoes_ponte::reset_components),
		pair<string, lua_function>("have_component", have_component),

		pair<string, lua_function>("get_object_family_json", funcoes_ponte::get_object_family_json),
		pair<string, lua_function>("get_object_family", funcoes_ponte::get_object_family),
		
		

		//movimento
		pair<string, lua_function>("to_move", funcoes_ponte::to_move),
		pair<string, lua_function>("in_ceiling", funcoes_ponte::in_ceiling),
		pair<string, lua_function>("in_floor", funcoes_ponte::in_floor),
		pair<string, lua_function>("get_gravity", funcoes_ponte::get_gravity),
		pair<string, lua_function>("set_gravity", funcoes_ponte::set_gravity),

		//transform
		pair<string, lua_function>("get_set_transform", funcoes_ponte::get_set_transform),

		pair<string, lua_function>("move_transform", funcoes_ponte::move_transform),
		pair<string, lua_function>("rotate_transform", funcoes_ponte::rotate_transform),

		pair<string, lua_function>("change_transfotm_position", funcoes_ponte::change_transfotm_position),
		pair<string, lua_function>("change_transfotm_rotation", funcoes_ponte::change_transfotm_rotation),
		pair<string, lua_function>("change_transfotm_scale", funcoes_ponte::change_transfotm_scale),

		//pos-procesing
		pair<string, lua_function>("get_set_post_processing", funcoes_ponte::get_set_post_processing),
		

		//camadas render
		pair<string, lua_function>("get_set_render_layer_instruction", funcoes_ponte::get_set_render_layer_instruction),
		

		//janela
		pair<string, lua_function>("get_set_window", funcoes_ponte::get_set_window),
		

		//sprite
		pair<string, lua_function>("get_set_sprite_render", funcoes_ponte::get_set_sprite_render),
		

		//render tilemap
		pair<string, lua_function>("get_set_render_tilemap", funcoes_ponte::get_set_render_tilemap),
		
		


		//text
		pair<string, lua_function>("get_set_render_text", funcoes_ponte::get_set_render_text),
		
		
		
		//shader
		pair<string, lua_function>("get_set_render_shader", funcoes_ponte::get_set_render_shader),
		
		
		//physic
		pair<string, lua_function>("get_set_physic_2D", funcoes_ponte::get_set_physic_2D),
		pair<string, lua_function>("add_force", funcoes_ponte::add_force),
		
		

		//camera
		pair<string, lua_function>("get_set_camera", funcoes_ponte::get_set_camera),

		//audio
		pair<string, lua_function>("get_set_audio", funcoes_ponte::get_set_audio),

		pair<string, lua_function>("set_lisener_object", funcoes_ponte::set_lisener_object),

		//mesh
		pair<string, lua_function>("get_set_render_mesh", funcoes_ponte::get_set_render_mesh),
		

		//script
		pair<string, lua_function>("get_script_size", get_script_size),
		pair<string, lua_function>("get_script_name", get_script_name),
		pair<string, lua_function>("have_script", have_script),
		pair<string, lua_function>("add_script_lua", add_script_lua),
		pair<string, lua_function>("remove_script", remove_script),
		pair<string, lua_function>("get_script_var", get_script_var),
		pair<string, lua_function>("set_script_var", set_script_var),
		pair<string, lua_function>("call_script_function", call_script_function),

		pair<string, lua_function>("get_lua_component", get_lua_component),
			
			
	};

};







namespace funcoes_lua {

	void adicionar_parte_funcoes_ponte_estado_lua(lua_State* L, vector<pair<string, lua_function>> funcoes, int inicio, int tamanho) {
		for (int i = inicio; i < tamanho; i++) {
			lua_register(L, funcoes[i].first.c_str(), funcoes[i].second);
		}
	}

	void adicionar_funcoes_ponte_estado_lua(lua_State* L) {
		vector<pair<string, lua_function>> funcoes_ponte_pair = map_vetor<string, lua_function>(funcoes_ponte::funcoes_ponte_map);



		//solu??????o 2
		//thread af(adicionar_parte_funcoes_ponte_estado_lua, L, funcoes_ponte_pair, 0, (int)funcoes_ponte_pair.size() / 2);
		//adicionar_parte_funcoes_ponte_estado_lua(L, funcoes_ponte_pair, (int)funcoes_ponte_pair.size() / 2, (int)funcoes_ponte_pair.size());
		//af.join();

		//solu??????o 1
		for (pair<string, lua_function > p : funcoes_ponte::funcoes_ponte_map) {
			lua_register(L, p.first.c_str(), p.second);
		}
	}



	void iniciar_estado_lua(lua_State* L) {


		//argumentos
		{
			lua_newtable(L);
			for (int i = 0; i < argumentos.size(); i++) {
				lua_pushinteger(L, i);
				lua_pushstring(L, argumentos[i].c_str());
				lua_settable(L, -3);
			}
			lua_setglobal(L, "args");

			lua_pushinteger(L, argumentos.size());
			lua_setglobal(L, "argsn");
		}

		//get_input
		{
			//controle & mouse
			{
				//lua_register(L, "get_input", funcoes_ponte::get_input);
			}
			//teclado
			{
				lua_getglobal(L, "get_keys_input");
				if (lua_toboolean(L, -1)) {
					lua_getglobal(L, "keys");
					//input texto
					lua_pushstring(L, "text_input");
					lua_pushstring(L, TECLADO.input_texto.c_str());
					lua_settable(L, -3);
					//input teclas
					for (pair<string, bool> p : TECLADO.teclas) {
						lua_pushstring(L, p.first.c_str());
						lua_pushboolean(L, p.second);
						lua_settable(L, -3);
					}
				}
			}
		}

		//af.join();

		funcoes_lua::adicionar_funcoes_ponte_estado_lua(L);

	}


	map<string, string> scripts_lua;
	void limpar_scripts_lua() {
		scripts_lua.clear();
	}

	lua_State* carregar_luaState(string s) {
		//criar
		lua_State* ret = luaL_newstate();
		luaL_openlibs(ret);


		//configurar diretorio
		string local = pegar_local_aplicacao() + "/resources/Scripts/?.lua";
		lua_getglobal(ret, "package");
		lua_pushstring(ret, local.c_str());
		lua_setfield(ret, -2, "path");
		lua_pop(ret, 1);


		//int i = luaL_dofile(ret,s.c_str());
		int i = luaL_dostring(ret, carregar_script_lua(s)->c_str());
		funcoes_lua::iniciar_estado_lua(ret);


		//buscar erros
		if (i != LUA_OK) {
			escrever("LUA NOT OK");
			
			

			
			//get error
			cout << "error in file: " << s << "\n";
			escrever(lua_tostring(ret, -1));
			

			
		}





		return ret;
	}




};



class componente_lua : public componente {
	bool iniciado = false;
	map<string, lua_State*> estados_lua;
	map<string, shared_ptr<string>> scripts_lua_string;
	map<string, bool> scripts_lua_iniciados;
public:

	vector<string> pegar_lista_scripts() {
		vector<string> ret = {};
		for (pair<string, lua_State*> p : estados_lua) {
			ret.push_back(p.first);
		}
		return ret;
	}

	map<string, lua_State*> pegar_estados_lua() {
		return estados_lua;
	}

	void deletar_estado_lua(lua_State* L) {
		lua_close(L);
	}

	void remover_script(string s) {
		if (estados_lua.find(s) != estados_lua.end()) {
			deletar_estado_lua(estados_lua[s]);
			estados_lua.erase(s);
			scripts_lua_string.erase(s);
			scripts_lua_iniciados.erase(s);
		}
	}
	void remover_scripts(vector<string> s) {
		for (string S : s) {
			remover_script(S);
		}
	}
	void limpar_scripts() {
		vector<string> scripts;
		for (pair<string, lua_State*> p : estados_lua) {
			scripts.push_back(p.first);
		}
		remover_scripts(scripts);
	}

	void adicionar_script(string s) {
		if (estados_lua.find(s) != estados_lua.end()) {
			lua_getglobal(estados_lua[s], "END");
			lua_call(estados_lua[s], 0, 0);
			remover_script(s);
		}


		lua_State* L = funcoes_lua::carregar_luaState(s);
		estados_lua.insert(pair<string, lua_State*>(s, L));
		scripts_lua_string.insert(pair<string, shared_ptr<string>>(s, carregar_script_lua(s)));
		scripts_lua_iniciados.insert(pair<string, bool>(s, false));

		lua_pushstring(L, ponteiro_string(esse_objeto.get()).c_str());
		lua_setglobal(L, "this_object_ptr");

		//if (iniciado) {
		//	lua_getglobal(estados_lua[s], "START");
		//	lua_call(estados_lua[s], 0, 0);
		//}


	}
	void adicionar_scripts(vector<string> s) {
		for (string S : s) {
			adicionar_script(S);
		}
	}









	void iniciar() {
		iniciado = true;
		for (pair<string, lua_State*> p : estados_lua) {
			//esse objetoget_tile_set_tile
			lua_pushstring(p.second, ponteiro_string(esse_objeto.get()).c_str());
			lua_setglobal(p.second, "this_object_ptr");

			lua_getglobal(p.second, "START");
			lua_call(p.second, 0, 0);
			scripts_lua_iniciados[p.first] = true;
		}
	}
	void atualisar() {

		for (pair<string, lua_State*> p : estados_lua) {
			if (scripts_lua_iniciados[p.first]) {
				lua_State* L = p.second;
				lua_getglobal(L, "UPDATE");
				lua_call(L, 0, 0);
			}
			else {
				lua_pushstring(p.second, ponteiro_string(esse_objeto.get()).c_str());
				lua_setglobal(p.second, "this_object_ptr");
				lua_getglobal(p.second, "START");
				lua_call(p.second, 0, 0);
				scripts_lua_iniciados[p.first] = true;
			}
		}

	}
	void colidir(colis_info col) {
		for (pair<string, lua_State*> p : estados_lua) {
			lua_State* L = p.second;

			if (col.obj != NULL) {
				lua_getglobal(L, "COLIDE");

				//tabela
				lua_newtable(L);
				//id obj
				lua_pushstring(L, "object_ID");
				lua_pushstring(L, ponteiro_string(col.obj).c_str());

				lua_settable(L, -3);
				//fim tabela



				lua_call(L, 1, 0);
			}
		}
	}
	void finalisar() {
		for (pair<string, lua_State*> p : estados_lua) {



			lua_State* L = p.second;


			lua_getglobal(L, "END");
			lua_call(L, 0, 0);



		}
	}
	~componente_lua() {
		limpar_scripts();
	}
	//mudar
	void mudar_string(string script, string var, string valor) {
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_pushstring(estados_lua[script], valor.c_str());
			lua_setglobal(estados_lua[script], var.c_str());
		}
	}
	void mudar_int(string script, string var, int valor) {
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_pushinteger(estados_lua[script], valor);
			lua_setglobal(estados_lua[script], var.c_str());
		}
	}
	void mudar_numero(string script, string var, float valor) {
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_pushnumber(estados_lua[script], valor);
			lua_setglobal(estados_lua[script], var.c_str());
		}
	}
	void mudar_boleana(string script, string var, bool valor) {
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_pushboolean(estados_lua[script], valor);
			lua_setglobal(estados_lua[script], var.c_str());
		}
	}

	//pegar
	string pegar_string(string script, string var) {
		string ret = "";
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_getglobal(estados_lua[script], var.c_str());
			ret = lua_tostring(estados_lua[script], -1);
		}
		return ret;
	}
	int pegar_int(string script, string var) {
		int ret = 0;
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_getglobal(estados_lua[script], var.c_str());
			ret = lua_tointeger(estados_lua[script], -1);
		}
		return ret;
	}
	float pegar_numero(string script, string var) {
		float ret = 0;
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_getglobal(estados_lua[script], var.c_str());
			ret = lua_tonumber(estados_lua[script], -1);
		}
		return ret;
	}
	bool pegar_boleana(string script, string var) {
		bool ret = false;
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_getglobal(estados_lua[script], var.c_str());
			ret = lua_toboolean(estados_lua[script], -1);
		}
		return ret;
	}

	void chamar_funcao(string script, string func) {
		if (estados_lua.find(script) != estados_lua.end()) {
			lua_getglobal(estados_lua[script], func.c_str());
			lua_call(estados_lua[script], 0, 0);
		}
	}

	componente_lua() {

	}
	componente_lua(string s) {
		adicionar_script(s);
	}
	componente_lua(vector<string> s) {
		adicionar_scripts(s);
	}


};

map<string,void(*)(objeto_jogo*,bool)> add_remove_component_by_string = {
	pair<string,void(*)(objeto_jogo*,bool)>("transform",[](objeto_jogo* obj,bool add){if(add){obj->adicionar_componente<transform_>(transform_());}else{obj->remover_componente<transform_>();}}),
	pair<string,void(*)(objeto_jogo*,bool)>("camera",[](objeto_jogo* obj,bool add){if(add){obj->adicionar_componente<camera>(camera());}else{obj->remover_componente<camera>();}}),
	pair<string,void(*)(objeto_jogo*,bool)>("render_text",[](objeto_jogo* obj,bool add){if(add){obj->adicionar_componente<render_texto>(render_texto());}else{obj->remover_componente<render_texto>();}}),
	pair<string,void(*)(objeto_jogo*,bool)>("render_sprite",[](objeto_jogo* obj,bool add){if(add){obj->adicionar_componente<render_sprite>(render_sprite());}else{obj->remover_componente<render_sprite>();}}),
	pair<string,void(*)(objeto_jogo*,bool)>("render_shader",[](objeto_jogo* obj,bool add){if(add){obj->adicionar_componente<render_shader>(render_shader());}else{obj->remover_componente<render_shader>();}}),
	pair<string,void(*)(objeto_jogo*,bool)>("physics_2D",[](objeto_jogo* obj,bool add){if(add){obj->adicionar_componente<box_2D>(box_2D());}else{obj->remover_componente<box_2D>();}}),
	pair<string,void(*)(objeto_jogo*,bool)>("character_physics_2D",[](objeto_jogo* obj,bool add){if(add){obj->adicionar_componente<fisica_char_B2D>(fisica_char_B2D());}else{obj->remover_componente<fisica_char_B2D>();}}),
	pair<string,void(*)(objeto_jogo*,bool)>("audio_source",[](objeto_jogo* obj,bool add){if(add){obj->adicionar_componente<sfml_audio>(sfml_audio());}else{obj->remover_componente<sfml_audio>();}}),
	pair<string,void(*)(objeto_jogo*,bool)>("lua_scripts",[](objeto_jogo* obj,bool add){if(add){obj->adicionar_componente<componente_lua>(componente_lua());}else{obj->remover_componente<componente_lua>();}}),
	pair<string,void(*)(objeto_jogo*,bool)>("render_tile_map",[](objeto_jogo* obj,bool add){if(add){obj->adicionar_componente<render_tilemap>(render_tilemap());}else{obj->remover_componente<render_tilemap>();}}),
	pair<string,void(*)(objeto_jogo*,bool)>("render_mesh",[](objeto_jogo* obj,bool add){if(add){obj->adicionar_componente<render_malha>(render_malha());}else{obj->remover_componente<render_malha>();}}),
};

int add_component(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	if (argumentos == 2) {
		add_remove_component_by_string[lua_tostring(L, 2)](obj,true);
	}


	return 0;
}

int remove_component(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	if (argumentos == 2) {
		add_remove_component_by_string[lua_tostring(L, 2)](obj,false);
	}


	return 0;
}

int get_component_size(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	int ret = 0;
	if (argumentos == 1) {
		ret = obj->pegar_lista_componentes().size();
	}

	lua_pushnumber(L, ret);
	return 1;
}


map<string,bool(*)(objeto_jogo*)> have_component_by_string = {
	pair<string,bool(*)(objeto_jogo*)>("transform",[](objeto_jogo* obj){return obj->tem_componente<transform_>();}),
	pair<string,bool(*)(objeto_jogo*)>("camera",[](objeto_jogo* obj){return obj->tem_componente<camera>();}),
	pair<string,bool(*)(objeto_jogo*)>("render_text",[](objeto_jogo* obj){return obj->tem_componente<render_texto>();}),
	pair<string,bool(*)(objeto_jogo*)>("render_sprite",[](objeto_jogo* obj){return obj->tem_componente<render_sprite>();}),
	pair<string,bool(*)(objeto_jogo*)>("render_shader",[](objeto_jogo* obj){return obj->tem_componente<render_shader>();}),
	pair<string,bool(*)(objeto_jogo*)>("physics_2D",[](objeto_jogo* obj){return obj->tem_componente<box_2D>();}),
	pair<string,bool(*)(objeto_jogo*)>("character_physics_2D",[](objeto_jogo* obj){return obj->tem_componente<fisica_char_B2D>();}),
	pair<string,bool(*)(objeto_jogo*)>("audio_source",[](objeto_jogo* obj){return obj->tem_componente<sfml_audio>();}),
	pair<string,bool(*)(objeto_jogo*)>("lua_scripts",[](objeto_jogo* obj){return obj->tem_componente<componente_lua>();}),
	pair<string,bool(*)(objeto_jogo*)>("render_tile_map",[](objeto_jogo* obj){return obj->tem_componente<render_tilemap>();}),
	pair<string,bool(*)(objeto_jogo*)>("render_mesh",[](objeto_jogo* obj){return obj->tem_componente<render_malha>();}),
};
int have_component(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	bool ret = false;
	if (argumentos == 2) {
		ret = have_component_by_string[lua_tostring(L, 2)](obj);
	}

	lua_pushboolean(L, ret);
	return 1;
}

int add_script_lua(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 2 && cl != NULL) {
		cl->adicionar_script(lua_tostring(L, 2));
	}
	return 0;
}

int remove_script(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 2 && cl != NULL) {
		cl->remover_script(lua_tostring(L, 2));
	}
	return 0;
}

//scripts

//pegar_lista_scripts

int get_script_size(lua_State* L) {
	int output = 0;
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	if (argumentos == 1 && obj != NULL && obj->pegar_componente<componente_lua>() != NULL) {
		output = obj->pegar_componente<componente_lua>()->pegar_lista_scripts().size();
	}
	lua_pushnumber(L, output);
	return 1;
}

int get_script_name(lua_State* L) {
	string output = "";
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	if (argumentos == 2 && obj != NULL && obj->pegar_componente<componente_lua>() != NULL) {
		output = obj->pegar_componente<componente_lua>()->pegar_lista_scripts()[0];
	}
	lua_pushstring(L, output.c_str());
	return 1;
}

int have_script(lua_State* L) {
	bool output = false;
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 2 && cl != NULL) {
		map<string, lua_State*> m = cl->pegar_estados_lua();
		if (m.find(lua_tostring(L, 2)) != m.end()) {
			output = true;
		}
	}
	lua_pushboolean(L, output);
	return 1;
}

int get_script_var(lua_State* L) {

	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 4 && cl != NULL) {

		if (lua_tonumber(L, 4) == 0) {
			lua_pushstring(L, cl->pegar_string(lua_tostring(L, 2), lua_tostring(L, 3)).c_str());
			return 1;
		}
		else if (lua_tonumber(L, 4) == 1) {
			lua_pushnumber(L, cl->pegar_numero(lua_tostring(L, 2), lua_tostring(L, 3)));
			return 1;
		}
		else if (lua_tonumber(L, 4) == 2) {
			lua_pushboolean(L, cl->pegar_boleana(lua_tostring(L, 2), lua_tostring(L, 3)));
			return 1;
		}

	}
	return 0;
}

int set_script_var(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 5 && cl != NULL) {

		if (lua_tonumber(L, 4) == 0) {
			cl->mudar_string(lua_tostring(L, 2), lua_tostring(L, 3), lua_tostring(L, 5));
		}
		else if (lua_tonumber(L, 4) == 1) {
			cl->mudar_numero(lua_tostring(L, 2), lua_tostring(L, 3), lua_tonumber(L, 5));

		}
		else if (lua_tonumber(L, 4) == 2) {
			cl->mudar_boleana(lua_tostring(L, 2), lua_tostring(L, 3), lua_toboolean(L, 5));

		}

	}
	return 0;
}

int call_script_function(lua_State* L) {
	int argumentos = lua_gettop(L);
	objeto_jogo* obj = NULL;
	if (argumentos > 0) {
		obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 1));
	}
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if (argumentos == 3 && cl != NULL) {
		cl->chamar_funcao(lua_tostring(L, 2), lua_tostring(L, 3));
	}
	return 0;
}

int get_lua_component(lua_State* L){
	Table ret;
	objeto_jogo* obj = string_ponteiro<objeto_jogo>(lua_tostring(L, 2));
	shared_ptr<componente_lua> cl = obj->pegar_componente<componente_lua>();
	if(cl != NULL){
		ret.setTable("scripts", vString_table(cl->pegar_lista_scripts()));
	}
	lua_pushtable(L,ret);
	return 1;
}
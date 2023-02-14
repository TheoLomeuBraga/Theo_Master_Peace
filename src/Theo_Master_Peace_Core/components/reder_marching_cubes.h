#pragma once

#include <iostream>
#include <functional>
using namespace std;


#include "RecursosT.h"
#include "game_object.h"

class reder_marching_cubes : public componente {
public:
	reder_marching_cubes() {}
	bool ligado = true;
	char lado_render = lado_render_malha::both;
	bool usar_oclusao = true;
	float porcentagem_pode_ocupar_tela = 1;
	uint8_t camada = 0;
	




};
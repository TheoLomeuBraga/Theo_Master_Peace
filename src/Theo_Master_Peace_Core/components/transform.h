#pragma once

#include <iostream>
#include <functional>
using namespace std;
#include "RecursosT.h"
#include "game_object.h"


mat4 MatrizMundi = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));

	class transform_ : public componente {
	public:






		transform_() {
			matrizTransform = MatrizMundi;
		}

		
		bool UI = false;
		transform_* paiTF;
		


		

		int local_hierarquia;

		bool usar_pai_matriz;
		mat4 pai_matriz;


		glm::vec3 pos, esca;

		glm::mat4 matrizTransform;
		quat quater;




		


		glm::mat4 pegar_matriz() {
			glm::mat4 ret;

			if (paiTF == NULL) {
				ret = MatrizMundi;
			}
			else
			{
				ret = paiTF->matrizTransform;
			}



			ret = translate(ret, pos);

			ret *= toMat4(quater);
			ret = scale(ret, esca);

			matrizTransform = ret;




			return ret;
		}

		glm::vec3 pegar_pos_global() {
			vec3 nada;
			vec4 nada2;
			vec3 pos;
			quat nada3;
			glm::decompose(matrizTransform, nada, nada3, pos, nada, nada2);
			return pos;
		}
		glm::quat pegar_qua_global() {
			vec3 nada;
			vec4 nada2;
			vec3 nada3;
			quat qua;
			glm::decompose(matrizTransform, nada, qua, nada3, nada, nada2);
			return qua;
		}
		glm::vec3 pegar_graus_global() {
			return quat_graus(pegar_qua_global());
		}

		void matriz_transform(glm::mat4 mat) {
			vec3 nada;
			vec4 nada2;

			vec3 escala;
			vec3 pos;
			quat qua;
			glm::decompose(paiTF->matrizTransform, escala, qua, pos, nada, nada2);



		}

		vec3 pegar_direcao_local(vec3 dir) {
			mat4 m = glm::translate(matrizTransform,dir);
			vec3 nada;
			vec4 nada2;
			vec3 escala;
			vec3 pos;
			quat qua;
			glm::decompose(m, escala, qua, pos, nada, nada2);

			return pos - pegar_pos_global() ;
		}

		void reset(bool ui, transform_* pai, glm::vec3 p, glm::vec3 r, glm::vec3 e) {
			UI = ui;
			paiTF = pai;
			pos = p;
			quater = graus_quat(r);
			esca = e;
			pegar_matriz();
		}

		void atualizar_tf() {
			//OBS: o unico proposito dessa fun��o e eu n�o me confundir 
			pegar_matriz();
		}


		transform_(glm::vec3 p, glm::vec3 r, glm::vec3 e) {
			reset(false, NULL, p, r, e);
		}




		transform_(bool ui, glm::vec3 p, glm::vec3 r, glm::vec3 e) {
			reset(ui, NULL, p, r, e);
		}





		transform_(transform_* pai, glm::vec3 p, glm::vec3 r, glm::vec3 e) {
			reset(false, pai, p, r, e);
		}

		transform_(bool ui, transform_* pai, glm::vec3 p, glm::vec3 r, glm::vec3 e) {
			reset(ui, pai, p, r, e);
		}

		vec3 pegar_angulo_graus() {
			return quat_graus(quater);

		}
		void mudar_angulo_graus(vec3 ang) {
			quater = graus_quat(ang);
		}

	};












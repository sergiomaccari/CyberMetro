#pragma once
#include "Obstaculo.h"

namespace CyberMetro {
	namespace Entidades {
		namespace Obstaculos {
			using json = nlohmann::json;

			class Choquinho : public Obstaculos::Obstaculo
			{
			private:
				int dano;

			public:
				Choquinho(float xi, float yi);
				~Choquinho();

				void obstaculizar(Personagens::Jogador* pj);
				void executar();

				void salvarDataBuffer(json& arq);
				void carregarDeBuffer(const json& data);
			};
		}
	}
}
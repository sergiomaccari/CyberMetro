#pragma once
#include "Obstaculo.h"

namespace CyberMetro {
    namespace Entidades {
        namespace Obstaculos {
            using json = nlohmann::json;

            class Plataforma : public Obstaculo
            {
            private:
                static const float largura_plataforma;
                static const float altura_plataforma;

                float yOrigem;
                float altura;
                static const float VELOCIDADE_FLUTUACAO;
                float tempoOffset;

                static const float imunidade;
				static const float anti_grav;


            public:
                Plataforma(float xi, float yi, int alt = 64);
                ~Plataforma();

                void obstaculizar(Entidades::Personagens::Jogador* pj);
                void executar();

                void salvarDataBuffer(json& arq);
                void carregarDeBuffer(const json& data);
            };
        }
    }
}
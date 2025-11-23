#pragma once
#include "Obstaculo.h"

namespace CyberMetro {
    namespace Entidades {
        namespace Obstaculos {
            using json = nlohmann::json;

            class Gelinho : public Obstaculo
            {
            private:
                float largura;
            public:
                Gelinho(float xi, float yi);
                ~Gelinho();

                void obstaculizar(Entidades::Personagens::Jogador* pj);
                void executar();

                void salvarDataBuffer(json& arq);
                void carregarDeBuffer(const json& data);
            };
        }
    }
}
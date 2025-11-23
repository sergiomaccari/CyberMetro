#pragma once
#include "Inimigo.h"

namespace CyberMetro {
    namespace Entidades {
        namespace Personagens {
            using json = nlohmann::json;

            class Robo_Senior : public Inimigo
            {
            private:
                int tamanho;
                float tempoProximaCura;
                float intervaloCura;
                float xINI;

            public:
                Robo_Senior(float xi, float yi);
                ~Robo_Senior();
                void mover();
                void executar();
                void danificar(Entidades::Personagens::Jogador* pJogador);
                void curar();

                void salvarDataBuffer(json& arq);
                void carregarDeBuffer(const json& data);
            };
        }
    }
}
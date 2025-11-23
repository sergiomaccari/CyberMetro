#pragma once
#include "Inimigo.h"


namespace CyberMetro {
    namespace Entidades {
        namespace Personagens {
            using json = nlohmann::json;

            class Robo_Junior : public Inimigo
            {
            protected:
                int volta;
                float xINI;
                float raio;

            public:
                Robo_Junior(float xi, float yi);
                ~Robo_Junior();
                void mover();
                void danificar(Jogador* pJogador);
                void executar();
                void salvarDataBuffer(json& arq);
                void carregarDeBuffer(const json& data);
 
            };
        }
    }
}
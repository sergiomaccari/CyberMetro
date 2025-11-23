#pragma once
#include "Inimigo.h" 
#include "ListaEntidades.h"
#include "Projetil.h"

namespace CyberMetro {
    namespace Entidades {
        namespace Personagens {
            using json = nlohmann::json;

            class Robo_CEO : public Inimigo {
            private:
                int forca;
                Jogador* pJogador;
                Listas::ListaEntidades projeteis;

                float PROJETIL_VELOCIDADE_CHEFE;
                static const int COOLDOWN_TIRO_CHEFE_FRAMES;
                int cooldownTiro;

                void lancar();

            public:
                Robo_CEO(float xi, float yi);
                ~Robo_CEO();
                void mover();
                void executar();
                void danificar(Jogador* pJogador);
                void setJogador(Jogador* j);
                Listas::ListaEntidades* getProjeteis();
                void salvarDataBuffer(json& arq);
                void carregarDeBuffer(const json& data);
            };
        }
    }
}
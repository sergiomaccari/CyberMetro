#pragma once
#include "Personagem.h"
#include "Projetil.h"
#include "ListaEntidades.h"


namespace CyberMetro {
    namespace Fases
    {
        class Fase;
    }
    namespace Entidades {
        namespace Personagens
        {
            using json = nlohmann::json;

            class Jogador : public Personagem
            {
            private:
                int pontos;
                Listas::ListaEntidades projeteis;
                int cooldown;
                int playerNum;
                float ultimaDirecaoX;
                static const float PROJETIL_VELOCIDADE_JOGADOR;

                sf::Time stun;
                sf::Time obstaculoCooldown;
            public:
                Jogador(int numJogador);
                Jogador(int numJogador, float xi, float yi);
                virtual ~Jogador();
                void mover();
                void executar();

                void lancar();
                Listas::ListaEntidades* getProjeteis();
                void trava_mov(float t);

                void iniciarObstaculoCooldown(sf::Time duracao);
                sf::Time getObstaculoCooldown() const;

                void resetar(float posX, float posY);
                void adicionarPontos(int p);

                int getPontos() const;

                void salvarDataBuffer(json& arq);
                void carregarDeBuffer(const json& data);
            };
        }
    }
}
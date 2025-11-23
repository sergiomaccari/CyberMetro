#pragma once
#include "Fase.h"
#include "Robo_CEO.h"
#include "Choquinho.h"


namespace CyberMetro {
    namespace Fases {
        class FaseSegunda : public Fase
        {
        private:
            
            static const int MIN_CEO;
            static const int MAX_CEO;
            
            static const int MIN_CHOQUINHO;
            static const int MAX_CHOQUINHO;

            std::vector<sf::Vector2f> posi_choquinho;
            std::vector<sf::Vector2f> posi_ceo;

            void criarRoboCEO(float x, float y);
            void criarChoquinho(float x, float y);

        public:
            FaseSegunda(Entidades::Personagens::Jogador* jogador1, Entidades::Personagens::Jogador* jogador2, bool carregarSave = false);
            ~FaseSegunda();

            void criarInimigos();
            void criarObstaculos();
            void desenharMapa();
        };
    }
}
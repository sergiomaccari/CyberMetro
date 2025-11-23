#pragma once
#include "Fase.h"
#include "Gelinho.h" 
#include "Choquinho.h" 
#include "Robo_Senior.h" 

namespace CyberMetro {
    namespace Fases {
        class FasePrimeira : public Fase
        {
        private:

            
            static const int MIN_SENIOR;
            static const int MAX_SENIOR;
            static const int MIN_GELINHO;
            static const int MAX_GELINHO;
      
            std::vector<sf::Vector2f> posi_robo_senior;
            std::vector<sf::Vector2f> posi_gelinho;

            void criarRoboSenior(float x, float y);
            void criarGelinho(float x, float y);

        public:
            FasePrimeira(Entidades::Personagens::Jogador* jogador1, Entidades::Personagens::Jogador* jogador2, bool carregarSave = false);
            ~FasePrimeira();

            void criarInimigos();
            void criarObstaculos();
            void desenharMapa();
        };
    }
}
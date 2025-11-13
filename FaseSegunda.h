#pragma once
#include "Fase.h"
#include "Robo_CEO.h"
#include "Choquinho.h"
#include <vector> 

namespace Fases {
    class FaseSegunda : public Fase
    {
    private:
        static const int MIN_JUNIOR;
        static const int MAX_JUNIOR;
        static const int MIN_CEO;
        static const int MAX_CEO;
        static const int MIN_PLATAFORMA;
        static const int MAX_PLATAFORMA;
        static const int MIN_CHOQUINHO;
        static const int MAX_CHOQUINHO;

        std::vector<sf::Vector2f> posi_choquinho;
        std::vector<sf::Vector2f> posi_ceo;

    protected:
        void criarRoboCEO(float x, float y);
        void criarChoquinho(float x, float y);

    public:
        FaseSegunda(Jogador* jogador1, Jogador* jogador2);
        ~FaseSegunda();
        virtual void criarInimigos();
        virtual void criarObstaculos();
        virtual void desenharMapa();
    };
}
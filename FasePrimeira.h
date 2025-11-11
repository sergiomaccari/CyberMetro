#pragma once
#include "Fase.h"
#include "Gelinho.h" 
#include "Choquinho.h" 
#include "Robo_Senior.h" 
#include <vector>  
#include <SFML/System/Vector3.hpp> // Necessario para sf::Vector3f

namespace Fases {
    class FasePrimeira : public Fase
    {
    private:

        // Constantes para numero de entidades
        static const int MIN_JUNIOR;
        static const int MAX_JUNIOR;
        static const int MIN_SENIOR;
        static const int MAX_SENIOR;
        static const int MIN_PLATAFORMA;
        static const int MAX_PLATAFORMA;
        static const int MIN_GELINHO;
        static const int MAX_GELINHO;

        // Listas com todas as posicoes de spawn possiveis
        std::vector<sf::Vector2f> posi_robo_senior;
        std::vector<sf::Vector2f> posi_gelinho;


    protected:
        void criarRoboSenior(float x, float y);
        void criarGelinho(float x, float y);

    public:
        FasePrimeira(Jogador* jogador1, Jogador* jogador2);
        ~FasePrimeira();
        virtual void criarInimigos();
        virtual void criarObstaculos();
        void criarMapa();
        virtual void desenharMapa();
    };
}
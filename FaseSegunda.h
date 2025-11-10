#pragma once
#include "Fase.h"
#include "Robo_CEO.h"
#include "Choquinho.h"

namespace Fases {
    class FaseSegunda : public Fase
    {
    private:
        sf::Texture texturaTileset;
        std::vector<std::vector<unsigned int>> gridMapa;

        std::vector<sf::Vector2f> posi_robo_junior;
        std::vector<sf::Vector2f> posi_gelinho;
        std::vector<sf::Vector2f> posi_choquinho;
        std::vector<sf::Vector2f> posi_ceo;

    protected:
        void criarChefe(float x, float y);
        void criarChoquinho(float x, float y);
        void criarGelinho(float x, float y);

    public:
        FaseSegunda(Jogador* jogador1, Jogador* jogador2);
        virtual ~FaseSegunda();
        virtual void criarInimigos();
        virtual void criarObstaculos();
        void criarMapa();
        virtual void desenharMapa();
    };
}
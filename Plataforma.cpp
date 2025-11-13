#include "Plataforma.h"
#include <iostream>

using namespace Personagens;

namespace Obstaculos {

    const float Plataforma::altura_plataforma = 32.0f;
    const float Plataforma::largura_plataforma = 32.0f;
    const float Plataforma::VELOCIDADE_FLUTUACAO = 1.5f;
    const float Plataforma::IMUNIDADE = 15.0f;

 //possivelmente da pra otimizar o movimento mas ok to num grind pica

    Plataforma::Plataforma(float xi, float yi, int alt)
        : Obstaculo(xi, yi),
        yIni(yi), // yi eh o centro
        amplitude((float)alt-32) // -32 pra nao expremer jogador na parede
    {
        if (pGG)
        {
            sf::Texture* tex = pGG->getTextura("Imagens/plataforma.png");
            if (tex)
            {
                pFigura->setTexture(*tex);
                pFigura->setScale(32.0f / tex->getSize().x, 32.0f / tex->getSize().y);
            }
        }
        else
        {
            std::cerr << "Erro Plataforma" << std::endl;
        }


        this->x = xi;
        this->y = yi;
        setPosicaoGrafica(this->x, this->y);
        this->danoso = false;
    }

    Plataforma::~Plataforma()
    {

    }

    void Plataforma::anti_gravitar(sf::Vector2f* pos)
    {
		vel_grav -= 0.0249f;
		*pos += sf::Vector2f(0.0f, vel_grav);
		
    }

    void Plataforma::obstaculizar(Jogador* pj)
    {
        pj->iniciarObstaculoCooldown(sf::seconds(IMUNIDADE));
    }

    void Plataforma::salvar()
    {

    }

    void Plataforma::executar()
    {
        float tempoTotal = clock.getElapsedTime().asSeconds();
        this->y = yIni + (amplitude * std::sin(tempoTotal * VELOCIDADE_FLUTUACAO));
        setPosicaoGrafica(this->x, this->y);
    }

}
#include "Gelinho.h"

using namespace Personagens;

namespace Obstaculos
{
    Gelinho::Gelinho(float xi, float yi) : Obstaculo(xi, yi), largura(rand()%6) {
        if (pGG)
        {
            sf::Texture* tex = pGG->getTextura("Imagens/obstaculo_medio.png");
            if (tex)
            {
                pFigura->setTexture(*tex);
                pFigura->setScale(32.0f / tex->getSize().x, (32.0f + float(largura)) / tex->getSize().y);
            }
        }
        else
        {
            std::cerr << "Erro" << std::endl;
        }

        this->x = xi;
        this->y = yi;
        setPosicaoGrafica(this->x, this->y);
    }

    Gelinho::~Gelinho() {}

    void Gelinho::obstaculizar(Jogador* pj)
    {
        if (pj && pj->getObstaculoCooldown() <= sf::Time::Zero)
        {
            pj->trava_mov(1.0f);
            pj->iniciarObstaculoCooldown(sf::seconds(1.5 + 2.0f*float(largura))); //1.5 segundos + 2*largura de tempo de congelamento
        }
    }

    void Gelinho::salvar()
    {

    }

    void Gelinho::executar()
    {
        vel_grav += grav;
        this->y += vel_grav;
        setPosicaoGrafica(this->x, this->y);
    }
}
#include "Entidade.h"

namespace Entidades
{
    const float Entidade::grav = 0.025f;

    Entidade::Entidade(float xi, float yi) : x(xi), y(yi), estaAtivo(true), vel_grav(0)
    {

    }

    Entidade::Entidade() : estaAtivo(true), vel_grav(0)
    {
        this->x = 0.0f;
        this->y = 0.0f;
    }
    Entidade::~Entidade()
    {

    }

    void Entidade::setPosicaoGrafica(float xi, float yi)
    {
        if (pFigura)
        {
            this->pFigura->setPosition(xi, yi);
        }
    }

    sf::Vector2f Entidade::getPosicaoGrafica()
    {
        if (pFigura)
        {
            return this->pFigura->getPosition();
        }
        return sf::Vector2f(0.f, 0.f);
    }

    void Entidade::setX(float novoX)
    {
        x = novoX;
    }

    void Entidade::setY(float novoY)
    {
        y = novoY;
    }

    float Entidade::getX() const
    {
        return x;
    }

    float Entidade::getY() const
    {
        return y;
    }

    void Entidade::setAtivo(bool b)
    {
        estaAtivo = b;
    }

    bool Entidade::getAtivo() const
    {
        return estaAtivo;
    }

    void Entidade::setVel_Grav(float nv)
    {
        vel_grav = nv;
    }

    float Entidade::getVel_Grav() const
    {
        return vel_grav;
    }

    void Entidade::gravidade(sf::Vector2f* pos)
    {
        vel_grav += grav;
        *pos += sf::Vector2f(0.0f, vel_grav);
    }

    void Entidade::setPosicaoGrafica(const sf::Vector2f& novaPos)
    {
        this->setPosicaoGrafica(novaPos.x, novaPos.y);
    }
}
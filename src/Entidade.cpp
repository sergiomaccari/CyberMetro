#include "Entidade.h"


namespace CyberMetro {
    namespace Entidades
    {
        const float Entidade::grav = 0.025f;

        Entidade::Entidade(float xi, float yi) : x(xi), y(yi), estaAtivo(true), vel_grav(0)
        {

        }

        Entidade::Entidade() : x(0.f), y(0.f), estaAtivo(true), vel_grav(0)
        {

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

        void Entidade::gravitar(sf::Vector2f* pos)
        {
            vel_grav += grav;
            *pos += sf::Vector2f(0.0f, vel_grav);
        }

        void Entidade::setPosicaoGrafica(const sf::Vector2f& novaPos)
        {
            this->setPosicaoGrafica(novaPos.x, novaPos.y);
        }

        void Entidade::salvarDataBuffer(json& arq)
        {
            arq["id"] = this->id;
            arq["x"] = this->x;
            arq["y"] = this->y;
            arq["estaAtivo"] = this->estaAtivo;
            arq["vel_grav"] = this->vel_grav;
        }

        void Entidade::carregarDeBuffer(const json& data)
        {
            this->id = this->id;
            this->x = data.value("x", this->x);
            this->y = data.value("y", this->y);
            this->estaAtivo = data.value("estaAtivo", this->estaAtivo);
            this->vel_grav = data.value("vel_grav", this->vel_grav);
            setPosicaoGrafica(this->x, this->y);
        }
    }
}
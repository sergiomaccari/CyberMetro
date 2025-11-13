#pragma once
#include "Ente.h"

namespace Entidades
{
    class Entidade : public Ente
    {
    protected:
        float x;
        float y;
        bool estaAtivo;
        float vel_grav;
        static const float grav;
        sf::Clock clock;
        sf::Time tempo;
		//sf::Vector2f movimento;


    public:
		Entidade(float xi, float yi);
        Entidade();
        virtual ~Entidade();
        void setX(float novoX);
        void setY(float novoY);
        float getX() const;
        float getY() const;
        void setPosicaoGrafica(float xi, float yi);
        sf::Vector2f getPosicaoGrafica();
        virtual void salvar() = 0;
        virtual void executar() = 0;
        virtual void setAtivo(bool b);
        virtual bool getAtivo() const;
        void setVel_Grav(float nv);
        float getVel_Grav() const;
		void gravitar(sf::Vector2f* pos);
        virtual void anti_gravitar(sf::Vector2f* pos);
        void setPosicaoGrafica(const sf::Vector2f& novaPos); 
    };
}
#pragma once
#include "Entidade.h"


namespace Entidades
{
    class Projetil : public Entidades::Entidade
    {
    private:
        int dano;
        float vx;
        float vy;
        int idDono;// 0 =roboceo 1=jog1 2=jog2

        const float FORCA_GRAVIDADE_PROJETIL;

    public:
        Projetil();
        ~Projetil();
        virtual void executar();
        virtual void salvar();
        void setVelocidade(float velX, float velY);

        
        void setIdDono(int id);
        int getIdDono() const;

        int getDano() const;
    };
}
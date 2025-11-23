#pragma once
#include "Entidade.h"


namespace CyberMetro {
    namespace Entidades
    {
        using json = nlohmann::json;

        class Projetil : public Entidades::Entidade
        {
        private:
            int dano;
            float vx;
            float vy;
            int idDono;// 0 =roboceo 1=jog1 2=jog2

            static const float FORCA_GRAVIDADE_PROJETIL;

        public:
            Projetil();
            ~Projetil();
            virtual void executar();

            void setVelocidade(float velX, float velY);
            void setIdDono(int id);
            int getIdDono() const;
            int getDano() const;

            void salvarDataBuffer(json& arq);
            virtual void carregarDeBuffer(const json& data);

        };
    }
}
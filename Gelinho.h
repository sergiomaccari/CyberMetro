#pragma once
#include "Obstaculo.h"


namespace Obstaculos {
    class Gelinho : public Obstaculo
    {
    private:
        float largura;
    public:
        Gelinho(float xi, float yi);
        ~Gelinho();
        virtual void obstaculizar(Jogador* pj);
        virtual void salvar();
        virtual void executar();
    };
}
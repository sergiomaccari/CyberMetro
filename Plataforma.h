#pragma once
#include "Obstaculo.h"

using namespace Personagens;

namespace Obstaculos {
    class Plataforma : public Obstaculo
    {
    private:
        static const float largura_plataforma;
        static const float altura_plataforma;
        float yOrigin;
        float amplitude;
        static const float VELOCIDADE_FLUTUACAO;

    public:
        Plataforma(float xi, float yi, int alt = 64);
        ~Plataforma();

        void obstaculizar(Jogador* pj);
        void salvar();
        void executar();


    };
}
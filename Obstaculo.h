#pragma once
#include "Entidade.h"
#include "Jogador.h"

using namespace Personagens;

namespace Obstaculos
{
    class Obstaculo : public Entidades::Entidade
    {
    protected:
        bool danoso;

    public:
        Obstaculo(float xi, float yi);
        virtual ~Obstaculo();
        virtual void obstaculizar(Jogador* pj) = 0;
        bool getDanoso() const;
    };
}
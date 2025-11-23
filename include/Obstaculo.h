#pragma once
#include "Jogador.h"

namespace Entidades {
    namespace Personagens {
        class Jogador;
    }
}
namespace CyberMetro {
    namespace Entidades {
        namespace Obstaculos
        {
            using json = nlohmann::json;

            class Obstaculo : public Entidades::Entidade
            {
            protected:
                bool danoso;

            public:
                Obstaculo(float xi, float yi);
                virtual ~Obstaculo();
                virtual void obstaculizar(Personagens::Jogador* pj) = 0;
                virtual void executar() = 0;

                virtual void salvarDataBuffer(json& arq);
                virtual void carregarDeBuffer(const json& data);

                bool getDanoso() const;
            };
        }
    }
}
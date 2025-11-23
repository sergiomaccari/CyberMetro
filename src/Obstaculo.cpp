#include "Obstaculo.h"

namespace CyberMetro {
    namespace Entidades {
        namespace Obstaculos
        {
            Obstaculo::Obstaculo(float xi, float yi) :
                Entidade(xi, yi),
                danoso(false)
            {
                this->x = xi;
                this->y = yi;

            }
            Obstaculo::~Obstaculo() {}

            void Obstaculo::salvarDataBuffer(json& arq)
            {
                Entidade::salvarDataBuffer(arq);
                arq["danoso"] = this->danoso;
            }

            void Obstaculo::carregarDeBuffer(const json& data)
            {
                Entidade::carregarDeBuffer(data);
                this->danoso = data.value("danoso", this->danoso);
            }

            bool Obstaculo::getDanoso() const
            {
                return danoso;
            }

        }
    }
}
#include "Inimigo.h"
namespace CyberMetro {
	namespace Entidades {
		namespace Personagens
		{
			Inimigo::Inimigo(float xi, float yi) :
				pontosPorMorte(0)
			{

				nivel_maldade = rand() % 5 + 1;
			}


			Inimigo::~Inimigo()
			{
				nivel_maldade = 0;
			}


			void Inimigo::executar() 
			{
			
			}


			int Inimigo::getPontosPorMorte() const
			{
				return pontosPorMorte;
			}

			void Inimigo::salvarDataBuffer(json& arq)
			{
				Personagem::salvarDataBuffer(arq);
				arq["nivel_maldade"] = this->nivel_maldade;
			}
			void Inimigo::carregarDeBuffer(const json& data)
			{
				Personagem::carregarDeBuffer(data);
				this->nivel_maldade = data.value("nivel_maldade", this->nivel_maldade);
			}
		}
	}
}
#include "Choquinho.h"

using namespace CyberMetro::Entidades::Personagens;
namespace CyberMetro {
	namespace Entidades {
		namespace Obstaculos {

			Choquinho::Choquinho(float xi, float yi) : Obstaculo(xi, yi)
			{

				dano = (rand() % 6) + 10;

				if (pGG)
				{
					sf::Texture* tex = pGG->getTextura("assets/obstaculo_dificil.png");
					if (tex)
					{
						pFigura->setTexture(*tex);
						pFigura->setScale(32.0f / tex->getSize().x, 32.0f / tex->getSize().y);
					}
				}
				else
				{
					std::cerr << "Erro Choquinho" << std::endl;
				}


				this->x = xi;
				this->y = yi;
				setPosicaoGrafica(this->x, this->y);

				this->danoso = true;
			}

			Choquinho::~Choquinho()
			{

			}

			void Choquinho::obstaculizar(Jogador* pj)
			{
				if (pj && pj->getObstaculoCooldown() <= sf::Time::Zero)
				{
					for (int i = 0; i < dano; i++) {
						pj->operator--();
					}

					pj->iniciarObstaculoCooldown(sf::seconds(0.5f));
				}
			}


			void Choquinho::salvarDataBuffer(json& arq)
			{
				Obstaculo::salvarDataBuffer(arq);
				arq["tipo"] = "Choquinho";
				arq["danosidade"] = this->dano;
			}

			void Choquinho::carregarDeBuffer(const json& data)
			{
				Obstaculo::carregarDeBuffer(data);

				this->dano = data.value("danosidade", this->dano);

				if (pGG)
				{
					sf::Texture* tex = pGG->getTextura("assets/obstaculo_dificil.png");
					if (tex)
					{
						pFigura->setTexture(*tex);
						pFigura->setScale(32.0f / tex->getSize().x, 32.0f / tex->getSize().y);
					}
				}
			}

			void Choquinho::executar()
			{
				Entidade::gravitar(&this->movimento);

				this->y += this->movimento.y * Ente::g_dt;

				setPosicaoGrafica(this->x, this->y);
			}

		}
	}
}
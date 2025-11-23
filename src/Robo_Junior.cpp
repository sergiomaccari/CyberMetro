#include "Robo_Junior.h"

namespace CyberMetro {
	namespace Entidades {
		namespace Personagens {

			Robo_Junior::Robo_Junior(float xi, float yi) :
				Inimigo(xi, yi),
				volta(1),
				raio((float)((rand() % 2) + 1) + 28)
			{
				this->xINI = xi;
				this->x = xi;
				this->y = yi;
				this->velocidade = 150.0f;
				this->n_vidas = 30;
				this->n_vidas_max = 30;
				this->pontosPorMorte = 100;


				if (pGG)
				{
					sf::Texture* tex = pGG->getTextura("assets/inimigo_facil.png");
					if (tex)
					{
						pFigura->setTexture(*tex);
						pFigura->setScale((float)raio / tex->getSize().x, (float)raio / tex->getSize().y);
					}
				}
				else
				{
					std::cerr << "Erro RoboJunior" << std::endl;
				}

				pFigura->setPosition(sf::Vector2f(static_cast<float>(this->x), static_cast<float>(this->y)));
				atualizarBarraVida();
			}
			Robo_Junior::~Robo_Junior()
			{

			}

			void Robo_Junior::mover()
			{
				this->movimento = sf::Vector2f(0.0f, 0.0f);

				float dt = Ente::g_dt;

				if (volta == 1 && this->x >= xINI + (int)raio * 3) {
					volta = -1;
				}
				else if (volta == -1 && this->x <= xINI) {
					volta = 1;
				}

				if (this->x <= x + (int)raio && volta == 1)
				{
					movimento += sf::Vector2f(1.0f, 0.f);

				}

				if (this->x >= x && volta == -1)
				{
					movimento += sf::Vector2f(-1.0f, 0.f);
				}

				Personagem::gravitar(&movimento);

				this->x += movimento.x * dt * velocidade;
				this->y += movimento.y * dt * velocidade;

				setPosicaoGrafica(this->x, this->y);
			}

			void Robo_Junior::danificar(Jogador* pJogador)
			{
				if (pJogador)
				{
					int danoCalculado = this->nivel_maldade - ((int(raio) % 28));
					if (danoCalculado < 1) {
						danoCalculado = 1;
					}

					for (int i = 0; i < danoCalculado; i++) {
						pJogador->operator--();
					}
				}
			}

			void Robo_Junior::salvarDataBuffer(json& arq)
			{
				Inimigo::salvarDataBuffer(arq);
				arq["tipo"] = "Robo_Junior";
				arq["xINI"] = this->xINI;
				arq["raio"] = this->raio;
				arq["volta"] = this->volta;
			}

			void Robo_Junior::carregarDeBuffer(const json& data)
			{
				Inimigo::carregarDeBuffer(data);
				this->xINI = data.value("xINI", this->xINI);
				this->raio = data.value("raio", this->raio);
				this->volta = data.value("volta", this->volta);
				if (pGG)
				{
					sf::Texture* tex = pGG->getTextura("assets/inimigo_facil.png");
					if (tex)
					{
						pFigura->setTexture(*tex);
						pFigura->setScale((float)raio / tex->getSize().x, (float)raio / tex->getSize().y);
					}
				}
			}

			void Robo_Junior::executar()
			{
				this->mover();
				atualizarBarraVida();
			}
		}
	}
}
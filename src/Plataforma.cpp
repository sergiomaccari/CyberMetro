#include "Plataforma.h"


namespace CyberMetro {
    using namespace Entidades::Personagens;
    namespace Entidades {
        namespace Obstaculos {

			const float Plataforma::anti_grav = Entidade::grav * (-1);
            const float Plataforma::imunidade = 3.0f;
            const float Plataforma::altura_plataforma = 32.0f;
            const float Plataforma::largura_plataforma = 32.0f;
            const float Plataforma::VELOCIDADE_FLUTUACAO = 1.5f;

            Plataforma::Plataforma(float xi, float yi, int alt)
                : Obstaculo(xi, yi),
                yOrigem(yi),
                altura((float)alt - 32),
                tempoOffset(0.0f)
            {
                if (pGG)
                {
                    sf::Texture* tex = pGG->getTextura("assets/plataforma.png");
                    if (tex)
                    {
                        pFigura->setTexture(*tex);
                        pFigura->setScale(32.0f / tex->getSize().x, 32.0f / tex->getSize().y);
                    }
                }
                else
                {
                    std::cerr << "Erro Plataforma" << std::endl;
                }


                this->x = xi;
                this->y = yi;
                setPosicaoGrafica(this->x, this->y);
                this->danoso = false;
                this->tempoOffset = (float)(rand() % 1000) / 100.0f;
            }

            Plataforma::~Plataforma()
            {

            }

            void Plataforma::obstaculizar(Jogador* pj)
            {
                pj->iniciarObstaculoCooldown(sf::seconds(imunidade + altura / 128));
            }

            void Plataforma::salvarDataBuffer(json& arq)
            {
                Obstaculo::salvarDataBuffer(arq);
                arq["tipo"] = "Plataforma";
                arq["yOrigem"] = this->yOrigem;
                arq["amplitude"] = this->altura;
                arq["tempoOffset"] = this->tempoOffset;
            }

            void Plataforma::carregarDeBuffer(const json& data)
            {
                Obstaculo::carregarDeBuffer(data);
                this->yOrigem = data.value("yOrigem", this->yOrigem);
                this->altura = data.value("amplitude", this->altura);

                this->tempoOffset = data.value("tempoOffset", this->tempoOffset);
            }

			void Plataforma::executar()
			{
				movimento = sf::Vector2f(0.0f, 1.0f);

				this->y = yOrigem + (altura * std::sin((this->tempoOffset + Ente::g_tempoTotal) * VELOCIDADE_FLUTUACAO));

				Entidade::gravitar(&this->movimento);

				this->y += anti_grav;

				this->y += this->movimento.y * Ente::g_dt;

				setPosicaoGrafica(this->x, this->y);
			}
        }
    }
}
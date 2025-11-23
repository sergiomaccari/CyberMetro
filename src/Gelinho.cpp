#include "Gelinho.h"
#include "Ente.h" 
#include "Jogador.h"
namespace CyberMetro {
    using namespace Entidades::Personagens;
    namespace Entidades {
        namespace Obstaculos
        {
            Gelinho::Gelinho(float xi, float yi) : Obstaculo(xi, yi), largura((rand() % 5)+1) {
                if (pGG)
                {
                    sf::Texture* tex = pGG->getTextura("assets/obstaculo_medio.png");
                    if (tex)
                    {
                        pFigura->setTexture(*tex);
                        pFigura->setScale(32.0f / tex->getSize().x, (32.0f + float(largura)) / tex->getSize().y);
                    }
                }
                else
                {
                    std::cerr << "Erro" << std::endl;
                }

                this->x = xi;
                this->y = yi;
                setPosicaoGrafica(this->x, this->y);
            }

            Gelinho::~Gelinho() 
            {
            
            }

            void Gelinho::obstaculizar(Personagens::Jogador* pj)
            {
                if (pj && pj->getObstaculoCooldown() <= sf::Time::Zero)
                {
                    pj->trava_mov(1.0f + 0.1*float(largura));
                    pj->iniciarObstaculoCooldown(sf::seconds(1.5 + 2.0f * float(largura)));
                }
            }

            void Gelinho::salvarDataBuffer(json& arq)
            {
                Obstaculo::salvarDataBuffer(arq);
                arq["tipo"] = "Gelinho";
                arq["largura"] = this->largura;
            }

            void Gelinho::carregarDeBuffer(const json& data)
            {
                Obstaculo::carregarDeBuffer(data);

                this->largura = data.value("largura", this->largura);
                if (pGG)
                {
                    sf::Texture* tex = pGG->getTextura("assets/obstaculo_medio.png");
                    if (tex)
                    {
                        pFigura->setTexture(*tex);
                        pFigura->setScale(32.0f / tex->getSize().x, (32.0f + float(largura)) / tex->getSize().y);
                    }
                }
            }

            void Gelinho::executar()
            {
                Entidade::gravitar(&this->movimento);
                
                this->y += this->movimento.y * Ente::g_dt;

                setPosicaoGrafica(this->x, this->y);
            }
        }
    }
}
#include "Robo_Senior.h"


namespace CyberMetro {
    namespace Entidades {
        namespace Personagens {

            Robo_Senior::Robo_Senior(float xi, float yi) :
                Inimigo(xi, yi),
                tamanho((rand() % 12) + 42),
                tempoProximaCura(0.0f)
            {
                this->xINI = xi;
                this->x = xi;
                this->y = yi;
                this->velocidade = 150.0f;
                this->n_vidas = 45;
                this->n_vidas_max = 45;
                this->vel_grav = 0.0010f;
                this->pontosPorMorte = 200;

                if (pGG)
                {
                    sf::Texture* tex = pGG->getTextura("assets/inimigo_medio.png");
                    if (tex)
                    {
                        pFigura->setTexture(*tex);
                        pFigura->setScale((float)tamanho / tex->getSize().x, (float)tamanho / tex->getSize().y);
                    }
                }
                else
                {
                    std::cerr << "Erro RoboSenior" << std::endl;
                }

                pFigura->setPosition(sf::Vector2f(this->x, this->y));
                atualizarBarraVida();
                this->intervaloCura = 10.0f - ((float)(this->tamanho - 42) / 11.0f * 5.0f);
                this->tempoProximaCura = Ente::g_tempoTotal + this->intervaloCura;
            }

            Robo_Senior::~Robo_Senior()
            {
            }

            void Robo_Senior::mover()
            {
                this->movimento = sf::Vector2f(0.0f, -1.0f);

                float dt = Ente::g_dt;

                Entidade::gravitar(&this->movimento);
                this->y += movimento.y * dt * velocidade;
                setPosicaoGrafica(this->x, this->y);
            }

            void Robo_Senior::danificar(Jogador* pJogador)
            {
                if (pJogador)
                    for (int i = 0; i < this->nivel_maldade; i++)
                        pJogador->operator--();
            }

            void Robo_Senior::salvarDataBuffer(json& arq)
            {
                Inimigo::salvarDataBuffer(arq);
                arq["tipo"] = "Robo_Senior";
                arq["xINI"] = this->xINI;
                arq["tamanho"] = this->tamanho;
                arq["tempoProximaCura"] = this->tempoProximaCura;
                arq["intervaloCura"] = this->intervaloCura;
            }

            void Robo_Senior::carregarDeBuffer(const json& data)
            {
                Inimigo::carregarDeBuffer(data);
                this->xINI = data.value("xINI", this->xINI);
                this->tamanho = data.value("tamanho", this->tamanho);
                this->intervaloCura = data.value("intervaloCura", 10.0f);
                this->tempoProximaCura = data.value("tempoProximaCura", Ente::g_tempoTotal + this->intervaloCura);


                if (pGG)
                {
                    sf::Texture* tex = pGG->getTextura("assets/inimigo_medio.png");
                    if (tex)
                    {
                        pFigura->setTexture(*tex);
                        pFigura->setScale((float)tamanho / tex->getSize().x, (float)tamanho / tex->getSize().y);
                    }
                }
            }


            void Robo_Senior::curar()
            {
                if (this->n_vidas < n_vidas_max) 
                    for(int i=0; i < 2*this->nivel_maldade; i++)
                        Personagem::operator++();
            }

            void Robo_Senior::executar()
            {
                this->mover();
                atualizarBarraVida();

                tempoProximaCura += Ente::g_dt;

                if (tempoProximaCura >= intervaloCura)
                {
                    curar();
                    tempoProximaCura = 0;
                }
            }
        }
    }
}
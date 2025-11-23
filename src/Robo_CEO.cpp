#include "Robo_CEO.h" 


namespace CyberMetro {
    namespace Entidades {
        namespace Personagens {
            const int Robo_CEO::COOLDOWN_TIRO_CHEFE_FRAMES = 200;

            Robo_CEO::Robo_CEO(float xi, float yi) :
                Inimigo(xi, yi),
                forca(rand() % 10 + 1),
                pJogador(nullptr),
                cooldownTiro(0),
                PROJETIL_VELOCIDADE_CHEFE((2.0f + 0.06f * (float)forca)*100)
            {
                cooldownTiro = COOLDOWN_TIRO_CHEFE_FRAMES / 2;

                this->x = xi;
                this->y = yi;
				this->velocidade = 0.0f;
                this->n_vidas = 60;
                this->n_vidas_max = 60;

                this->pontosPorMorte = 500;

                if (pGG)
                {
                    sf::Texture* tex = pGG->getTextura("assets/chefe.png");
                    if (tex)
                    {
                        pFigura->setTexture(*tex);
                        pFigura->setScale(64.0f / tex->getSize().x, 64.0f / tex->getSize().y);
                    }
                }
                else
                {
                    std::cerr << "Erro RoboCEO" << std::endl;
                }

                pFigura->setPosition(this->x, this->y);
                atualizarBarraVida();
            }

            Robo_CEO::~Robo_CEO()
            {

            }

            void Robo_CEO::setJogador(Jogador* j)
            {
                this->pJogador = j;
            }

            void Robo_CEO::mover()
            {
                this->movimento = sf::Vector2f(0.0f, 0.0f);

                float dt = Ente::g_dt;

                Entidade::gravitar(&this->movimento);

                this->y += movimento.y * dt * velocidade;

                setPosicaoGrafica(this->x, this->y);
            }

            void Robo_CEO::lancar()
            {
                if (cooldownTiro > 0)
                {
                    cooldownTiro--;
                    return;
                }
                cooldownTiro = COOLDOWN_TIRO_CHEFE_FRAMES;

                if (pJogador == nullptr)
                    return;

                Entidades::Projetil* p = new Entidades::Projetil();

                if (pGG) p->setGerenciadorGrafico(pGG);
                else p->setGerenciadorGrafico(Ente::getGerenciadorGrafico());

                sf::FloatRect corpoChefe = pFigura->getGlobalBounds();
                float CentroX = corpoChefe.left + corpoChefe.width / 2.0f;
                float CentroY = corpoChefe.top + corpoChefe.height / 2.0f;

                sf::FloatRect corpoJog = pJogador->getFigura()->getGlobalBounds();
                float AlvoX = corpoJog.left + corpoJog.width / 2.0f;
                float AlvoY = corpoJog.top + corpoJog.height / 2.0f;

                float dx = AlvoX - CentroX;
                float dy = AlvoY - CentroY;

                float mag = std::sqrt(dx * dx + dy * dy);

                float velocidadeTotal = PROJETIL_VELOCIDADE_CHEFE;

                if (mag > 0.0f)
                {
                    float vx = (dx / mag) * PROJETIL_VELOCIDADE_CHEFE;
                    float vy = (dy / mag) * PROJETIL_VELOCIDADE_CHEFE;
                    p->setVelocidade(vx, vy);
                }
                else
                {
                    p->setVelocidade(0.0f, velocidadeTotal);
                }

                p->setX(CentroX - 10.0f);
                p->setY(CentroY);
                p->setIdDono(0);

                projeteis.inserir(p);
            }

            void Robo_CEO::danificar(Jogador* pJogador)
            {
                if (pJogador)
                {
                    for (int i = 0; i < this->nivel_maldade; i++)
                        pJogador->operator--();
                }
            }
            void Robo_CEO::salvarDataBuffer(json& arq)
            {
                Inimigo::salvarDataBuffer(arq);
                arq["tipo"] = "Robo_CEO";
                arq["forca"] = this->forca;
                arq["cooldownTiro"] = this->cooldownTiro;
            }

            void Robo_CEO::carregarDeBuffer(const json& data)
            {
                Inimigo::carregarDeBuffer(data);

                this->forca = data.value("forca", this->forca);
                this->cooldownTiro = data.value("cooldownTiro", this->cooldownTiro);
                PROJETIL_VELOCIDADE_CHEFE = 1.2f + 0.06f * (float)forca;

                if (pGG)
                {
                    sf::Texture* tex = pGG->getTextura("assets/chefe.png");
                    if (tex)
                    {
                        pFigura->setTexture(*tex);
                        pFigura->setScale(64.0f / tex->getSize().x, 64.0f / tex->getSize().y);
                    }
                }
            }


            void Robo_CEO::executar()
            {
                this->mover();
                this->lancar();
                this->projeteis.executar();
                atualizarBarraVida();
            }

            Listas::ListaEntidades* Robo_CEO::getProjeteis()
            {
                return &projeteis;
            }
        }
    }
}
#include "Personagem.h"

namespace CyberMetro {
    namespace Entidades {
        namespace Personagens
        {
            const sf::Vector2f Personagem::TAMANHO_BARRA_VIDA(32.0f, 4.0f);

            Personagem::Personagem(float xi, float yi) :
                Entidade(xi, yi),
                n_vidas(1),
                n_vidas_max(1),
                velocidade(0.0)
            {
                BarraVidaFundo.setSize(TAMANHO_BARRA_VIDA);
                BarraVidaFundo.setFillColor(sf::Color(100, 0, 0));
                BarraVidaAtual.setSize(TAMANHO_BARRA_VIDA);
                BarraVidaAtual.setFillColor(sf::Color::Red);
            }

            Personagem::Personagem() :
                Entidade(0.f, 0.f),
                n_vidas(1),
                n_vidas_max(1),
                velocidade(0.0)
            {
                BarraVidaFundo.setSize(TAMANHO_BARRA_VIDA);
                BarraVidaFundo.setFillColor(sf::Color(100, 0, 0));
                BarraVidaAtual.setSize(TAMANHO_BARRA_VIDA);
                BarraVidaAtual.setFillColor(sf::Color::Red);
            }

            Personagem::~Personagem()
            {

            }

            void Personagem::salvarDataBuffer(json& arq)
            {
                Entidade::salvarDataBuffer(arq);
                arq["n_vidas"] = this->n_vidas;
                arq["n_vidas_max"] = this->n_vidas_max;
                arq["velocidade"] = this->velocidade;
            }

            void Personagem::carregarDeBuffer(const json& data)
            {
                Entidade::carregarDeBuffer(data);
                this->n_vidas = data.value("n_vidas", this->n_vidas);
                this->n_vidas_max = data.value("n_vidas_max", this->n_vidas_max);
                this->velocidade = data.value("velocidade", this->velocidade);
                atualizarBarraVida();
            }

            void Personagem::atualizarBarraVida()
            {
                if (n_vidas_max <= 0) return;

                float perc = (float)n_vidas / (float)n_vidas_max;
                if (perc < 0)
                {
                    perc = 0;
                }
                BarraVidaAtual.setSize(sf::Vector2f(TAMANHO_BARRA_VIDA.x * perc, TAMANHO_BARRA_VIDA.y));

                if (pFigura)
                {
                    sf::FloatRect boundsFig = pFigura->getGlobalBounds();
                    float posX = boundsFig.left + (boundsFig.width / 2.0f) - (TAMANHO_BARRA_VIDA.x / 2.0f);
                    float posY = boundsFig.top - TAMANHO_BARRA_VIDA.y - 2.0f;

                    BarraVidaFundo.setPosition(posX, posY);
                    BarraVidaAtual.setPosition(posX, posY);
                }
            }

            const sf::RectangleShape& Personagem::getBarraFundo() const
            {
                return BarraVidaFundo;
            }

            const sf::RectangleShape& Personagem::getBarraAtual() const
            {
                return BarraVidaAtual;
            }


            void Personagem::setMovimentoY(float yn)
            {
                this->movimento.y = yn;
            }

            sf::Vector2f Personagem::getMovimento()
            {
                return movimento;
            }

            void Personagem::setVelocidade(float vn)
            {
                this->velocidade = vn;
            }

            float Personagem::getVelocidade()
            {
                return velocidade;
            }

            void Personagem::operator++()
            {
                this->n_vidas++;
                if (n_vidas > n_vidas_max)
                {
                    n_vidas = n_vidas_max;
                }
                atualizarBarraVida();
            }

            void Personagem::operator--()
            {
                this->n_vidas--;
                if (this->n_vidas <= 0)
                {
                    this->n_vidas = 0;
                    this->setAtivo(false);
                }
                atualizarBarraVida();
            }
        }
    }
}
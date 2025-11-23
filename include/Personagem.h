#pragma once
#include "Entidade.h"


namespace CyberMetro {
    namespace Entidades {
        namespace Personagens
        {
            using json = nlohmann::json;

            class Personagem : public Entidades::Entidade {
            protected:
                int n_vidas;
                int n_vidas_max;
                float velocidade;
                sf::RectangleShape BarraVidaFundo;
                sf::RectangleShape BarraVidaAtual;
                static const sf::Vector2f TAMANHO_BARRA_VIDA;
                void atualizarBarraVida();

            public:
                Personagem(float xi, float yi);
                Personagem();
                virtual ~Personagem();
                void setMovimentoY(float yn);
                sf::Vector2f getMovimento();
                virtual void mover() = 0;

                virtual void salvarDataBuffer(json& arq);
                virtual void carregarDeBuffer(const json& data);

                float getVelocidade();
                void setVelocidade(float vn);
                void operator++();
                void operator--();
                const sf::RectangleShape& getBarraFundo() const;
                const sf::RectangleShape& getBarraAtual() const;
            };
        }
    }
}
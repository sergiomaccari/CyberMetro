#include "Robo_Senior.h"
#include <cmath>
#include <iostream>
#include "Jogador.h"

namespace Personagens {

    Robo_Senior::Robo_Senior(float xi, float yi) : Inimigo(xi, yi), tamanho((rand() % 12) + 42), volta(1), ALCANCE(70)
    {
        this->xINI = xi;
        this->x = xi;
        this->y = yi;
        this->velocidade = 300.0f;
        this->n_vidas = 2;
        if (pGG)
        {
            sf::Texture* tex = pGG->getTextura("Imagens/inimigo_medio.png");
            if (tex)
            {
                pFigura->setTexture(*tex);
                pFigura->setScale((float)tamanho / tex->getSize().x, (float)tamanho / tex->getSize().y);
            }
        }
        else
        {
            std::cerr << "Erro: Gerenciador Grafico nao setado para Robo_Senior!" << std::endl;
        }

        pFigura->setPosition(sf::Vector2f(this->x, this->y));
    }

    Robo_Senior::~Robo_Senior()
    {
    }

    void Robo_Senior::mover()
    {
        sf::Vector2f movimento = sf::Vector2f(0.0f, 0.0f);
        tempo = clock.restart();

        if (volta == 1 && this->x >= xINI + ALCANCE) {
            volta = -1;
        }
        else if (volta == -1 && this->x <= xINI) {
            volta = 1;
        }

        if (this->x <= x + ALCANCE && volta == 1)
        {
            movimento += sf::Vector2f(1.0f, 0.f);

        }

        if (this->x >= x && volta == -1)
        {
            movimento += sf::Vector2f(-1.0f, 0.f);
        }

        Personagem::gravidade(&movimento);

        this->x += movimento.x * tempo.asSeconds() * velocidade;
        this->y += movimento.y * tempo.asSeconds() * velocidade;

        setPosicaoGrafica(this->x, this->y);
    }

    void Robo_Senior::danificar(Jogador* pJogador)
    {
        if (pJogador)
        {
            for (int i = 0; i < this->nivel_maldade; i++)
                pJogador->operator--();
        }
    }
    void Robo_Senior::salvar()
    {

    }

    void Robo_Senior::executar()
    {
        this->mover();
    }
}
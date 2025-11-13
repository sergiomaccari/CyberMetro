#include "Robo_Senior.h"
#include <cmath>
#include <iostream>
#include "Jogador.h"

namespace Personagens {

    const float Robo_Senior::ALTURA_PULO = 64.0f;
    const float Robo_Senior::FORCA_PULO = -1.78885438f;

    Robo_Senior::Robo_Senior(float xi, float yi) :
        Inimigo(xi, yi),
        tamanho((rand() % 12) + 42),
        estaNoChao(true)
    {
        this->xINI = xi;
        this->x = xi;
        this->y = yi;
        this->velocidade = 300.0f;
        this->n_vidas = 2;
        this->n_vidas_max = 2; // define a vida maxima

        this->pontosPorMorte = 200;

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
            std::cerr << "Erro RoboSenior" << std::endl;
        }

        pFigura->setPosition(sf::Vector2f(this->x, this->y));
        atualizarBarraVida(); // inicializa a barra

        this->intervaloCura = sf::seconds(15.0f - ((float)(this->tamanho - 42) / 11.0f * 5.0f));//vai de 15 a 10 segundos
        this->tempoCura.restart();
    }

    Robo_Senior::~Robo_Senior()
    {
    }

    void Robo_Senior::mover()
    {
        tempo = clock.restart();

        if (vel_grav == 0.0f)
        {
            estaNoChao = true;
        }

        if (estaNoChao)
        {
            vel_grav = FORCA_PULO;
            estaNoChao = false;
        }

        vel_grav += grav;
        this->y += vel_grav;
        this->x = xINI;

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

    void Robo_Senior::atualar()
    {
        if (this->n_vidas < n_vidas_max) {// definir constante para n vida max e estudar a possibilidade de colcoar isso em uma outra função
            Personagem::operator++();
        }
    }

    void Robo_Senior::executar()
    {
        this->mover();
        atualizarBarraVida();

        if (tempoCura.getElapsedTime() > intervaloCura)
        {
            atualar();
            tempoCura.restart();
        }
    }
}
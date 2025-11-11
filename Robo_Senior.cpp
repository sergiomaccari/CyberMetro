#include "Robo_Senior.h"
#include <cmath>
#include <iostream>
#include "Jogador.h"

namespace Personagens {

    // v = sqrt(2 * g * h) // v = sqrt(2 * 0.025 * 32.0) = sqrt(1.6)
    // 0.025 eh o 'grav' de Entidade.h
    const float Robo_Senior::ALTURA_PULO = 32.0f;
    const float Robo_Senior::FORCA_PULO = -1.264911064f; // negativo eh para cima

    Robo_Senior::Robo_Senior(float xi, float yi) :
        Inimigo(xi, yi),
        tamanho((rand() % 12) + 42),
        estaNoChao(true) // comeca no chao
    {
        this->xINI = xi;
        this->x = xi;
        this->y = yi;
        this->velocidade = 300.0f; // nao eh mais usado no movimento
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
            std::cerr << "Erro RoboSenior" << std::endl;
        }

        pFigura->setPosition(sf::Vector2f(this->x, this->y));

        this->intervaloCura = sf::seconds(15.0f - ((float)(this->tamanho - 42) / 11.0f * 5.0f));//vai de 15 a 10 segundos
        this->tempoCura.restart();
    }

    Robo_Senior::~Robo_Senior()
    {
    }

    void Robo_Senior::mover()
    {
        // essa logica nao usa mais o vetor 'movimento' nem a 'velocidade'
        // ela aplica a gravidade diretamente no 'y'
        // essa eh a gravidade que eu quero implementar pra todo mundo
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

    void Robo_Senior::executar()
    {
        this->mover();
        if (tempoCura.getElapsedTime() > intervaloCura)
        {
            if (this->n_vidas < 2) {//PRESUMINDO QUE N VIDAS MENOR QUE 2, A CONSTANTE QUE LIMITE É A VIDA TOTAL, TALVEZ IMPLEMENTAR ALGO COMO UM GETTER
                Personagem::operator++();
            }
            tempoCura.restart();
        }
    }
}
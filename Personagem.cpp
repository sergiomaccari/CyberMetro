#pragma once
#include "Personagem.h"

namespace Personagens
{
    // padrao tamanho da barra de vida
    const sf::Vector2f Personagem::TAMANHO_BARRA_VIDA(32.0f, 4.0f);

    Personagem::Personagem(float xi, float yi) :
        Entidade(xi, yi),
        n_vidas(1),
        n_vidas_max(1),//padrao max vida
        movimento(0.0f, 0.0f),
        velocidade(0.0)
    {
        // configura a visualizacao das barras
        healthBarFundo.setSize(TAMANHO_BARRA_VIDA);
        healthBarFundo.setFillColor(sf::Color(100, 0, 0));//vermelho mais esatualo que mostra dano (fica legal confia)
        healthBarAtual.setSize(TAMANHO_BARRA_VIDA);
        healthBarAtual.setFillColor(sf::Color::Red); // vermelho mais claro pra indicar vida restante
    }

    Personagem::Personagem() :
        n_vidas(1),
        n_vidas_max(1),
        movimento(0.0f, 0.0f),
        velocidade(0.0)
    {
        healthBarFundo.setSize(TAMANHO_BARRA_VIDA);
        healthBarFundo.setFillColor(sf::Color(100, 0, 0));
        healthBarAtual.setSize(TAMANHO_BARRA_VIDA);
        healthBarAtual.setFillColor(sf::Color::Red);
    }

    Personagem::~Personagem()
    {

    }

    void Personagem::atualizarBarraVida()
    {
        if (n_vidas_max <= 0) return; // evita divisao por zero

        float perc = (float)n_vidas / (float)n_vidas_max;
        if (perc < 0) 
        { 
            perc = 0; 
        }
        healthBarAtual.setSize(sf::Vector2f(TAMANHO_BARRA_VIDA.x * perc, TAMANHO_BARRA_VIDA.y));

        if (pFigura)
        {
            sf::FloatRect boundsFig = pFigura->getGlobalBounds();
            float posX = boundsFig.left + (boundsFig.width / 2.0f) - (TAMANHO_BARRA_VIDA.x / 2.0f);//deixa barra centralizada
            float posY = boundsFig.top - TAMANHO_BARRA_VIDA.y - 2.0f;

            healthBarFundo.setPosition(posX, posY);
            healthBarAtual.setPosition(posX, posY);
        }
    }

    // gets
    const sf::RectangleShape& Personagem::getBarraFundo() const
    {
        return healthBarFundo;
    }

    const sf::RectangleShape& Personagem::getBarraAtual() const
    {
        return healthBarAtual;
    }


    void Personagem::setMovimentoY(float yn)
    {
        movimento.y = yn;
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
        if (n_vidas > n_vidas_max) n_vidas = n_vidas_max; // limita a atuala, mas como so o roboSenior usa podia limitar na função que sera feita pro metodo Curar
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
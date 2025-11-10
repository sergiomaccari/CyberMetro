#include "Robo_Junior.h"
#include "Jogador.h" 
#include <iostream>

using namespace Personagens;

Robo_Junior::Robo_Junior(float xi, float yi) : Inimigo(xi, yi), volta(1), ALCANCE(35), raio((float)(rand() % 12) + 28)
{
	this->xINI = xi;
	this->x = xi;
	this->y = yi;
	this->velocidade = 150.0f;
	this->n_vidas = 2;
	

	if (pGG)
	{
		sf::Texture* tex = pGG->getTextura("Imagens/inimigo_facil.png");
		if (tex)
		{
			pFigura->setTexture(*tex);
			pFigura->setScale((float) raio / tex->getSize().x, (float) raio / tex->getSize().y);
		}
	}
	else
	{
		std::cerr << "Erro: Gerenciador Grafico nao setado para Robo_Junior!" << std::endl;
	}

	pFigura->setPosition(sf::Vector2f(static_cast<float>(this->x), static_cast<float>(this->y)));
}
Robo_Junior::~Robo_Junior()
{

}

void Robo_Junior::mover()
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

	//pFigura->move(movimento * tempo.asSeconds() * velocidade);

	setPosicaoGrafica(this->x, this->y);
}

void Robo_Junior::danificar(Jogador* pJogador)
{
	if (pJogador)
	{
		for(int i=0; i < this->nivel_maldade; i++)
			pJogador->operator--();
	}
}
void Robo_Junior::salvar()
{

}

void Robo_Junior::executar()
{
	this->mover();
}
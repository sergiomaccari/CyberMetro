#include "Choquinho.h"

using namespace Personagens;

namespace Obstaculos {

	Choquinho::Choquinho(float xi, float yi) : Obstaculo(xi, yi)
	{

		danosidade = (rand() % 2) + 1;

		if (pGG)
		{
			sf::Texture* tex = pGG->getTextura("Imagens/obstaculo_dificil.png");
			if (tex)
			{
				pFigura->setTexture(*tex);
				pFigura->setScale(32.0f / tex->getSize().x, 32.0f / tex->getSize().y);
			}
		}
		else
		{
			std::cerr << "Erro Choquinho" << std::endl;
		}


		this->x = xi;
		this->y = yi;
		setPosicaoGrafica(this->x, this->y);

		this->danoso = true;
	}

	Choquinho::~Choquinho()
	{

	}

	void Choquinho::obstaculizar(Jogador* pj)
	{
		if (pj && pj->getObstaculoCooldown() <= sf::Time::Zero)
		{
			for (int i = 0; i < danosidade; i++) {
				pj->operator--();
			}
			pj->trava_mov(1.0f /(float) danosidade); // stun, quanto maior o dano menor o stunt

			pj->iniciarObstaculoCooldown(sf::seconds(1.0f));// cooldown
		}
	}

	void Choquinho::salvar() 
	{
	
	}

	void Choquinho::executar()
	{
		vel_grav += grav;
		this->y += vel_grav;
		setPosicaoGrafica(this->x, this->y);
	}
}
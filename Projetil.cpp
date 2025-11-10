#include "Projetil.h"
#include "Gerenciador_Grafico.h"

namespace Entidades
{//UTILIZAR DANO
	Projetil::Projetil() : dano(0), vx(0.0f), vy(0.0f), DoBem(true), FORCA_GRAVIDADE_PROJETIL(-0.0249f) {

		if (pGG)
		{
			sf::Texture* tex = pGG->getTextura("Imagens/projetil.png");
			if (tex)
			{
				pFigura->setTexture(*tex);
				pFigura->setScale(20.0f / tex->getSize().x, 20.0f / tex->getSize().y);
			}
		}
	}
	Projetil::~Projetil() {}

	void Projetil::executar() {
		vel_grav += grav + FORCA_GRAVIDADE_PROJETIL;
		this->x += vx;
		this->y += (vy + vel_grav);
		setPosicaoGrafica(this->x, this->y);
		if (this->x < 0.0f || this->x > 3200.0f || this->y < 0.0f || this->y > 900.0f)
		{
			this->setAtivo(false);
		}
	}

	void Projetil::salvar() 
	{
	
	}

	void Projetil::setVelocidade(float velX, float velY)
	{
		vx = velX;
		vy = velY;
		vel_grav = 0.0f;
	}

	void Projetil::setDoBem(bool b)
	{
		DoBem = b;
	}

	bool Projetil::getDoBem() const
	{
		return DoBem;
	}
}
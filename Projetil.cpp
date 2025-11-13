#include "Projetil.h"
#include "Gerenciador_Grafico.h"

namespace Entidades
{
	Projetil::Projetil() :
		dano(1),
		vx(0.0f),
		vy(0.0f),
		idDono(0), // padrao eh roboceo
		FORCA_GRAVIDADE_PROJETIL(-0.0249f) {

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
//agora projetil tem dono com um int pra roboceo, jog1, jog2
	void Projetil::setIdDono(int id)
	{
		idDono = id;
	}

	int Projetil::getIdDono() const
	{
		return idDono;
	}

	int Projetil::getDano() const
	{
		return dano;
	}
}
#include "Projetil.h"


namespace CyberMetro {
	namespace Entidades
	{
		const float Projetil::FORCA_GRAVIDADE_PROJETIL = 0.0001 - Entidade::grav;

		Projetil::Projetil() :
			Entidade(0.f, 0.f),
			dano(15),
			vx(0.0f),
			vy(0.0f),
			idDono(0)
		{

			if (pGG)
			{
				sf::Texture* tex = pGG->getTextura("assets/projetil.png");
				if (tex)
				{
					pFigura->setTexture(*tex);
					pFigura->setScale(20.0f / tex->getSize().x, 20.0f / tex->getSize().y);
				}
			}
		}
		Projetil::~Projetil() {}

		void Projetil::executar() {

			float dt = Ente::g_dt;
			this->x += vx * dt;
			this->y += vy * dt;
			this->movimento = sf::Vector2f(0.0f, 0.0f);
			Entidade::gravitar(&this->movimento);
			this->y += FORCA_GRAVIDADE_PROJETIL;

			setPosicaoGrafica(this->x, this->y);

			
			if (this->x < 0.0f || this->x > 3200.0f || this->y < 0.0f || this->y > 900.0f)//verifica se saiu da tela
			{
				this->setAtivo(false);
			}
		}

		void Projetil::salvarDataBuffer(json& arq)
		{
			Entidade::salvarDataBuffer(arq);
			arq["tipo"] = "Projetil";
			arq["vx"] = this->vx;
			arq["vy"] = this->vy;
			arq["idDono"] = this->idDono;

		}

		void Projetil::carregarDeBuffer(const json& data)
		{
			Entidade::carregarDeBuffer(data);
			this->vx = data.value("vx", this->vx);
			this->vy = data.value("vy", this->vy);
			this->idDono = data.value("idDono", this->idDono);
		}

		void Projetil::setVelocidade(float velX, float velY)
		{
			vx = velX;
			vy = velY;
		}
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
}
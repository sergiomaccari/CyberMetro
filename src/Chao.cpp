#include "Chao.h" 


namespace CyberMetro {
	namespace Entidades
	{
		const float Entidades::Chao::largura = 32.0f;
		const float Entidades::Chao::altura = 32.0f;

		Chao::Chao(float xi, float yi)
			: Entidade(xi, yi)
		{
			if (pGG)
			{
				pFigura->setTextureRect(sf::IntRect(0, 0, (int)Chao::largura, (int)Chao::altura));
			}
			else
			{
				std::cerr << "Erro Chao" << std::endl;
			}

			this->x = xi;
			this->y = yi;
			setPosicaoGrafica(this->x, this->y);
		}

		Chao::~Chao()
		{

		}

		void Chao::salvarDataBuffer(json& arq)
		{
			Entidade::salvarDataBuffer(arq);
			arq["tipo"] = "Chao";
		}

		void Chao::carregarDeBuffer(const json& data)
		{
			Entidade::carregarDeBuffer(data);
			if (pGG)
			{
				pFigura->setTextureRect(sf::IntRect(0, 0, (int)Chao::largura, (int)Chao::altura));
			}
		}

		void Chao::executar()
		{
			if (estaAtivo)
			{
				return;
			}
			bool fora_esquerda = (this->x + this->largura) < 0; // fora da tela na esquerda
			bool fora_direita = this->x > MUNDO_X_MAX; // fora do mapa na direita
			bool fora_cima = (this->y + this->altura) < 0; // encima da tela
			bool fora_baixo = this->y > ALTURA_TELA; //embaixo da tela

			if (fora_esquerda || fora_direita || fora_cima || fora_baixo)
			{
				this->setAtivo(false);
			}
		}
	}
}
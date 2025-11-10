#pragma once
#include "ListaEntidades.h"
#include "Gerenciador_Colisoes.h"
#include "Jogador.h"


using namespace Personagens;
using namespace Gerenciadores;

namespace Fases {
	class Fase : public Ente
	{
	protected:
		Jogador* pJogador1;
		Jogador* pJogador2;
		ListaEntidades* pListaInimigos;
		ListaEntidades* pListaObstaculos;
		Gerenciador_Colisoes* pColisoes;
		std::vector<sf::Vector2f> posi_robo_junior;
		std::vector<sf::Vector2f> posi_plataforma;
		const int TILESET_LARGURA_EM_BLOCOS = 8;
		const float TAMANHO_BLOCO_X = 32.0f;
		const float TAMANHO_BLOCO_Y = 32.0f;
		void criarRoboJunior(float x, float y);
		void criarRoboJunior(const sf::Vector2f& pos);
		void criarPlataforma(float x, float y);
		void criarPlataforma(const sf::Vector2f& pos);
		void criarChao(float x, float y);

	public:
		Fase(Jogador* jogador1, Jogador* jogador2 = nullptr);
		virtual ~Fase();
		virtual void executar();
		void limpar();
		virtual void adicionarJogador2(Jogador* j2);
		virtual void criarInimigos() = 0;
		virtual void criarObstaculos() = 0;
		void criarMapa();
		virtual void desenharMapa();
	};
}
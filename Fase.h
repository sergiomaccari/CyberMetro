#pragma once
#include "ListaEntidades.h"
#include "Gerenciador_Colisoes.h"
#include "Jogador.h"
#include <vector>
#include <SFML/System/Vector3.hpp> // Necessario para sf::Vector3f, eu achei esse o melhor jeito de fazer


using namespace Personagens;
using namespace Gerenciadores;

// declaracao antecipada para quebrar chamada circular
class Jogo;

namespace Fases {
	class Fase : public Ente
	{
	protected:

		sf::Texture texturaTileset;
		std::vector<std::vector<unsigned int>> gridMapa;

		Jogador* pJogador1;
		Jogador* pJogador2;

		ListaEntidades* pListaInimigos;
		ListaEntidades* pListaObstaculos;
		ListaEntidades* pListaChao;

		Gerenciador_Colisoes* pColisoes;

		std::vector<sf::Vector2f> posi_robo_junior;
		std::vector<sf::Vector3f> posi_plataforma;

		const int TILESET_LARGURA_EM_BLOCOS = 8;//pq cacete é 8? R: pq a imagem tem 256px e cada bloco 32px, 256/32=8
		const float TAMANHO_BLOCO_X = 32.0f;
		const float TAMANHO_BLOCO_Y = 32.0f;
		void criarRoboJunior(float x, float y);
		void criarRoboJunior(const sf::Vector2f& pos);
		void criarPlataforma(float x, float y, int altura);
		void criarChao(float x, float y);

		Jogo* pJogo;

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

		void setJogo(Jogo* pJ); // setter para o ponteiro do Jogo
	};
}
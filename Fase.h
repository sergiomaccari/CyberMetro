#pragma once
#include "ListaEntidades.h"
#include "Gerenciador_Colisoes.h"
#include "Jogador.h"
#include <vector> 
#include <SFML/System/Vector3.hpp> 

using namespace Personagens;
using namespace Gerenciadores;

class Jogo;

namespace Fases {
	class Fase : public Ente
	{
	protected:
		struct SpawnPlataforma {
			float x;
			float y;
			int altura;
		};

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

		sf::FloatRect areaDeSaida; 

		sf::Texture texturaSaida; 
		sf::Sprite spriteSaida;

		const int TILESET_LARGURA_EM_BLOCOS = 8;
		const float TAMANHO_BLOCO_X = 32.0f;
		const float TAMANHO_BLOCO_Y = 32.0f;
		void criarRoboJunior(float x, float y);
		void criarRoboJunior(const sf::Vector2f& pos);
		void criarPlataforma(float x, float y, int altura);
		void criarChao(float x, float y);

		Jogo* pJogo;

		void verificarFimDeFase();

		bool terminou;
		sf::Clock relogioFase;

	public:
		Fase(Jogador* jogador1, Jogador* jogador2 = nullptr);
		virtual ~Fase();

		virtual void executar() override;
		void desenharFase();

		void limpar();
		virtual void adicionarJogador2(Jogador* j2);
		virtual void criarInimigos() = 0;
		virtual void criarObstaculos() = 0;
		void criarCenario(const std::string& arquivoJson); 
		virtual void desenharMapa();

		void setJogo(Jogo* pJ);

		bool getTerminou() const;
		sf::FloatRect getAreaSaida() const;
		float getTempoDecorrido() const;
	};
}
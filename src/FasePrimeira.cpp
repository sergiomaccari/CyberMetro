#include "FasePrimeira.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Gelinho.h"
#include "Choquinho.h"
#include "Robo_Senior.h"
#include <algorithm> // para std::swap e std::min
#include <cstdlib>   // para rand()

using json = nlohmann::json;
using namespace CyberMetro::Entidades::Personagens;
using namespace CyberMetro::Entidades::Obstaculos;
using namespace CyberMetro::Gerenciadores;
namespace CyberMetro {
	namespace Fases {
		
		//constantes minimo e maximo de obstaculos e inimigos

		const int FasePrimeira::MIN_SENIOR = 3;
		const int FasePrimeira::MAX_SENIOR = 12;
		
		const int FasePrimeira::MIN_GELINHO = 3;
		const int FasePrimeira::MAX_GELINHO = 17;

		FasePrimeira::FasePrimeira(Entidades::Personagens::Jogador* jogador1, Entidades::Personagens::Jogador* jogador2, bool carregarSave)
			: Fase(jogador1, jogador2)
		{
			if (pJogador1) {
				pJogador1->resetar(32 * 1, 32 * 23);//reseta o jogador no começo da fase para a posição adequada
			}
			if (pJogador2 && pJogador2->getAtivo()) {
				pJogador2->resetar(pJogador1->getX(), pJogador1->getY());
			}
			this->areaDeSaida = sf::FloatRect(32.0f * 85.0f, 32.0f * 12.0f, 64.0f, 64.0f);//define area de saida
			this->spriteSaida.setPosition(this->areaDeSaida.left, this->areaDeSaida.top);
			sf::Vector2u texSize = this->texturaSaida.getSize();
			if (texSize.x > 0 && texSize.y > 0) {
				this->spriteSaida.setScale(
					this->areaDeSaida.width / texSize.x,
					this->areaDeSaida.height / texSize.y
				);
			}



			if (!texturaTileset.loadFromFile("assets/cyberpunk_floor_tiles_256x256_v3.png"))
			{
				std::cerr << "Erro: Nao foi possivel carregar o tileset " << std::endl;
			}

			//posições disponiveis para inimigos spawnnarem a seguir:

			posi_robo_junior = {
				{32 * 35, 32 * 1},
				{32 * 29, 32 * 4},
				{32 * 39, 32 * 6},
				{32 * 71, 32 * 6},
				{32 * 77, 32 * 6},
				{32 * 83, 32 * 6},
				{32 * 13, 32 * 9},
				{32 * 17, 32 * 9},
				{32 * 15, 32 * 13},
				{32 * 19, 32 * 13},
				{32 * 45, 32 * 18},
				{32 * 68, 32 * 18},
				{32 * 76, 32 * 18},
				{32 * 83, 32 * 18},
				{32 * 4, 32 * 23},
				{32 * 9, 32 * 23},
				{32 * 16, 32 * 23},
				{32 * 21, 32 * 23},
				{32 * 46, 32 * 23},
				{32 * 57, 32 * 23},
				{32 * 63, 32 * 23},
				{32 * 69, 32 * 23},
				{32 * 75, 32 * 23},
				{32 * 81, 32 * 23},
				{32 * 88, 32 * 23},
				{32 * 95, 32 * 23}
			};
			posi_robo_senior = {
				{32 * 9, 32 * 1},
				{32 * 15, 32 * 1},
				{32 * 18, 32 * 1},
				{32 * 24, 32 * 1},
				{32 * 50, 32 * 1},
				{32 * 56, 32 * 1},
				{32 * 63, 32 * 1},
				{32 * 70, 32 * 1},
				{32 * 77, 32 * 1},
				{32 * 60, 32 * 14},
				{32 * 66, 32 * 14},
				{32 * 72, 32 * 14},
				{32 * 78, 32 * 14},
				{32 * 28, 32 * 19}
			};
			posi_plataforma = {
				{32 * 1, 32 * 10, (float)(9 * 32)},
				{32 * 11, 32 * 16, (float)(3 * 32)},
				{32 * 24, 32 * 11, (float)(3 * 32)},
				{32 * 33, 32 * 19, (float)(5 * 32)},
				{32 * 55, 32 * 17, (float)(2 * 32)},
				{32 * 92, 32 * 12, (float)(7 * 32)},
				{32 * 98, 32 * 12, (float)(12 * 32)}
			};
			posi_gelinho = {
				{32 * 5, 32 * 1},
				{32 * 12, 32 * 1},
				{32 * 21, 32 * 1},
				{32 * 53, 32 * 1},
				{32 * 60, 32 * 1},
				{32 * 67, 32 * 1},
				{32 * 74, 32 * 1},
				{32 * 81, 32 * 1},
				{32 * 88, 32 * 1},
				{32 * 9, 32 * 10},
				{32 * 22, 32 * 10},
				{32 * 53, 32 * 10},
				{32 * 14, 32 * 23},
				{32 * 26, 32 * 23},
				{32 * 54, 32 * 23},
				{32 * 61, 32 * 23},
				{32 * 67, 32 * 23},
				{32 * 74, 32 * 23},
			};
			if (!carregarSave)
			{
				criarObstaculos();
				criarInimigos();
			}
			else
			{
				Fase::criarCenario("assets/tiledcybermapa1.json");
			}
		}

		FasePrimeira::~FasePrimeira() {}

		void FasePrimeira::criarInimigos()
		{
			int numJuniors = MIN_JUNIOR + (rand() % (MAX_JUNIOR - MIN_JUNIOR + 1));//define numero de entidades
			numJuniors = std::min(numJuniors, (int)posi_robo_junior.size());//medida anti-cagada

			for (int i = 0; i < numJuniors; ++i) {
				int j = i + (rand() % (posi_robo_junior.size() - i));
				std::swap(posi_robo_junior[i], posi_robo_junior[j]);//vai trocando aleatoriamente ate chegar na quantidade de inimigos
				criarRoboJunior(posi_robo_junior[i].x, posi_robo_junior[i].y);//depois cria
			}
			int numSeniors = MIN_SENIOR + (rand() % (MAX_SENIOR - MIN_SENIOR + 1));
			numSeniors = std::min(numSeniors, (int)(posi_robo_senior.size()));

			for (int i = 0; i < numSeniors; ++i) {
				int j = i + (rand() % ((int)(posi_robo_senior.size()) - i));
				std::swap(posi_robo_senior[i], posi_robo_senior[j]);
				criarRoboSenior(posi_robo_senior[i].x, posi_robo_senior[i].y);
			}
		}

		void FasePrimeira::criarObstaculos()
		{
			Fase::criarCenario("assets/tiledcybermapa1.json");
			int numPlataformas = MIN_PLATAFORMA + (rand() % (MAX_PLATAFORMA - MIN_PLATAFORMA + 1));
			numPlataformas = std::min(numPlataformas, (int)(posi_plataforma.size()));

			for (int i = 0; i < numPlataformas; ++i) {
				int j = i + (rand() % (posi_plataforma.size() - i));
				std::swap(posi_plataforma[i], posi_plataforma[j]);
				sf::Vector3f& spawn = posi_plataforma[i];
				criarPlataforma(spawn.x, spawn.y, (int)spawn.z);
			}

			int numGelinhos = MIN_GELINHO + (rand() % (MAX_GELINHO - MIN_GELINHO + 1));
			numGelinhos = std::min(numGelinhos, (int)(posi_gelinho.size()));

			for (int i = 0; i < numGelinhos; ++i) {
				int j = i + (rand() % (posi_gelinho.size() - i));
				std::swap(posi_gelinho[i], posi_gelinho[j]);
				criarGelinho(posi_gelinho[i].x, posi_gelinho[i].y);
			}
		}

		void FasePrimeira::desenharMapa()
		{
			Gerenciador_Grafico* pGG = Ente::getGerenciadorGrafico();
			if (!pGG || !pGG->getJanela())
			{
				return;
			}

			const int tilesetColunas = texturaTileset.getSize().x / (int)TAMANHO_BLOCO_X;//numero de colunas de tiles na imagem

			for (int y = 0; y < (int)gridMapa.size(); ++y)
			{
				for (int x = 0; x < (int)gridMapa[y].size(); ++x)
				{
					unsigned int id = gridMapa[y][x];
					if (id == 0)//o criarMapa já leu o arquivo e marcou que aqui é 0 pq n tem nada
					{
						continue;//quebra loop
					}

					int idAjustado = id - 1;//array começa com 0 e tiled começa com 1
					int tileX = (idAjustado % tilesetColunas) * (int)TAMANHO_BLOCO_X;//coluna do tile dentro do tile set * tamanho para converter isso em pixels
					int tileY = (idAjustado / tilesetColunas) * (int)TAMANHO_BLOCO_Y;//esses tileX e Y são o canto superior esquerdo do tile

					sf::Sprite tile(texturaTileset);//cria um sprite 
					tile.setTextureRect(sf::IntRect(tileX, tileY, (int)TAMANHO_BLOCO_X, (int)TAMANHO_BLOCO_Y));//ajusta pro tile
					tile.setPosition(x * TAMANHO_BLOCO_X, y * TAMANHO_BLOCO_Y);//coloca no lugar certo
					pGG->getJanela()->draw(tile);//desenhar o tile
				}
			}
		}


		void FasePrimeira::criarRoboSenior(float x, float y)
		{
			Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
			if (!pGG_local)
			{
				return;
			}
			Entidades::Personagens::Robo_Senior* inimigo = new Entidades::Personagens::Robo_Senior(x, y);
			inimigo->setGerenciadorGrafico(pGG_local);
			pListaInimigos->inserir(inimigo);
		}

		void FasePrimeira::criarGelinho(float x, float y)
		{
			Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
			if (!pGG_local) 
			{
				return;
			}

			Entidades::Obstaculos::Gelinho* obst = new Entidades::Obstaculos::Gelinho(x, y);
			obst->setGerenciadorGrafico(pGG_local);
			pListaObstaculos->inserir(obst);
		}
	}
}
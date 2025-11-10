#include "FasePrimeira.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Gelinho.h"
#include "Choquinho.h"
#include "Robo_Senior.h"


using json = nlohmann::json;
using namespace Personagens;

namespace Fases {

	FasePrimeira::FasePrimeira(Jogador* jogador1, Jogador* jogador2)
		: Fase(jogador1, jogador2)
	{
		if (!texturaTileset.loadFromFile("cyberpunk_floor_tiles_256x256_v3.png"))
		{
			std::cerr << "Erro: Nao foi possivel carregar o tileset " << std::endl;
		}

		posi_robo_junior = { {2336, 700}, {2336, 288}, {480, 600}, {480, 700} };
		posi_robo_senior = { {1280, 550}, {2336, 600}, {2336, 298}, {1600, 550} };
		posi_plataforma = { {1344, 704}, {2304, 416}, {2336, 416}, {2368, 416} };
		posi_gelinho = { {1536, 608}, {1568, 608}, {1600, 608}, {1632, 608} };

		criarObstaculos();
		criarInimigos();
	}

	FasePrimeira::~FasePrimeira() {}

	void FasePrimeira::criarInimigos()
	{
		criarRoboJunior(1600, 64);
		criarRoboJunior(1600, 360);
		criarRoboJunior(1900, 64);

		criarRoboSenior(700, 600);
		criarRoboSenior(160, 700);
		criarRoboSenior(160, 600);

		int na = rand() % 5;

		for (int i = 0; i < na; ++i) {
			int j = rand() % 4;
			int k = rand() % 4;
			sf::Vector2f posIF = posi_robo_junior[j];
			sf::Vector2f posIM = posi_robo_senior[k];
			criarRoboSenior(posIM.x, posIM.y);
			criarRoboJunior(posIF.x, posIF.y);

		}
	}

	void FasePrimeira::criarObstaculos()
	{
		int na = rand() % 1;
		criarMapa();
		criarGelinho(672, 736);
		criarGelinho(1300, 736);
		criarPlataforma(1248, 704);

		for (int i = 0; i < na; i++)
		{
			criarGelinho(posi_gelinho[i].x, posi_gelinho[i].y);
			criarPlataforma(posi_plataforma[i].x, posi_plataforma[i].y);
		}
	}

	void FasePrimeira::criarMapa()
	{
		std::ifstream file("tiledcyber3.json");
		if (!file.is_open()) {
			std::cerr << "Erro: Nao foi possivel abrir 'tiledcyber3.json'" << std::endl;
			return;
		}

		json mapa;
		file >> mapa;
		file.close();

		int tileWidth = mapa["tilewidth"];
		int tileHeight = mapa["tileheight"];
		int width = mapa["width"];
		int height = mapa["height"];

		gridMapa.assign(height, std::vector<unsigned int>(width, 0));

		auto data = mapa["layers"][0]["data"];
		int index = 0;
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				gridMapa[y][x] = data[index++];
			}
		}

		auto vazio = [&](int ry, int rx) {
			if (ry < 0 || ry >= height || rx < 0 || rx >= width) return true;
			return gridMapa[ry][rx] == 0;
			};

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				if (gridMapa[y][x] != 0) {
					bool borda = vazio(y - 1, x) || vazio(y + 1, x) || vazio(y, x - 1) || vazio(y, x + 1);
					if (borda) {
						float posX = x * TAMANHO_BLOCO_X;
						float posY = y * TAMANHO_BLOCO_Y;
						Fase::criarChao(posX, posY);
					}
				}
			}
		}
	}

	void FasePrimeira::desenharMapa()
	{
		Gerenciador_Grafico* pGG = Ente::getGerenciadorGrafico();
		if (!pGG || !pGG->getJanela()) return;

		const int tilesetCols = texturaTileset.getSize().x / (int)TAMANHO_BLOCO_X;

		for (int y = 0; y < (int)gridMapa.size(); ++y)
		{
			for (int x = 0; x < (int)gridMapa[y].size(); ++x)
			{
				unsigned int id = gridMapa[y][x];
				if (id == 0)
					continue;

				int idAjustado = id - 1;
				int tileX = (idAjustado % tilesetCols) * (int)TAMANHO_BLOCO_X;
				int tileY = (idAjustado / tilesetCols) * (int)TAMANHO_BLOCO_Y;

				sf::Sprite tile(texturaTileset);
				tile.setTextureRect(sf::IntRect(tileX, tileY, (int)TAMANHO_BLOCO_X, (int)TAMANHO_BLOCO_Y));
				tile.setPosition(x * TAMANHO_BLOCO_X, y * TAMANHO_BLOCO_Y);

				pGG->getJanela()->draw(tile);
			}
		}
	}


	void FasePrimeira::criarRoboSenior(float x, float y)
	{
		Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
		if (!pGG_local) return;

		Personagens::Robo_Senior* inimigo = new Personagens::Robo_Senior(x, y);
		inimigo->setGerenciadorGrafico(pGG_local);
		pListaInimigos->inserir(inimigo);
	}

	void FasePrimeira::criarGelinho(float x, float y)
	{
		Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
		if (!pGG_local) return;

		Obstaculos::Gelinho* obst = new Obstaculos::Gelinho(x, y);
		obst->setGerenciadorGrafico(pGG_local);
		pListaObstaculos->inserir(obst);
	}

	void FasePrimeira::criarChoquinho(float x, float y)
	{
		Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
		if (!pGG_local) return;

		Obstaculos::Choquinho* obst = new Obstaculos::Choquinho(x, y);
		obst->setGerenciadorGrafico(pGG_local);
		pListaObstaculos->inserir(obst);
	}


}
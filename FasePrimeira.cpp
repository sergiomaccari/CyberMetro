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
using namespace Personagens;

namespace Fases {

	// Definicao das constantes
	const int FasePrimeira::MIN_JUNIOR = 3;
	const int FasePrimeira::MAX_JUNIOR = 5;
	const int FasePrimeira::MIN_SENIOR = 3;
	const int FasePrimeira::MAX_SENIOR = 5;
	const int FasePrimeira::MIN_PLATAFORMA = 3;
	const int FasePrimeira::MAX_PLATAFORMA = 5;
	const int FasePrimeira::MIN_GELINHO = 3;
	const int FasePrimeira::MAX_GELINHO = 5;

	FasePrimeira::FasePrimeira(Jogador* jogador1, Jogador* jogador2)
		: Fase(jogador1, jogador2)
	{
		// --- LOGICA DE SPAWN ADICIONADA AQUI ---
		if (pJogador1) {
			pJogador1->setX(32 * 1); // Posicao da Fase 1
			pJogador1->setY(32 * 24);
			pJogador1->setPosicaoGrafica(pJogador1->getX(), pJogador1->getY());
		}
		if (pJogador2 && pJogador2->getAtivo()) {
			pJogador2->setX(pJogador1->getX() + 50.0f); // Perto do P1
			pJogador2->setY(pJogador1->getY());
			pJogador2->setPosicaoGrafica(pJogador2->getX(), pJogador2->getY());
		}
		// --- FIM DA LOGICA DE SPAWN ---


		if (!texturaTileset.loadFromFile("cyberpunk_floor_tiles_256x256_v3.png"))
		{
			std::cerr << "Erro: Nao foi possivel carregar o tileset " << std::endl;
		}

		// --- CARREGA VETORES DE SPAWN ---
		// Posições de Robo Junior (herdado de Fase)
		posi_robo_junior = {
			{32 * 35, 32 * 2}, {32 * 29, 32 * 5}, {32 * 39, 32 * 7}, {32 * 71, 32 * 7},
			{32 * 77, 32 * 7}, {32 * 83, 32 * 7}, {32 * 13, 32 * 10}, {32 * 17, 32 * 10},
			{32 * 15, 32 * 14}, {32 * 19, 32 * 14}, {32 * 45, 32 * 19}, {32 * 68, 32 * 19},
			{32 * 76, 32 * 19}, {32 * 83, 32 * 19}, {32 * 4, 32 * 24}, {32 * 9, 32 * 24},
			{32 * 16, 32 * 24}, {32 * 21, 32 * 24}, {32 * 46, 32 * 24}, {32 * 51, 32 * 24},
			{32 * 57, 32 * 24}, {32 * 63, 32 * 24}, {32 * 69, 32 * 24}, {32 * 75, 32 * 24},
			{32 * 81, 32 * 24}, {32 * 87, 32 * 24}, {32 * 93, 32 * 24}
		};
		// Posições de Robo Senior (especifico da Fase 1)
		posi_robo_senior = {
			{32 * 9, 32 * 2}, {32 * 15, 32 * 2}, {32 * 18, 32 * 2}, {32 * 24, 32 * 2},
			{32 * 50, 32 * 2}, {32 * 56, 32 * 2}, {32 * 63, 32 * 2}, {32 * 70, 32 * 2},
			{32 * 77, 32 * 2}, {32 * 60, 32 * 14}, {32 * 66, 32 * 14}, {32 * 72, 32 * 14},
			{32 * 78, 32 * 14}, {32 * 28, 32 * 20}
		};

		// Posições de Plataforma (herdado de Fase)
		posi_plataforma = {
			{32 * 1, 32 * 11, (float)(9 * 32)}, {32 * 11, 32 * 17, (float)(3 * 32)}, {32 * 24, 32 * 12, (float)(3 * 32)},
			{32 * 33, 32 * 20, (float)(5 * 32)}, {32 * 55, 32 * 18, (float)(2 * 32)}, {32 * 92, 32 * 13, (float)(7 * 32)},
			{32 * 98, 32 * 13, (float)(12 * 32)}
		};

		// Posições de Gelinho (especifico da Fase 1)
		posi_gelinho = {
			{32 * 5, 32 * 2}, {32 * 12, 32 * 2}, {32 * 21, 32 * 2}, {32 * 53, 32 * 2},
			{32 * 60, 32 * 2}, {32 * 67, 32 * 2}, {32 * 74, 32 * 2}, {32 * 81, 32 * 2},
			{32 * 88, 32 * 2}, {32 * 9, 32 * 10}, {32 * 22, 32 * 10}, {32 * 53, 32 * 11},
			{32 * 14, 32 * 24}, {32 * 26, 32 * 24}, {32 * 53, 32 * 24}, {32 * 60, 32 * 24},
			{32 * 67, 32 * 24}, {32 * 74, 32 * 24}, {32 * 81, 32 * 24}, {32 * 88, 32 * 24},
			{32 * 95, 32 * 24}
		};
		// --- FIM DA CARGA ---

		criarObstaculos();
		criarInimigos();
	}

	FasePrimeira::~FasePrimeira() {}

	void FasePrimeira::criarInimigos()
	{
		// --- LOGICA DE SPAWN ALEATORIO PARA ROBO JUNIOR ---
		int totalPosJuniors = posi_robo_junior.size();
		int numJuniors = MIN_JUNIOR + (rand() % (MAX_JUNIOR - MIN_JUNIOR + 1));
		numJuniors = std::min(numJuniors, totalPosJuniors);

		for (int i = 0; i < numJuniors; ++i) {
			int j = i + (rand() % (totalPosJuniors - i));
			std::swap(posi_robo_junior[i], posi_robo_junior[j]);
			criarRoboJunior(posi_robo_junior[i].x, posi_robo_junior[i].y);
		}

		// --- LOGICA DE SPAWN ALEATORIO PARA ROBO SENIOR ---
		int totalPosSeniors = posi_robo_senior.size();
		int numSeniors = MIN_SENIOR + (rand() % (MAX_SENIOR - MIN_SENIOR + 1));
		numSeniors = std::min(numSeniors, totalPosSeniors);

		for (int i = 0; i < numSeniors; ++i) {
			int j = i + (rand() % (totalPosSeniors - i));
			std::swap(posi_robo_senior[i], posi_robo_senior[j]);
			criarRoboSenior(posi_robo_senior[i].x, posi_robo_senior[i].y);
		}
	}

	void FasePrimeira::criarObstaculos()
	{
		criarMapa();

		// --- LOGICA DE SPAWN ALEATORIO PARA PLATAFORMA ---
		int totalPosPlataformas = posi_plataforma.size();
		int numPlataformas = MIN_PLATAFORMA + (rand() % (MAX_PLATAFORMA - MIN_PLATAFORMA + 1));
		numPlataformas = std::min(numPlataformas, totalPosPlataformas);

		for (int i = 0; i < numPlataformas; ++i) {
			int j = i + (rand() % (totalPosPlataformas - i));
			std::swap(posi_plataforma[i], posi_plataforma[j]);

			// Acessamos os dados com .x, .y, .z
			sf::Vector3f& spawn = posi_plataforma[i];

			// fazemos o cast de 'z' (float) de volta para 'int'
			criarPlataforma(spawn.x, spawn.y, (int)spawn.z);
		}

		// --- LOGICA DE SPAWN ALEATORIO PARA GELINHO ---
		int totalPosGelinhos = posi_gelinho.size();
		int numGelinhos = MIN_GELINHO + (rand() % (MAX_GELINHO - MIN_GELINHO + 1));
		numGelinhos = std::min(numGelinhos, totalPosGelinhos);

		for (int i = 0; i < numGelinhos; ++i) {
			int j = i + (rand() % (totalPosGelinhos - i));
			std::swap(posi_gelinho[i], posi_gelinho[j]);
			criarGelinho(posi_gelinho[i].x, posi_gelinho[i].y);
		}
	}

	void FasePrimeira::criarMapa()
	{
		std::ifstream file("tiledcyber3.json");//armazenar futuramente como constante
		if (!file.is_open()) {
			std::cerr << "Erro: nao abriu 'tiledcyber3.json'" << std::endl;
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
}
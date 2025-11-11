#include "FaseSegunda.h"
#include "Plataforma.h"
#include "Robo_Junior.h"
#include "Chao.h"
#include "Gerenciador_Grafico.h"
#include "Choquinho.h"
#include "Gelinho.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <algorithm> // para std::swap e std::min
#include <cstdlib>   // para rand()

using json = nlohmann::json;
using namespace Personagens;

namespace Fases {

    // Definicao das constantes
    const int FaseSegunda::MIN_JUNIOR = 3;
    const int FaseSegunda::MAX_JUNIOR = 5;
    const int FaseSegunda::MIN_CEO = 3;
    const int FaseSegunda::MAX_CEO = 5;
    const int FaseSegunda::MIN_PLATAFORMA = 3;
    const int FaseSegunda::MAX_PLATAFORMA = 5;
    const int FaseSegunda::MIN_CHOQUINHO = 3;
    const int FaseSegunda::MAX_CHOQUINHO = 5;


    FaseSegunda::FaseSegunda(Jogador* jogador1, Jogador* jogador2)
        : Fase(jogador1, jogador2)
    {
        // --- LOGICA DE SPAWN ADICIONADA AQUI ---
        if (pJogador1) {
            pJogador1->setX(32 * 2); // Posicao da Fase 2
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
            std::cerr << "Erro:nao carregou o tileset " << std::endl;
        }

        // --- CARREGA VETORES DE SPAWN ---
        // (descomentando e usando os vetores herdados e proprios)
        posi_ceo = {
            {3200 - (32 * 11), 32 * 10}, {3200 - (32 * 11), 32 * 15}, {3200 - (32 * 11), 32 * 20},
            {3200 - (32 * 11), 32 * 25}, {32 * 28, 32 * 25}
        };

        posi_robo_junior = {
            {32 * 37, 32 * 1}, {32 * 41, 32 * 1}, {32 * 47, 32 * 1}, {32 * 53, 32 * 1},
            {32 * 59, 32 * 1}, {32 * 65, 32 * 1}, {32 * 71, 32 * 1}, {32 * 77, 32 * 1},
            {32 * 83, 32 * 1}, {32 * 89, 32 * 1}, {32 * 95, 32 * 1}, {32 * 6, 32 * 2},
            {32 * 11, 32 * 2}, {32 * 18, 32 * 2}, {32 * 41, 32 * 6}, {32 * 47, 32 * 6},
            {32 * 53, 32 * 6}, {32 * 59, 32 * 6}, {32 * 65, 32 * 6}, {32 * 71, 32 * 6},
            {32 * 77, 32 * 6}, {32 * 83, 32 * 6}, {32 * 89, 32 * 6}, {32 * 95, 32 * 6},
            {32 * 11, 32 * 7}, {32 * 15, 32 * 7}, {32 * 21, 32 * 7}, {32 * 37, 32 * 11},
            {32 * 41, 32 * 11}, {32 * 47, 32 * 11}, {32 * 11, 32 * 12}, {32 * 15, 32 * 12},
            {32 * 21, 32 * 12}, {32 * 39, 32 * 16}, {32 * 51, 32 * 16}, {32 * 57, 32 * 16},
            {32 * 11, 32 * 17}, {32 * 15, 32 * 17}, {32 * 21, 32 * 17}, {32 * 37, 32 * 21},
            {32 * 49, 32 * 21}, {32 * 41, 32 * 26}, {32 * 47, 32 * 26}, {32 * 71, 32 * 26}
        };

        posi_plataforma = {
            {32 * 3, 32 * 14, 13 * 32}, {32 * 32, 32 * 14, 13 * 32}, {32 * 34, 32 * 14, 13 * 32},
            {32 * 65, 32 * 18, 9 * 32}, {32 * 67, 32 * 18, 9 * 32}
        };

        posi_choquinho = {
            {32 * 45, 32 * 1}, {32 * 57, 32 * 1}, {32 * 69, 32 * 1}, {32 * 81, 32 * 1},
            {32 * 93, 32 * 1}, {32 * 16, 32 * 2}, {32 * 26, 32 * 2}, {32 * 45, 32 * 6},
            {32 * 57, 32 * 6}, {32 * 69, 32 * 6}, {32 * 81, 32 * 6}, {32 * 93, 32 * 6},
            {32 * 9, 32 * 7}, {32 * 26, 32 * 7}, {32 * 49, 32 * 11}, {32 * 9, 32 * 12},
            {32 * 26, 32 * 12}, {32 * 49, 32 * 16}, {32 * 55, 32 * 16}, {32 * 9, 32 * 17},
            {32 * 26, 32 * 17}, {32 * 47, 32 * 21}, {32 * 55, 32 * 21}, {32 * 9, 32 * 26}
        };

        criarObstaculos();
        criarInimigos();
    }

    FaseSegunda::~FaseSegunda()
    {

    }

    void FaseSegunda::criarInimigos()
    {
        int totalPosJuniors = posi_robo_junior.size();
        int numJuniors = MIN_JUNIOR + (rand() % (MAX_JUNIOR - MIN_JUNIOR + 1));
        numJuniors = std::min(numJuniors, totalPosJuniors);

        for (int i = 0; i < numJuniors; ++i) {
            int j = i + (rand() % (totalPosJuniors - i));
            std::swap(posi_robo_junior[i], posi_robo_junior[j]);
            criarRoboJunior(posi_robo_junior[i].x, posi_robo_junior[i].y);
        }

        // --- LOGICA DE SPAWN ALEATORIO PARA ROBO CEO ---
        int totalPosCeos = posi_ceo.size();
        int numCeos = MIN_CEO + (rand() % (MAX_CEO - MIN_CEO + 1));
        numCeos = std::min(numCeos, totalPosCeos);

        for (int i = 0; i < numCeos; ++i) {
            int j = i + (rand() % (totalPosCeos - i));
            std::swap(posi_ceo[i], posi_ceo[j]);
            criarRoboCEO(posi_ceo[i].x, posi_ceo[i].y);
        }
    }

    void FaseSegunda::criarObstaculos()
    {
        criarMapa();

        // --- LOGICA DE SPAWN ALEATORIO PARA PLATAFORMA (HERDADO) ---
        int totalPosPlataformas = posi_plataforma.size();
        int numPlataformas = MIN_PLATAFORMA + (rand() % (MAX_PLATAFORMA - MIN_PLATAFORMA + 1));
        numPlataformas = std::min(numPlataformas, totalPosPlataformas);

        for (int i = 0; i < numPlataformas; ++i) {
            int j = i + (rand() % (totalPosPlataformas - i));
            std::swap(posi_plataforma[i], posi_plataforma[j]);

            sf::Vector3f& spawn = posi_plataforma[i];
            criarPlataforma(spawn.x, spawn.y, (int)spawn.z);
        }

        // --- LOGICA DE SPAWN ALEATORIO PARA CHOQUINHO ---
        int totalPosChoquinhos = posi_choquinho.size();
        int numChoquinhos = MIN_CHOQUINHO + (rand() % (MAX_CHOQUINHO - MIN_CHOQUINHO + 1));
        numChoquinhos = std::min(numChoquinhos, totalPosChoquinhos);

        for (int i = 0; i < numChoquinhos; ++i) {
            int j = i + (rand() % (totalPosChoquinhos - i));
            std::swap(posi_choquinho[i], posi_choquinho[j]);
            criarChoquinho(posi_choquinho[i].x, posi_choquinho[i].y);
        }
    }

    void FaseSegunda::criarMapa()
    {
        std::ifstream file("tiledcyberSeg.json");
        if (!file.is_open()) {
            std::cerr << "Erro: nao abriu 'tiledcyberSeg.json'" << std::endl;
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

    void FaseSegunda::desenharMapa()
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

    void FaseSegunda::criarRoboCEO(float x, float y)
    {
        Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
        if (!pGG_local) return;

        Personagens::Robo_CEO* chefe = new Personagens::Robo_CEO(x, y);
        chefe->setGerenciadorGrafico(pGG_local);
        chefe->setJogador(pJogador1);
        pListaInimigos->inserir(chefe);
    }

    void FaseSegunda::criarChoquinho(float x, float y)
    {
        Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
        if (!pGG_local) return;

        Obstaculos::Choquinho* obst = new Obstaculos::Choquinho(x, y);
        obst->setGerenciadorGrafico(pGG_local);
        pListaObstaculos->inserir(obst);
    }

}
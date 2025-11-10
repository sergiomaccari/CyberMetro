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

using json = nlohmann::json;
using namespace Personagens;

namespace Fases {
    FaseSegunda::FaseSegunda(Jogador* jogador1, Jogador* jogador2)
        : Fase(jogador1, jogador2)
    {
        if (!texturaTileset.loadFromFile("cyberpunk_floor_tiles_256x256_v3.png"))
        {
            std::cerr << "Erro: Nao foi possivel carregar o tileset " << std::endl;
        }

        posi_robo_junior = { {1000, 500}, {1200, 400} };
        posi_gelinho = { {800, 600}, {900, 600} };
        posi_choquinho = { {1500, 550}, {1600, 550} };
        posi_ceo = { {2000, 400} };

        criarObstaculos();
        criarInimigos();
    }

    FaseSegunda::~FaseSegunda() {}

    void FaseSegunda::criarInimigos()
    {
        for (const auto& pos : posi_robo_junior) {
            criarRoboJunior(pos.x, pos.y);
        }

        for (const auto& pos : posi_ceo) {
            criarChefe(pos.x, pos.y);
        }
    }

    void FaseSegunda::criarObstaculos()
    {
        criarMapa();

        for (const auto& pos : posi_gelinho) {
            criarGelinho(pos.x, pos.y);
        }

        for (const auto& pos : posi_choquinho) {
            criarChoquinho(pos.x, pos.y);
        }
    }

    void FaseSegunda::criarMapa()
    {
        std::ifstream file("tiledcyberSeg.json");
        if (!file.is_open()) {
            std::cerr << "Erro: Nao foi possivel abrir 'tiledcyberSeg.json'" << std::endl;
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

    void FaseSegunda::criarChefe(float x, float y)
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

    void FaseSegunda::criarGelinho(float x, float y)
    {
        Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
        if (!pGG_local) return;

        Obstaculos::Gelinho* obst = new Obstaculos::Gelinho(x, y);
        obst->setGerenciadorGrafico(pGG_local);
        pListaObstaculos->inserir(obst);
    }
}
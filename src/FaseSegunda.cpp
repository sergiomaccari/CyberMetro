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
#include <algorithm> //std::swap e std::min
#include <cstdlib>//rand()

using json = nlohmann::json;
using namespace CyberMetro::Entidades::Personagens;
using namespace CyberMetro::Entidades::Obstaculos;
using namespace CyberMetro::Gerenciadores;
namespace CyberMetro {
    namespace Fases {

        //constantes minimo e maximo de obstaculos e inimigos

        const int FaseSegunda::MIN_CEO = 3;
        const int FaseSegunda::MAX_CEO = 5;
        
        const int FaseSegunda::MIN_CHOQUINHO = 3;
        const int FaseSegunda::MAX_CHOQUINHO = 21;

        FaseSegunda::FaseSegunda(Entidades::Personagens::Jogador* jogador1, Entidades::Personagens::Jogador* jogador2, bool carregarSave)
            : Fase(jogador1, jogador2)
        {
            if (pJogador1) {
                pJogador1->resetar(32 * 1, 32 * 26);
            }
            if (pJogador2 && pJogador2->getAtivo()) {
                pJogador2->resetar(pJogador1->getX() + 50.0f, pJogador1->getY());
            }

            this->areaDeSaida = sf::FloatRect(32.0f * 97.0f, 32.0f * 10.0f, 64.0f, 64.0f);
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

            posi_ceo = {
                {3200 - (32 * 10), 32 * 10},
                {3200 - (32 * 10), 32 * 15},
                {3200 - (32 * 10), 32 * 20},
                {3200 - (32 * 10), 32 * 25},
                {32 * 28, 32 * 25}
            };

            posi_robo_junior = {
                {32 * 37, 32 * 1},
                {32 * 41, 32 * 1},
                {32 * 47, 32 * 1},
                {32 * 53, 32 * 1},
                {32 * 59, 32 * 1},
                {32 * 65, 32 * 1},
                {32 * 71, 32 * 1},
                {32 * 77, 32 * 1},
                {32 * 83, 32 * 1},
                {32 * 89, 32 * 1},
                {32 * 95, 32 * 1},
                {32 * 6, 32 * 1},
                {32 * 11, 32 * 1},
                {32 * 18, 32 * 1},
                {32 * 41, 32 * 6},
                {32 * 47, 32 * 6},
                {32 * 53, 32 * 6},
                {32 * 59, 32 * 6},
                {32 * 65, 32 * 6},
                {32 * 71, 32 * 6},
                {32 * 77, 32 * 6},
                {32 * 83, 32 * 6},
                {32 * 11, 32 * 6},
                {32 * 15, 32 * 6},
                {32 * 21, 32 * 6},
                {32 * 37, 32 * 11},
                {32 * 11, 32 * 11},
                {32 * 15, 32 * 11},
                {32 * 21, 32 * 11},
                {32 * 39, 32 * 15},
                {32 * 51, 32 * 15},
                {32 * 11, 32 * 15},
                {32 * 15, 32 * 15},
                {32 * 21, 32 * 15},
                {32 * 37, 32 * 21},
                {32 * 49, 32 * 21},
                {32 * 41, 32 * 26},
                {32 * 47, 32 * 26},
            };

            posi_plataforma = {
                {32 * 3, 32 * 14, 12 * 32},
                {32 * 32, 32 * 14, 12 * 32},
                {32 * 34, 32 * 14, 12 * 32},
                {32 * 65, 32 * 18, 8 * 32},
                {32 * 67, 32 * 18, 8 * 32}
            };

            posi_choquinho = {
                {32 * 45, 32 * 1},
                {32 * 57, 32 * 1},
                {32 * 69, 32 * 1},
                {32 * 81, 32 * 1},
                {32 * 93, 32 * 1},
                {32 * 16, 32 * 2},
                {32 * 26, 32 * 2},
                {32 * 45, 32 * 6},
                {32 * 57, 32 * 6},
                {32 * 69, 32 * 6},
                {32 * 81, 32 * 6},
                {32 * 93, 32 * 6},
                {32 * 9, 32 * 7},
                {32 * 26, 32 * 7},
                {32 * 49, 32 * 11},
                {32 * 9, 32 * 12},
                {32 * 26, 32 * 12},
                {32 * 49, 32 * 16},
                {32 * 55, 32 * 16},
                {32 * 9, 32 * 17},
                {32 * 26, 32 * 17},
                {32 * 47, 32 * 21},
                {32 * 55, 32 * 21},
                {32 * 9, 32 * 26}
            };

            if (!carregarSave)
            {
                criarObstaculos();
                criarInimigos();
            }
            else
            {
                Fase::criarCenario("assets/tiledcybermapa2.json");
            }
        }

        FaseSegunda::~FaseSegunda()
        {

        }

        void FaseSegunda::criarInimigos()
        {
            //int totalPosJuniors = posi_robo_junior.size();
            int numJuniors = MIN_JUNIOR + (rand() % (MAX_JUNIOR - MIN_JUNIOR + 1));
            numJuniors = std::min(numJuniors, (int)(posi_robo_junior.size()));

            for (int i = 0; i < numJuniors; ++i) {
                int j = i + (rand() % (posi_robo_junior.size() - i));
                std::swap(posi_robo_junior[i], posi_robo_junior[j]);
                criarRoboJunior(posi_robo_junior[i].x, posi_robo_junior[i].y);
            }


            //int totalPosCeos = posi_ceo.size();
            int numCeos = MIN_CEO + (rand() % (MAX_CEO - MIN_CEO + 1));
            numCeos = std::min(numCeos, (int)(posi_ceo.size()));

            for (int i = 0; i < numCeos; ++i) {
                int j = i + (rand() % (posi_ceo.size() - i));
                std::swap(posi_ceo[i], posi_ceo[j]);
                criarRoboCEO(posi_ceo[i].x, posi_ceo[i].y);
            }
        }

        void FaseSegunda::criarObstaculos()
        {
            Fase::criarCenario("assets/tiledcybermapa2.json");
            int numPlataformas = MIN_PLATAFORMA + (rand() % (MAX_PLATAFORMA - MIN_PLATAFORMA + 1));
            numPlataformas = std::min(numPlataformas, (int)(posi_plataforma.size()));

            for (int i = 0; i < numPlataformas; ++i)
            {
                int j = i + (rand() % (posi_plataforma.size() - i));
                std::swap(posi_plataforma[i], posi_plataforma[j]);

                sf::Vector3f& spawn = posi_plataforma[i];
                criarPlataforma(spawn.x, spawn.y, (int)spawn.z);
            }

            int numChoquinhos = MIN_CHOQUINHO + (rand() % (MAX_CHOQUINHO - MIN_CHOQUINHO + 1));
            numChoquinhos = std::min(numChoquinhos,(int) posi_choquinho.size());

            for (int i = 0; i < numChoquinhos; ++i)
            {
                int j = i + (rand() % (posi_choquinho.size() - i));
                std::swap(posi_choquinho[i], posi_choquinho[j]);
                criarChoquinho(posi_choquinho[i].x, posi_choquinho[i].y);
            }
        }

        void FaseSegunda::desenharMapa()
        {
            Gerenciador_Grafico* pGG = Ente::getGerenciadorGrafico();
            if (!pGG || !pGG->getJanela()) 
            {
                return;
            }
            const int tilesetCols = texturaTileset.getSize().x / (int)TAMANHO_BLOCO_X;//numero de colunas de tiles na imagem

            for (int y = 0; y < (int)gridMapa.size(); ++y)
            {
                for (int x = 0; x < (int)gridMapa[y].size(); ++x)
                {
                    unsigned int id = gridMapa[y][x];
                    if (id == 0)//o criarMapa já leu o arquivo e marcou que aqui é 0 pq n tem nada
                    {
                        continue;
                    }
                        

                    int idAjustado = id - 1;//array começa com 0 e tiled começa com 1
                    int tileX = (idAjustado % tilesetCols) * (int)TAMANHO_BLOCO_X;//coluna do tile dentro do tile set * tamanho para converter isso em pixels
                    int tileY = (idAjustado / tilesetCols) * (int)TAMANHO_BLOCO_Y;//esses tileX e Y são o canto superior esquerdo do tile

                    sf::Sprite tile(texturaTileset);//cria um sprite 
                    tile.setTextureRect(sf::IntRect(tileX, tileY, (int)TAMANHO_BLOCO_X, (int)TAMANHO_BLOCO_Y));//ajusta pro tile
                    tile.setPosition(x * TAMANHO_BLOCO_X, y * TAMANHO_BLOCO_Y);//coloca no lugar certo
                    pGG->getJanela()->draw(tile);//desenhar o tile
                }
            }
        }

        void FaseSegunda::criarRoboCEO(float x, float y)
        {
            Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
            if (!pGG_local) 
            {
				return;
            }

            Entidades::Personagens::Robo_CEO* chefe = new Entidades::Personagens::Robo_CEO(x, y);
            chefe->setGerenciadorGrafico(pGG_local);
            pListaInimigos->inserir(chefe);
        }

        void FaseSegunda::criarChoquinho(float x, float y)
        {
            Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
            if (!pGG_local) 
            {
                return;
            }
            Entidades::Obstaculos::Choquinho* obst = new Entidades::Obstaculos::Choquinho(x, y);
            obst->setGerenciadorGrafico(pGG_local);
            pListaObstaculos->inserir(obst);
        }
    }
}
#include "Fase.h"
#include "Robo_Junior.h"
#include "Plataforma.h"
#include "Chao.h"
#include "Robo_CEO.h" 
#include "Lista.h"
#include "Projetil.h" 
#include "Robo_Senior.h" 
#include "Jogo.h" 
#include "Gerenciador_Grafico.h" 
#include <iostream> 
#include <SFML/Graphics.hpp>
#include <fstream>      
#include <nlohmann/json.hpp> 
#include <functional>   

using json = nlohmann::json;
using namespace CyberMetro::Entidades;
using namespace CyberMetro::Entidades::Personagens;
using namespace CyberMetro::Gerenciadores;
using namespace CyberMetro::Listas;
using namespace CyberMetro;

namespace CyberMetro {
	namespace Fases {

		const int Fase::MIN_JUNIOR = 3;
		const int Fase::MAX_JUNIOR = 26;
		const int Fase::MIN_PLATAFORMA = 3;
		const int Fase::MAX_PLATAFORMA = 5;

		Fase::Fase(Entidades::Personagens::Jogador* jogador1, Entidades::Personagens::Jogador* jogador2)
			: Ente(),
			pJogador1(jogador1),
			pJogador2(jogador2),
			pListaInimigos(nullptr),
			pListaObstaculos(nullptr),
			pListaChao(nullptr),
			pColisoes(nullptr),
			pJogo(nullptr),
			areaDeSaida(0.f, 0.f, 0.f, 0.f),
			terminou(false)
		{
			relogioFase.restart();

			if (!texturaSaida.loadFromFile("assets/saida.png"))
			{
				std::cerr << "Erro: Nao foi possivel carregar assets/saida.png" << std::endl;
			}
			spriteSaida.setTexture(texturaSaida);


			pListaInimigos = new Listas::ListaEntidades();
			pListaObstaculos = new Listas::ListaEntidades();
			pListaChao = new Listas::ListaEntidades();

			pColisoes = new Gerenciadores::Gerenciador_Colisoes(pJogador1);

			if (pJogador2 && pJogador2->getAtivo()) {
				pColisoes->setJogador2(pJogador2);
			}
		}

		Fase::~Fase()
		{
			limpar();
		}

		void Fase::setJogo(CyberMetro::Jogo* pJ)
		{
			pJogo = pJ;
		}

		void Fase::adicionarJogador2(Entidades::Personagens::Jogador* j2)
		{
			pJogador2 = j2;

			if (pColisoes) {
				pColisoes->setJogador2(pJogador2);
			}
		}

		void Fase::verificarFimDeFase()
		{
			if (terminou || areaDeSaida.width == 0 || !pJogo) {
				return;
			}

			if (pJogador1 && pJogador1->getAtivo()) {
				if (areaDeSaida.intersects(pJogador1->getFigura()->getGlobalBounds())) {
					terminou = true;
				}
			}

			if (pJogador2 && pJogador2->getAtivo()) {
				if (areaDeSaida.intersects(pJogador2->getFigura()->getGlobalBounds())) {
					terminou = true;
				}
			}
		}

		void Fase::executar()
		{
			if (pJogador1)
			{
				pJogador1->executar();
			}

			if (pJogador2)
			{
				pJogador2->executar();
			}

			if (pListaInimigos)
			{//faz roboCEO olhar pro jogador
				Lista<Entidades::Entidade>::Elemento* atualI = pListaInimigos->getPrimeiro();
				while (atualI) {
					Entidades::Entidade* pEnt = atualI->getInfo();
					Entidades::Personagens::Robo_CEO* robCeo = dynamic_cast<Entidades::Personagens::Robo_CEO*>(pEnt);
					if (robCeo && robCeo->getAtivo())
					{
						if (pJogador1 && pJogador1->getAtivo())
						{
							robCeo->setJogador(pJogador1);
						}
						else if (pJogador2 && pJogador2->getAtivo())
						{
							robCeo->setJogador(pJogador2);
						}
						else
						{
							robCeo->setJogador(nullptr);
						}
					}
					atualI = atualI->getProx();
				}

			}


			if (pListaInimigos)
			{
				pListaInimigos->executar(); // pra deixar mais bonito
			}

			if (pListaObstaculos) 
			{
				pListaObstaculos->executar();
			}

			if (pListaChao)
			{
				pListaChao->executar();
			}

			if (pColisoes) {
				pColisoes->limparInimigos();
				pColisoes->limparObstaculos();
				pColisoes->limparProjeteis();
				pColisoes->limparChao();

				if (pListaInimigos)
				{ // incluindo na lista para verificar colisões
					Lista<Entidades::Entidade>::Elemento* atualI = pListaInimigos->getPrimeiro();
					while (atualI) {
						Entidades::Entidade* pEnt = atualI->getInfo();
						Entidades::Personagens::Inimigo* inim = dynamic_cast<Entidades::Personagens::Inimigo*>(pEnt);
						if (inim && inim->getAtivo()) {
							pColisoes->incluirInimigo(inim);
						}
						atualI = atualI->getProx();
					}
				}

				if (pListaObstaculos)
				{
					Lista<Entidades::Entidade>::Elemento* atualO = pListaObstaculos->getPrimeiro();
					while (atualO) {
						Entidades::Entidade* pEnt = atualO->getInfo();
						Entidades::Obstaculos::Obstaculo* obst = dynamic_cast<Entidades::Obstaculos::Obstaculo*>(pEnt);
						if (obst && obst->getAtivo()) {
							pColisoes->incluirObstaculo(obst);
						}
						atualO = atualO->getProx();
					}
				}

				if (pListaChao)
				{
					Lista<Entidades::Entidade>::Elemento* atualC = pListaChao->getPrimeiro();
					while (atualC) {
						Entidades::Entidade* pEnt = atualC->getInfo();
						Entidades::Chao* chao = dynamic_cast<Entidades::Chao*>(pEnt);
						if (chao && chao->getAtivo()) {
							pColisoes->incluirChao(chao);
						}
						atualC = atualC->getProx();
					}
				}

				if (pJogador1)
				{
					Listas::ListaEntidades* projJog = pJogador1->getProjeteis();
					Lista<Entidades::Entidade>::Elemento* atualPJog = projJog->getPrimeiro();
					while (atualPJog) {
						Entidades::Entidade* p = atualPJog->getInfo();
						if (p->getAtivo()) {
							Entidades::Projetil* pProj = dynamic_cast<Entidades::Projetil*>(p);
							if (pProj) {
								pColisoes->incluirProjetil(pProj);
							}
						}
						atualPJog = atualPJog->getProx();
					}
				}

				if (pJogador2)
				{
					Listas::ListaEntidades* projJog2 = pJogador2->getProjeteis();
					Lista<Entidades::Entidade>::Elemento* atualPJog2 = projJog2->getPrimeiro();
					while (atualPJog2) {
						Entidades::Entidade* p = atualPJog2->getInfo();
						if (p->getAtivo()) {
							Entidades::Projetil* pProj = dynamic_cast<Entidades::Projetil*>(p);
							if (pProj) {
								pColisoes->incluirProjetil(pProj);
							}
						}
						atualPJog2 = atualPJog2->getProx();
					}
				}

				if (pListaInimigos)
				{
					Lista<Entidades::Entidade>::Elemento* atualI = pListaInimigos->getPrimeiro();
					while (atualI) {
						if (atualI->getInfo()->getAtivo()) {
							Entidades::Personagens::Robo_CEO* robCeo = dynamic_cast<Entidades::Personagens::Robo_CEO*>(atualI->getInfo());
							if (robCeo) {
								Listas::ListaEntidades* projrobCeo = robCeo->getProjeteis();
								Lista<Entidades::Entidade>::Elemento* atualProbCeo = projrobCeo->getPrimeiro();
								while (atualProbCeo) {
									Entidades::Entidade* p = atualProbCeo->getInfo();
									if (p->getAtivo()) {
										Entidades::Projetil* pProj = dynamic_cast<Entidades::Projetil*>(p);
										if (pProj) {
											pColisoes->incluirProjetil(pProj);
										}
									}
									atualProbCeo = atualProbCeo->getProx();
								}
							}
						}
						atualI = atualI->getProx();
					}
				}

				pColisoes->executar();
			}

			verificarFimDeFase();
		}

		void Fase::desenharFase()
		{
			Gerenciador_Grafico* pGG = Ente::getGerenciadorGrafico();
			if (pGG)
			{
				pGG->getJanela()->setView(pGG->getJanela()->getDefaultView());
				pGG->desenharBackground();

				if (pJogador1 && pJogador1->getAtivo())
				{
					pGG->setCamera(pJogador1);
				}
				else if (pJogador2 && pJogador2->getAtivo())
				{
					pGG->setCamera(pJogador2);
				}

				desenharMapa();

				pGG->getJanela()->draw(spriteSaida);

				if (pListaObstaculos) {
					pListaObstaculos->desenhar();
				}

				if (pJogador1) {
					if (pJogador1->getAtivo()) pJogador1->desenhar();
					pJogador1->getProjeteis()->desenhar();
				}
				if (pJogador2) {
					if (pJogador2->getAtivo()) pJogador2->desenhar();
					pJogador2->getProjeteis()->desenhar();
				}

				if (pListaInimigos)
				{
					pListaInimigos->desenhar();
					Lista<Entidades::Entidade>::Elemento* atualI = pListaInimigos->getPrimeiro();
					while (atualI) {
						Entidades::Entidade* pEnt = atualI->getInfo();
						if (pEnt && pEnt->getAtivo()) {
							Entidades::Personagens::Robo_CEO* robCeo = dynamic_cast<Entidades::Personagens::Robo_CEO*>(pEnt);
							if (robCeo) {
								robCeo->getProjeteis()->desenhar();
							}
						}
						atualI = atualI->getProx();
					}
				}
			}
		}


		void Fase::limpar()
		{
			if (pListaInimigos) {
				delete pListaInimigos;
				pListaInimigos = nullptr;
			}
			if (pListaObstaculos) {
				delete pListaObstaculos;
				pListaObstaculos = nullptr;
			}
			if (pListaChao) {
				delete pListaChao;
				pListaChao = nullptr;
			}
			if (pColisoes) {
				delete pColisoes;
				pColisoes = nullptr;
			}
		}


		void Fase::criarRoboJunior(float x, float y)
		{
			Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
			if (!pGG_local) return;

			Entidades::Personagens::Robo_Junior* inimigo = new Entidades::Personagens::Robo_Junior(x, y);
			inimigo->setGerenciadorGrafico(pGG_local);
			pListaInimigos->inserir(inimigo);
		}

		void Fase::criarRoboJunior(const sf::Vector2f& pos)//sobrecarga
		{
			this->criarRoboJunior(pos.x, pos.y);
		}

		void Fase::criarPlataforma(float x, float y, int altura)
		{
			Gerenciadores::Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
			if (!pGG_local) return;

			Entidades::Obstaculos::Plataforma* plat = new Entidades::Obstaculos::Plataforma(x, y, altura);
			plat->setGerenciadorGrafico(pGG_local);
			pListaObstaculos->inserir(plat);
		}

		void Fase::criarChao(float x, float y)
		{
			Gerenciadores::Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
			if (!pGG_local) return;

			Entidades::Chao* chao = new Entidades::Chao(x, y);
			chao->setGerenciadorGrafico(pGG_local);
			pListaChao->inserir(chao);
		}

		void Fase::desenharMapa()
		{
			//virtual
		}

		void Fase::criarCenario(const std::string& arquivoJson)
		{
			std::ifstream file(arquivoJson);
			if (!file.is_open()) {
				std::cerr << "Erro: Nao foi possivel abrir '" << arquivoJson << "'" << std::endl;
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

			json data = mapa["layers"][0]["data"];
			int index = 0;
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					gridMapa[y][x] = data[index++];
				}
			}

			std::function<bool(int, int)> vazio = [&](int ry, int rx) {
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

		bool Fase::getTerminou() const
		{
			return terminou;
		}

		sf::FloatRect Fase::getAreaSaida() const
		{
			return areaDeSaida;
		}

		float Fase::getTempoDecorrido() const
		{
			return relogioFase.getElapsedTime().asSeconds();
		}
		Listas::ListaEntidades* Fase::getListaInimigos() const
		{
			return pListaInimigos;
		}

		Listas::ListaEntidades* Fase::getListaObstaculos() const
		{
			return pListaObstaculos;
		}

		Listas::ListaEntidades* Fase::getListaChao() const
		{
			return pListaChao;
		}

		void Fase::salvarFase(json& arq)
		{
			arq["inimigos"] = json::array();
			arq["obstaculos"] = json::array();
			arq["projeteis"] = json::array();

			auto* elem = pListaInimigos->getPrimeiro();
			while (elem)
			{
				auto* ent = elem->getInfo();

				Inimigo* inim = dynamic_cast<Inimigo*>(ent);

				if (inim && inim->getAtivo())
				{
					json j_inim;
					inim->salvarDataBuffer(j_inim);
					arq["inimigos"].push_back(j_inim);

					if (auto* ceo = dynamic_cast<Robo_CEO*>(inim))
					{
						auto* projCeo = ceo->getProjeteis()->getPrimeiro();
						while (projCeo)
						{
							auto* projEnt = projCeo->getInfo();
							if (projEnt && projEnt->getAtivo())
							{
								json j_proj;
								projEnt->salvarDataBuffer(j_proj);
								arq["projeteis"].push_back(j_proj);
							}
							projCeo = projCeo->getProx();
						}
					}
				}
				elem = elem->getProx();
			}

			
			if (pListaObstaculos) {
				auto* elemObst = pListaObstaculos->getPrimeiro();
				while (elemObst) {
					CyberMetro::Entidades::Entidade* ent = elemObst->getInfo();

					auto* obst = dynamic_cast<CyberMetro::Entidades::Obstaculos::Obstaculo*>(ent);
					if (obst && obst->getAtivo()) {
						json j_obst;
						obst->salvarDataBuffer(j_obst);
						arq["obstaculos"].push_back(j_obst);
					}

					elemObst = elemObst->getProx();
				}
			}

			
			if (pJogador1) {
				auto* elemProj1 = pJogador1->getProjeteis()->getPrimeiro();
				while (elemProj1) {
					CyberMetro::Entidades::Entidade* ent = elemProj1->getInfo();
					auto* proj = dynamic_cast<CyberMetro::Entidades::Projetil*>(ent);

					if (proj && proj->getAtivo()) {
						json j_proj;
						proj->salvarDataBuffer(j_proj);
						arq["projeteis"].push_back(j_proj);
					}

					elemProj1 = elemProj1->getProx();
				}
			}

			if (pJogador2 && pJogador2->getAtivo()) {
				auto* elemProj2 = pJogador2->getProjeteis()->getPrimeiro();
				while (elemProj2) {
					CyberMetro::Entidades::Entidade* ent = elemProj2->getInfo();
					auto* proj = dynamic_cast<CyberMetro::Entidades::Projetil*>(ent);

					if (proj && proj->getAtivo()) {
						json j_proj;
						proj->salvarDataBuffer(j_proj);
						arq["projeteis"].push_back(j_proj);
					}

					elemProj2 = elemProj2->getProx();
				}
			}
		}
	}
}
 
#include "Fase.h"
#include "Robo_Junior.h"
#include "Plataforma.h"
#include "Chao.h"
#include "Robo_CEO.h" 
#include "Lista.h"
#include "Projetil.h" 
#include "Robo_Senior.h" 

namespace Fases {
	Fase::Fase(Jogador* jogador1, Jogador* jogador2)
		: Ente(),
		pJogador1(jogador1),
		pJogador2(jogador2),
		pListaInimigos(nullptr),
		pListaObstaculos(nullptr),
		pColisoes(nullptr)
	{
		if (pJogador1) {
			pJogador1->setX(50.0f);
			pJogador1->setY(450.0f);
			pJogador1->setPosicaoGrafica(pJogador1->getX(), pJogador1->getY());
			pJogador1->setAtivo(true);
		}

		if (pJogador2 && pJogador2->getAtivo()) {
			pJogador2->setX(100.0f);
			pJogador2->setY(450.0f);
			pJogador2->setPosicaoGrafica(pJogador2->getX(), pJogador2->getY());
		}

		pListaInimigos = new ListaEntidades();
		pListaObstaculos = new ListaEntidades();

		pColisoes = new Gerenciador_Colisoes(pJogador1);

		if (pJogador2 && pJogador2->getAtivo()) {
			pColisoes->setJogador2(pJogador2);
		}
	}

	Fase::~Fase()
	{
		limpar();
	}

	void Fase::adicionarJogador2(Jogador* j2)
	{
		pJogador2 = j2;
		if (pColisoes) {
			pColisoes->setJogador2(pJogador2);
		}
	}

	void Fase::executar()
	{
		if (pJogador1)
			pJogador1->executar();

		if (pJogador2)
			pJogador2->executar();

		if (pListaInimigos)
			pListaInimigos->executar();

		if (pListaObstaculos)
			pListaObstaculos->executar();

		if (pColisoes) {

			pColisoes->limparInimigos();
			pColisoes->limparObstaculos();
			pColisoes->limparProjeteis();

			if (pListaInimigos)
			{
				Elemento<Entidades::Entidade>* curI = pListaInimigos->getPrimeiro();
				while (curI) {
					Entidade* pEnt = curI->getInfo();
					Personagens::Inimigo* inim = dynamic_cast<Personagens::Inimigo*>(pEnt);
					if (inim && inim->getAtivo()) {
						pColisoes->incluirInimigo(inim);
					}
					curI = curI->getProx();
				}
			}

			if (pListaObstaculos)
			{
				Elemento<Entidades::Entidade>* curO = pListaObstaculos->getPrimeiro();
				while (curO) {
					Entidade* pEnt = curO->getInfo();
					Obstaculos::Obstaculo* obst = dynamic_cast<Obstaculos::Obstaculo*>(pEnt);
					if (obst && obst->getAtivo()) {
						pColisoes->incluirObstaculo(obst);
					}
					curO = curO->getProx();
				}
			}

			if (pJogador1)
			{
				ListaEntidades* projJog = pJogador1->getProjeteis();
				Elemento<Entidades::Entidade>* curPJog = projJog->getPrimeiro();
				while (curPJog) {
					Entidade* p = curPJog->getInfo();
					if (p->getAtivo()) {
						Entidades::Projetil* pProj = dynamic_cast<Entidades::Projetil*>(p);
						if (pProj) {
							pColisoes->incluirProjetil(pProj);
						}
					}
					curPJog = curPJog->getProx();
				}
			}

			if (pJogador2)
			{
				ListaEntidades* projJog2 = pJogador2->getProjeteis();
				Elemento<Entidades::Entidade>* curPJog2 = projJog2->getPrimeiro();
				while (curPJog2) {
					Entidade* p = curPJog2->getInfo();
					if (p->getAtivo()) {
						Entidades::Projetil* pProj = dynamic_cast<Entidades::Projetil*>(p);
						if (pProj) {
							pColisoes->incluirProjetil(pProj);
						}
					}
					curPJog2 = curPJog2->getProx();
				}
			}

			if (pListaInimigos)
			{
				Elemento<Entidades::Entidade>* curI = pListaInimigos->getPrimeiro();
				while (curI) {
					if (curI->getInfo()->getAtivo()) {
						Personagens::Robo_CEO* robCeo = dynamic_cast<Personagens::Robo_CEO*>(curI->getInfo());
						if (robCeo) {
							ListaEntidades* projrobCeo = robCeo->getProjeteis();
							Elemento<Entidades::Entidade>* curProbCeo = projrobCeo->getPrimeiro();
							while (curProbCeo) {
								Entidade* p = curProbCeo->getInfo();
								if (p->getAtivo()) {
									Entidades::Projetil* pProj = dynamic_cast<Entidades::Projetil*>(p);
									if (pProj) {
										pColisoes->incluirProjetil(pProj);
									}
								}
								curProbCeo = curProbCeo->getProx();
							}
						}
					}
					curI = curI->getProx();
				}
			}

			pColisoes->executar();
		}

		Gerenciador_Grafico* pGG = Ente::getGerenciadorGrafico();
		if (pGG)
		{
			pGG->clear();
			pGG->getJanela()->setView(pGG->getJanela()->getDefaultView());
			pGG->desenharBackground();
			if (pJogador1) {
				pGG->setCamera(pJogador1);
			}
			desenharMapa();
			if (pListaObstaculos) pListaObstaculos->desenhar();
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
				Elemento<Entidades::Entidade>* curI = pListaInimigos->getPrimeiro();
				while (curI) {
					Entidade* pEnt = curI->getInfo();
					if (pEnt && pEnt->getAtivo()) {
						Personagens::Robo_CEO* robCeo = dynamic_cast<Personagens::Robo_CEO*>(pEnt);
						if (robCeo) {
							robCeo->getProjeteis()->desenhar();
						}
					}
					curI = curI->getProx();
				}
			}

			pGG->render();
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
		if (pColisoes) {
			delete pColisoes;
			pColisoes = nullptr;
		}
	}


	void Fase::criarRoboJunior(float x, float y)
	{
		Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
		if (!pGG_local) return;

		Personagens::Robo_Junior* inimigo = new Personagens::Robo_Junior(x, y);
		inimigo->setGerenciadorGrafico(pGG_local);
		pListaInimigos->inserir(inimigo);
	}

	void Fase::criarRoboJunior(const sf::Vector2f& pos)
	{
		this->criarRoboJunior(pos.x, pos.y);
	}

	void Fase::criarPlataforma(float x, float y)
	{
		Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
		if (!pGG_local) return;

		Obstaculos::Plataforma* plat = new Obstaculos::Plataforma(x, y);
		plat->setGerenciadorGrafico(pGG_local);
		pListaObstaculos->inserir(plat);
	}

	void Fase::criarPlataforma(const sf::Vector2f& pos)
	{
		this->criarPlataforma(pos.x, pos.y);
	}

	void Fase::criarChao(float x, float y)
	{
		Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
		if (!pGG_local) return;

		Obstaculos::Chao* chao = new Obstaculos::Chao(x, y);
		chao->setGerenciadorGrafico(pGG_local);
		pListaObstaculos->inserir(chao);
	}

	void Fase::desenharMapa() 
	{
	
	}
	void Fase::criarMapa() 
	{
	
	}
}
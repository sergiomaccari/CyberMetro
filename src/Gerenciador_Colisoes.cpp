#include "Gerenciador_Colisoes.h"
#include <iostream>
#include "Inimigo.h"
#include "Gerenciador_Colisoes.h"
#include <iostream>
#include "Inimigo.h"
#include "Obstaculo.h"
#include "Projetil.h" 
#include "Chao.h"

#include "Entidade.h" 
#include "Jogador.h"

using namespace CyberMetro::Entidades;
using namespace CyberMetro::Entidades::Personagens;
using namespace CyberMetro::Entidades::Obstaculos;

const float CyberMetro::Gerenciadores::Gerenciador_Colisoes::RAIO_CHEKAGEM_QUADRADO = 4096.0f;

namespace CyberMetro {
	namespace Gerenciadores
	{

		Gerenciador_Colisoes::Gerenciador_Colisoes(Personagens::Jogador* j1) : p1(j1), p2(NULL)
		{
		}

		Gerenciador_Colisoes::~Gerenciador_Colisoes()
		{
			p1 = NULL;
			p2 = NULL;
			LIs.clear();
			LOs.clear();
			LCs.clear();
			LPs.clear();
		}

		void Gerenciador_Colisoes::setJogador2(Personagens::Jogador* j2)
		{
			this->p2 = j2;
		}


		void Gerenciador_Colisoes::incluirInimigo(Personagens::Inimigo* pi)
		{
			if (pi)
				LIs.push_back(pi);
		}

		void Gerenciador_Colisoes::incluirObstaculo(Obstaculos::Obstaculo* po)
		{
			if (po)
				LOs.push_back(po);
		}

		void Gerenciador_Colisoes::incluirChao(Entidades::Chao* pc)
		{
			if (pc)
				LCs.push_back(pc);
		}

		void Gerenciador_Colisoes::incluirProjetil(Entidades::Projetil* pj)
		{
			if (pj)
				LPs.insert(pj);
		}
		void Gerenciador_Colisoes::limparProjeteis() 
		{ 
			LPs.clear(); 
		}
		void Gerenciador_Colisoes::limparInimigos() 
		{
			LIs.clear(); 
		}
		void Gerenciador_Colisoes::limparObstaculos() 
		{ 
			LOs.clear(); 
		}
		void Gerenciador_Colisoes::limparChao() 
		{
			LCs.clear();
		}

		const bool Gerenciador_Colisoes::verificarColisao(Entidade* pe1, Entidade* pe2) const
		{
			if (pe1 && pe1->getFigura() && pe2 && pe2->getFigura() && pe1->getAtivo() && pe2->getAtivo())
			{
				sf::FloatRect bounds1 = pe1->getFigura()->getGlobalBounds();
				sf::FloatRect bounds2 = pe2->getFigura()->getGlobalBounds();
				return bounds1.intersects(bounds2);
			}

			return false;
		}

		void Gerenciador_Colisoes::tratarColisoesJogsObstacs()
		{
			if (p1 && p1->getFigura() && p1->getAtivo())
			{
				sf::FloatRect boundsJ = p1->getFigura()->getGlobalBounds();
				for (std::list<Obstaculos::Obstaculo*>::iterator it = LOs.begin(); it != LOs.end(); ++it)
				{
					Obstaculos::Obstaculo* obst = *it;
					//normalizacao da dsitancia/raio de checkagem
					float dx = p1->getX() - obst->getX();
					float dy = p1->getY() - obst->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
					{
						continue;
					}

					if (verificarColisao(p1, obst))
					{
						sf::FloatRect boundsO = obst->getFigura()->getGlobalBounds();
						sf::FloatRect intersection;
						boundsJ.intersects(boundsO, intersection);
						if (intersection.width > intersection.height) {
							if (boundsJ.top < boundsO.top)
							{
								p1->setVel_Grav(0);
								p1->setY(boundsO.top - boundsJ.height);
							}
							else
							{

								p1->setY(boundsO.top + boundsO.height);
							}
						}
						else {
							if (boundsJ.left < boundsO.left)
							{
								p1->setMovimentoY(0.0f);
								p1->setX(boundsO.left - boundsJ.width);
							}
							else
							{
								p1->setMovimentoY(0.0f);
								p1->setX(boundsO.left + boundsO.width);
							}
							p1->setVel_Grav(0);
						}
						p1->setPosicaoGrafica(p1->getX(), p1->getY());
						obst->obstaculizar(p1);
					}
				}
			}

			if (p2 && p2->getFigura() && p2->getAtivo())
			{
				sf::FloatRect boundsJ = p2->getFigura()->getGlobalBounds();
				for (std::list<Obstaculos::Obstaculo*>::iterator it = LOs.begin(); it != LOs.end(); ++it)
				{
					Obstaculos::Obstaculo* obst = *it;

					float dx = p2->getX() - obst->getX();
					float dy = p2->getY() - obst->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
					{
						continue;
					}

					if (verificarColisao(p2, obst))
					{
						sf::FloatRect boundsO = obst->getFigura()->getGlobalBounds();
						sf::FloatRect intersection;
						boundsJ.intersects(boundsO, intersection);

						if (intersection.width > intersection.height) {
							if (boundsJ.top < boundsO.top)
							{
								p2->setVel_Grav(0);
								p2->setY(boundsO.top - boundsJ.height);
							}
							else 
							{
								p2->setMovimentoY(2.0f);
								p2->setVel_Grav(0);
								p2->setY(boundsO.top + boundsO.height);
							}
						}
						else {

							if (boundsJ.left < boundsO.left)
								p2->setX(boundsO.left - boundsJ.width);
							else
								p2->setX(boundsO.left + boundsO.width);

						}
						p2->setPosicaoGrafica(p2->getX(), p2->getY());
						obst->obstaculizar(p2);
					}
				}
			}
		}

		void Gerenciador_Colisoes::tratarColisoesInimgsObstacs()
		{
			for (std::vector<Personagens::Inimigo*>::iterator it_inim = LIs.begin(); it_inim != LIs.end(); ++it_inim)
			{
				Personagens::Inimigo* inim = *it_inim;
				if (inim && inim->getFigura() && inim->getAtivo())
				{
					sf::FloatRect boundsI = inim->getFigura()->getGlobalBounds();

					for (std::list<Obstaculos::Obstaculo*>::iterator it_obst = LOs.begin(); it_obst != LOs.end(); ++it_obst)
					{
						Obstaculos::Obstaculo* obst = *it_obst;

						float dx = inim->getX() - obst->getX();
						float dy = inim->getY() - obst->getY();
						float distQuadrada = (dx * dx) + (dy * dy);
						if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
						{
							continue;
						}

						if (verificarColisao(inim, obst))
						{
							sf::FloatRect boundsO = obst->getFigura()->getGlobalBounds();
							sf::FloatRect intersection;
							boundsI.intersects(boundsO, intersection);
							if (intersection.width > intersection.height)
							{
								if (boundsI.top < boundsO.top) 
								{
									inim->setVel_Grav(0);
									inim->setY(boundsO.top - boundsI.height);
								}
								else
								{
									inim->setVel_Grav(0);
									inim->setY(boundsO.top + boundsO.height);
								}
							}
							else
							{
								if (boundsI.left < boundsO.left)
									inim->setX(boundsO.left - boundsI.width);
								else
									inim->setX(boundsO.left + boundsO.width);
							}
							inim->setPosicaoGrafica(inim->getX(), inim->getY());
						}
					}
				}
			}
		}


		void Gerenciador_Colisoes::tratarColisoesProjeteisObstacs()
		{
			std::set<Entidades::Projetil*>::iterator it_proj = LPs.begin();
			while (it_proj != LPs.end())
			{
				Entidades::Projetil* proj = *it_proj;
				if (!proj->getAtivo())
				{
					++it_proj; 
					continue;
				}

				for (std::list<Obstaculos::Obstaculo*>::iterator it_obst = LOs.begin(); it_obst != LOs.end(); ++it_obst)
				{
					Obstaculos::Obstaculo* obst = *it_obst;

					float dx = proj->getX() - obst->getX();
					float dy = proj->getY() - obst->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
					{
						continue;
					}

					if (verificarColisao(proj, obst))
					{
						proj->setAtivo(false);
						break;
					}
				}
				++it_proj;
			}
		}

		void Gerenciador_Colisoes::tratarColisoesObstacsObstacs()
		{
			for (std::list<Obstaculos::Obstaculo*>::iterator it_A = LOs.begin(); it_A != LOs.end(); ++it_A)
			{
				Obstaculos::Obstaculo* obstA = *it_A;
				if (!obstA || !obstA->getAtivo())
					continue;

				for (std::list<Obstaculos::Obstaculo*>::iterator it_B = LOs.begin(); it_B != LOs.end(); ++it_B)
				{
					if (it_A == it_B) 
					{ 
						continue; 
					}
						

					Obstaculos::Obstaculo* obstB = *it_B;
					if (!obstB || !obstB->getAtivo())
					{
						continue;
					}

					float dx = obstA->getX() - obstB->getX();
					float dy = obstA->getY() - obstB->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
					{
						continue;
					}

					if (verificarColisao(obstA, obstB))
					{
						sf::FloatRect boundsA = obstA->getFigura()->getGlobalBounds();
						sf::FloatRect boundsB = obstB->getFigura()->getGlobalBounds();
						sf::FloatRect intersection;
						boundsA.intersects(boundsB, intersection);

						if (intersection.width > intersection.height)
						{
							if (boundsA.top < boundsB.top)
							{
								obstA->setVel_Grav(0);
								obstA->setY(boundsB.top - boundsA.height);
							}
							else
							{
								obstA->setVel_Grav(0);
								obstA->setY(boundsB.top + boundsB.height);
							}
						}
						else
						{
							if (boundsA.left < boundsB.left)
								obstA->setX(boundsB.left - boundsA.width);
							else
								obstA->setX(boundsB.left + boundsB.width);
						}
						obstA->setPosicaoGrafica(obstA->getX(), obstA->getY());
					}
				}
			}
		}


		void Gerenciador_Colisoes::tratarColisoesJogsInimgs()
		{
			if (p1 && p1->getAtivo())
			{
				for (std::vector<Personagens::Inimigo*>::iterator it_inim = LIs.begin(); it_inim != LIs.end(); ++it_inim)
				{
					Personagens::Inimigo* inim = *it_inim;
					if (!inim || !inim->getAtivo())
					{
						continue;
					}

					float dx = p1->getX() - inim->getX();
					float dy = p1->getY() - inim->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
					{
						continue;
					}

					if (verificarColisao(p1, inim))
					{
						inim->danificar(p1);
						sf::FloatRect boundsJ = p1->getFigura()->getGlobalBounds();
						sf::FloatRect boundsI = inim->getFigura()->getGlobalBounds();
						sf::FloatRect intersection;
						boundsJ.intersects(boundsI, intersection);
						if (intersection.width > intersection.height) {
							if (boundsJ.top < boundsI.top) p1->setY(boundsI.top - boundsJ.height);
							else p1->setY(boundsI.top + boundsI.height);
						}
						else {
							if (boundsJ.left < boundsI.left) p1->setX(boundsI.left - boundsJ.width - 1.0f);
							else p1->setX(boundsI.left + boundsI.width + 1.0f);
						}
						p1->setPosicaoGrafica(p1->getX(), p1->getY());
					}
				}
			}

			if (p2 && p2->getAtivo())
			{
				for (std::vector<Personagens::Inimigo*>::iterator it_inim = LIs.begin(); it_inim != LIs.end(); ++it_inim)
				{
					Personagens::Inimigo* inim = *it_inim;
					if (!inim || !inim->getAtivo()) 
					{
						continue;
					}

					float dx = p2->getX() - inim->getX();
					float dy = p2->getY() - inim->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
					{
						continue;
					}

					if (verificarColisao(p2, inim))
					{
						inim->danificar(p2);
						sf::FloatRect boundsJ = p2->getFigura()->getGlobalBounds();
						sf::FloatRect boundsI = inim->getFigura()->getGlobalBounds();
						sf::FloatRect intersection;
						boundsJ.intersects(boundsI, intersection);
						if (intersection.width > intersection.height) {
							if (boundsJ.top < boundsI.top) p2->setY(boundsI.top - boundsJ.height);
							else p2->setY(boundsI.top + boundsI.height);
						}
						else {
							if (boundsJ.left < boundsI.left) p2->setX(boundsI.left - boundsJ.width - 1.0f);
							else p2->setX(boundsI.left + boundsI.width + 1.0f);
						}
						p2->setPosicaoGrafica(p2->getX(), p2->getY());
					}
				}
			}
		}

		void Gerenciador_Colisoes::tratarColisoesJogsProjeteis()
		{
			if (p1 && p1->getAtivo())
			{
				std::set<Entidades::Projetil*>::iterator it_proj = LPs.begin();
				while (it_proj != LPs.end())
				{
					Entidades::Projetil* proj = *it_proj;

					float dx = p1->getX() - proj->getX();
					float dy = p1->getY() - proj->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
					{
						++it_proj;
						continue;
					}

					if (proj->getAtivo() && proj->getIdDono() == 0 && verificarColisao(p1, proj))
					{
						for (int i = 0; i < proj->getDano(); i++) {
							p1->operator--();
						}
						proj->setAtivo(false);
					}
					++it_proj;
				}
			}

			if (p2 && p2->getAtivo())
			{
				std::set<Entidades::Projetil*>::iterator it_proj = LPs.begin();
				while (it_proj != LPs.end())
				{
					Entidades::Projetil* proj = *it_proj;

					float dx = p2->getX() - proj->getX();
					float dy = p2->getY() - proj->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
					{
						++it_proj;
						continue;
					}

					if (proj->getAtivo() && proj->getIdDono() == 0 && verificarColisao(p2, proj))
					{
						for (int i = 0; i < proj->getDano(); i++) {
							p2->operator--();
						}
						proj->setAtivo(false);
					}
					++it_proj;
				}
			}
		}

		void Gerenciador_Colisoes::tratarColisoesInimgsProjeteis()
		{
			std::set<Entidades::Projetil*>::iterator it_proj = LPs.begin();
			while (it_proj != LPs.end())
			{
				Entidades::Projetil* proj = *it_proj;

				if (proj->getAtivo() && proj->getIdDono() > 0)
				{
					std::vector<Personagens::Inimigo*>::iterator it_inim = LIs.begin();
					while (it_inim != LIs.end())
					{
						Personagens::Inimigo* inim = *it_inim;
						if (!inim || !inim->getAtivo())
						{
							++it_inim;
							continue;
						}

						float dx = proj->getX() - inim->getX();
						float dy = proj->getY() - inim->getY();
						float distQuadrada = (dx * dx) + (dy * dy);
						if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
						{
							++it_inim;
							continue;
						}

						if (verificarColisao(proj, inim))
						{

							for (int i = 0; i < proj->getDano(); i++)
							{
								inim->operator--();
							}

							if (!inim->getAtivo())
							{
								int pontos = inim->getPontosPorMorte();

								if (proj->getIdDono() == 1 && p1) {
									p1->adicionarPontos(pontos);
								}
								else if (proj->getIdDono() == 2 && p2) {
									p2->adicionarPontos(pontos);
								}
							}

							proj->setAtivo(false);

							break;
						}
						else
						{
							++it_inim;
						}
					}
				}
				++it_proj;
			}
		}

		void Gerenciador_Colisoes::tratarColisoesInimgsInimgs()
		{
			for (std::vector<Personagens::Inimigo*>::iterator it_A = LIs.begin(); it_A != LIs.end(); ++it_A)
			{
				Personagens::Inimigo* inimA = *it_A;
				if (!inimA->getAtivo()) 
				{
					continue;
				}

				for (std::vector<Personagens::Inimigo*>::iterator it_B = std::next(it_A); it_B != LIs.end(); ++it_B)
				{
					Personagens::Inimigo* inimB = *it_B;
					if (!inimB->getAtivo()) continue;
					float dx = inimA->getX() - inimB->getX();
					float dy = inimA->getY() - inimB->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
					{
						continue;
					}

					if (verificarColisao(inimA, inimB))
					{
						sf::FloatRect boundsA = inimA->getFigura()->getGlobalBounds();
						sf::FloatRect boundsB = inimB->getFigura()->getGlobalBounds();
						sf::FloatRect intersection;
						boundsA.intersects(boundsB, intersection);

						float pushX = 0, pushY = 0;

						if (intersection.width > intersection.height)
						{
							pushY = intersection.height / 2.0f;
							if (boundsA.top < boundsB.top) {
								inimA->setY(inimA->getY() - pushY);
								inimB->setY(inimB->getY() + pushY);
							}
							else {
								inimA->setY(inimA->getY() + pushY);
								inimB->setY(inimB->getY() - pushY);
							}
						}
						else
						{
							pushX = intersection.width / 2.0f;
							if (boundsA.left < boundsB.left) 
							{
								inimA->setX(inimA->getX() - pushX);
								inimB->setX(inimB->getX() + pushX);
							}
							else 
							{
								inimA->setX(inimA->getX() + pushX);
								inimB->setX(inimB->getX() - pushX);
							}
						}

						inimA->setPosicaoGrafica(inimA->getX(), inimA->getY());
						inimB->setPosicaoGrafica(inimB->getX(), inimB->getY());
					}
				}
			}
		}

		void Gerenciador_Colisoes::tratarColisoesJogsChao()
		{
			if (p1 && p1->getFigura() && p1->getAtivo())
			{
				sf::FloatRect boundsJ = p1->getFigura()->getGlobalBounds();
				for (std::list<Entidades::Chao*>::iterator it = LCs.begin(); it != LCs.end(); ++it)
				{
					Entidades::Chao* chao = *it;

					float dx = p1->getX() - chao->getX();
					float dy = p1->getY() - chao->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
					{
						continue;
					}

					if (verificarColisao(p1, chao))
					{
						sf::FloatRect boundsC = chao->getFigura()->getGlobalBounds();
						sf::FloatRect intersection;
						boundsJ.intersects(boundsC, intersection);
						if (intersection.width > intersection.height) {
							if (boundsJ.top < boundsC.top) 
							{
								p1->setVel_Grav(0);
								p1->setY(boundsC.top - boundsJ.height);
							}
							else
							{
								p1->setVel_Grav(p1->getVel_Grav()*1.5);
								p1->setY(boundsC.top + boundsC.height);
							}
						}
						else {
							if (boundsJ.left < boundsC.left)
							{

								p1->setX(boundsC.left - boundsJ.width);
							}
							else
							{

								p1->setX(boundsC.left + boundsC.width);
							}
							p1->setVel_Grav(0);
						}
						p1->setPosicaoGrafica(p1->getX(), p1->getY());

					}
				}
			}

			if (p2 && p2->getFigura() && p2->getAtivo())
			{
				sf::FloatRect boundsJ = p2->getFigura()->getGlobalBounds();
				for (std::list<Entidades::Chao*>::iterator it = LCs.begin(); it != LCs.end(); ++it)
				{
					Entidades::Chao* chao = *it;
					float dx = p2->getX() - chao->getX();
					float dy = p2->getY() - chao->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
					{
						continue;
					}

					if (verificarColisao(p2, chao))
					{
						sf::FloatRect boundsC = chao->getFigura()->getGlobalBounds();
						sf::FloatRect intersection;
						boundsJ.intersects(boundsC, intersection);

						if (intersection.width > intersection.height) {
							if (boundsJ.top < boundsC.top)
							{
								p2->setVel_Grav(0);
								p2->setY(boundsC.top - boundsJ.height);
							}
							else
							{
                                p2->setVel_Grav(p2->getVel_Grav()*1.5);
								p2->setY(boundsC.top + boundsC.height);
							}
						}
						else {

							if (boundsJ.left < boundsC.left)
							{
								p2->setX(boundsC.left - boundsJ.width); 
							}
								
							else
							{
								p2->setX(boundsC.left + boundsC.width);
							}
							p2->setVel_Grav(0);
						}
						p2->setPosicaoGrafica(p2->getX(), p2->getY());
					}
				}
			}
		}

		void Gerenciador_Colisoes::tratarColisoesInimgsChao()
		{
			for (std::vector<Personagens::Inimigo*>::iterator it_inim = LIs.begin(); it_inim != LIs.end(); ++it_inim)
			{
				Personagens::Inimigo* inim = *it_inim;
				if (inim && inim->getFigura() && inim->getAtivo())
				{


					for (std::list<Entidades::Chao*>::iterator it_chao = LCs.begin(); it_chao != LCs.end(); ++it_chao)
					{
						Entidades::Chao* chao = *it_chao;

						float dx = inim->getX() - chao->getX();
						float dy = inim->getY() - chao->getY();
						float distQuadrada = (dx * dx) + (dy * dy);
						if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
						{
							continue;
						}

						if (verificarColisao(inim, chao))
						{
							sf::FloatRect boundsI = inim->getFigura()->getGlobalBounds();
							sf::FloatRect boundsC = chao->getFigura()->getGlobalBounds();
							sf::FloatRect intersection;
							boundsI.intersects(boundsC, intersection);
							inim->setVel_Grav(0);
							if (intersection.width > intersection.height)
							{
								if (boundsI.top < boundsC.top)
								{
									inim->setVel_Grav(0);
									inim->setY(boundsC.top - boundsI.height);
								}
								else
								{
									inim->setVel_Grav(0);
									inim->setY(boundsC.top + boundsC.height);
								}
							}
							else
							{
								if (boundsI.left < boundsC.left)
									inim->setX(boundsC.left - boundsI.width);
								else
									inim->setX(boundsC.left + boundsC.width);
							}
							inim->setPosicaoGrafica(inim->getX(), inim->getY());
							
						}
					}
				}
			}
		}

		void Gerenciador_Colisoes::tratarColisoesProjeteisChao()
		{
			std::set<Entidades::Projetil*>::iterator it_proj = LPs.begin();
			while (it_proj != LPs.end())
			{
				Entidades::Projetil* proj = *it_proj;
				if (!proj->getAtivo())
				{
					++it_proj; 
					continue;
				}

				for (std::list<Entidades::Chao*>::iterator it_chao = LCs.begin(); it_chao != LCs.end(); ++it_chao)
				{
					Entidades::Chao* chao = *it_chao;
					float dx = proj->getX() - chao->getX();
					float dy = proj->getY() - chao->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
					{
						continue;
					}

					if (verificarColisao(proj, chao))
					{
						proj->setAtivo(false);
						break;
					}
				}
				++it_proj;
			}
		}

		void Gerenciador_Colisoes::tratarColisoesObstacsChao()
		{
			for (std::list<Obstaculos::Obstaculo*>::iterator it_obst = LOs.begin(); it_obst != LOs.end(); ++it_obst)
			{
				Obstaculos::Obstaculo* obst = *it_obst;
				if (obst && obst->getFigura() && obst->getAtivo())
				{
					sf::FloatRect boundsO = obst->getFigura()->getGlobalBounds();

					for (std::list<Entidades::Chao*>::iterator it_chao = LCs.begin(); it_chao != LCs.end(); ++it_chao)
					{
						Entidades::Chao* chao = *it_chao;
						float dx = obst->getX() - chao->getX();
						float dy = obst->getY() - chao->getY();
						float distQuadrada = (dx * dx) + (dy * dy);
						if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
						{
							continue;
						}

						if (verificarColisao(obst, chao))
						{
							sf::FloatRect boundsC = chao->getFigura()->getGlobalBounds();
							sf::FloatRect intersection;
							boundsO.intersects(boundsC, intersection);
							if (intersection.width > intersection.height)
							{
								if (boundsO.top < boundsC.top)
								{
									//obst->setVel_Grav(0);
									obst->setY(boundsC.top - boundsO.height);
								}
								else
								{
									//obst->setVel_Grav(0);
									obst->setY(boundsC.top + boundsC.height);
								}
							}
							//obst->setY(0);
							obst->setVel_Grav(0);
							obst->setPosicaoGrafica(obst->getX(), obst->getY());
							//boundsO = obst->getFigura()->getGlobalBounds();
							//break;

						}
					}
				}
			}
		}


		void Gerenciador_Colisoes::executar()
		{
			//essa ordem da chamada importa
			tratarColisoesJogsInimgs();
			tratarColisoesInimgsInimgs();
			tratarColisoesJogsProjeteis();
			tratarColisoesInimgsProjeteis();
			tratarColisoesProjeteisObstacs();
			tratarColisoesProjeteisChao();
			tratarColisoesJogsChao();
			tratarColisoesInimgsChao();
			tratarColisoesObstacsChao();
			tratarColisoesJogsObstacs();
			tratarColisoesInimgsObstacs();
			tratarColisoesObstacsObstacs();
		}
	}
}
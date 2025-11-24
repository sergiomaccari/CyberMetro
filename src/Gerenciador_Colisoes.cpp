#include "Gerenciador_Colisoes.h"
#include "Inimigo.h"
#include "Obstaculo.h"
#include "Chao.h"
#include "Jogador.h"

using namespace CyberMetro::Entidades;
using namespace CyberMetro::Entidades::Personagens;
using namespace CyberMetro::Entidades::Obstaculos;

const float CyberMetro::Gerenciadores::Gerenciador_Colisoes::RAIO_CHEKAGEM_QUADRADO = 4096.0f;//otimização que faz com que so cheque se tem colisao se as entidades estao no maximo a esse valor de distancia

namespace CyberMetro {
	namespace Gerenciadores
	{

		Gerenciador_Colisoes::Gerenciador_Colisoes(Personagens::Jogador* j1) : p1(j1), p2(NULL)
		{
		}

		Gerenciador_Colisoes::~Gerenciador_Colisoes()
		{
			//boas praticas de limpar e aterrar
			p1 = NULL;
			p2 = NULL;
			LIs.clear();
			LOs.clear();
			LCs.clear();
			LPs.clear();
		}

		void Gerenciador_Colisoes::setJogador2(Personagens::Jogador* j2)//define o jogador 2, jogador 1 definido pela construtora
		{
			this->p2 = j2;
		}

		//A seguir métodos que incluem entidades nas listas apropriadas
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

		//a seguir, métodos que limpam as listas de entidades
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

		//metodo mt importante que verifica se existe uma colisao ou nao
		const bool Gerenciador_Colisoes::verificarColisao(Entidade* pe1, Entidade* pe2) const
		{
			if (pe1 && pe1->getFigura() && pe2 && pe2->getFigura() && pe1->getAtivo() && pe2->getAtivo())
			{
				sf::FloatRect bounds1 = pe1->getFigura()->getGlobalBounds();//pega contorno
				sf::FloatRect bounds2 = pe2->getFigura()->getGlobalBounds();//pega outro contornoi
				return bounds1.intersects(bounds2);//retorna a area de colisão
			}

			return false;
		}

		void Gerenciador_Colisoes::tratarColisoesJogsObstacs()//percorre a lista de obstaculos com jogador
		{
			if (p1 && p1->getFigura() && p1->getAtivo())
			{
				sf::FloatRect boundsJ = p1->getFigura()->getGlobalBounds();
				for (std::list<Obstaculos::Obstaculo*>::iterator it = LOs.begin(); it != LOs.end(); ++it)//percorre a lista
				{
					Obstaculos::Obstaculo* obst = *it;
					//normalizacao da dsitancia para raio de checkagem
					float dx = p1->getX() - obst->getX();//distância entre o centro x e y dos elementos
					float dy = p1->getY() - obst->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)//se os dois elementos estão a uma distância maior que a definida lá encima (64 pixels) ele não verifica colisão, já que eles estão geograficamente longe
					{
						continue;//quebra o looping, dava pra fazer a suficiencia da relação (ex: a tal b, fazer b tal a) o que talvez seria mais natural, porem a gente quis explicitar a quebra do loop
					}

					if (verificarColisao(p1, obst))//se tem colisão
					{
						sf::FloatRect boundsO = obst->getFigura()->getGlobalBounds();//variavel auxiliar para deixar o código mais simples
						sf::FloatRect intersection;//rect auxiliar
						boundsJ.intersects(boundsO, intersection);// verficia se eles se interseptam e retorna a área dessa colisão em intersection
						if (intersection.width > intersection.height)
						{
							if (boundsJ.top < boundsO.top)//colidiu por baixo
							{
								p1->setVel_Grav(0);//faz com que ele não sofra gravidade caso esteja na colisão por baixo
								p1->setY(boundsO.top - boundsJ.height);//ajusta o desenho
							}
							else
							{
								p1->setY(boundsO.top + boundsO.height);//ajusta o desenho
							}
						}
						else
						{
							if (boundsJ.left < boundsO.left)//colisao 
							{
								p1->setMovimentoY(0.0f);
								p1->setX(boundsO.left - boundsJ.width);//ajusta o desenho
							}
							else
							{
								p1->setMovimentoY(0.0f);
								p1->setX(boundsO.left + boundsO.width);//ajusta o desenho
							}
							p1->setVel_Grav(0);//rdefine a velocidade da grav que foi acumulada
						}
						p1->setPosicaoGrafica(p1->getX(), p1->getY());//define a nova posição
						obst->obstaculizar(p1);//faz o efeito de obstacularizar 
					}
				}
			}

			if (p2 && p2->getFigura() && p2->getAtivo())
			{
				sf::FloatRect boundsJ = p2->getFigura()->getGlobalBounds();
				for (std::list<Obstaculos::Obstaculo*>::iterator it = LOs.begin(); it != LOs.end(); ++it)//percorre a lista
				{
					Obstaculos::Obstaculo* obst = *it;

					float dx = p2->getX() - obst->getX();//distância entre o centro x e y dos elementos
					float dy = p2->getY() - obst->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)//se os dois elementos estão a uma distância maior que a definida lá encima (64 pixels) ele não verifica colisão, já que eles estão geograficamente longe
					{
						continue;//quebra o looping, dava pra fazer a suficiencia da relação (ex: a tal b, fazer b tal a) o que talvez seria mais natural, porem a gente quis explicitar a quebra do loop
					}

					if (verificarColisao(p2, obst))//se tem colisão
					{
						sf::FloatRect boundsO = obst->getFigura()->getGlobalBounds();//variavel auxiliar para deixar o código mais simples
						sf::FloatRect intersection;//rect auxiliar
						boundsJ.intersects(boundsO, intersection);// verficia se eles se interseptam e retorna a área dessa colisão em intersection

						if (intersection.width > intersection.height) {
							if (boundsJ.top < boundsO.top)//colidiu por baixo
							{
								p2->setVel_Grav(0);//faz com que ele não sofra gravidade caso esteja na colisão por baixo
								p2->setY(boundsO.top - boundsJ.height);//ajusta o desenho
							}
							else
							{
								p2->setMovimentoY(2.0f);
								p2->setVel_Grav(0);
								p2->setY(boundsO.top + boundsO.height);//ajusta o desenho
							}
						}
						else {

							if (boundsJ.left < boundsO.left)//colisao 
								p2->setX(boundsO.left - boundsJ.width);//ajusta o desenho
							else
								p2->setX(boundsO.left + boundsO.width);//ajusta o desenho

						}
						p2->setPosicaoGrafica(p2->getX(), p2->getY());//define a nova posição
						obst->obstaculizar(p2);//faz o efeito de obstacularizar 
					}
				}
			}
		}

		void Gerenciador_Colisoes::tratarColisoesInimgsObstacs() //percorre a lista de inimigos com obstaculos
		{
			for (std::vector<Personagens::Inimigo*>::iterator it_inim = LIs.begin(); it_inim != LIs.end(); ++it_inim)//percorre a lista
			{
				Personagens::Inimigo* inim = *it_inim;
				if (inim && inim->getFigura() && inim->getAtivo())
				{
					sf::FloatRect boundsI = inim->getFigura()->getGlobalBounds();

					for (std::list<Obstaculos::Obstaculo*>::iterator it_obst = LOs.begin(); it_obst != LOs.end(); ++it_obst)//percorre a lista
					{
						Obstaculos::Obstaculo* obst = *it_obst;

						//normalizacao da dsitancia para raio de checkagem
						float dx = inim->getX() - obst->getX(); //distância entre o centro x e y dos elementos
						float dy = inim->getY() - obst->getY();
						float distQuadrada = (dx * dx) + (dy * dy);
						if (distQuadrada > RAIO_CHEKAGEM_QUADRADO) //se os dois elementos estão a uma distância maior que a definida lá encima (64 pixels) ele não verifica colisão, já que eles estão geograficamente longe
						{
							continue; //quebra o looping, dava pra fazer a suficiencia da relação (ex: a tal b, fazer b tal a) o que talvez seria mais natural, porem a gente quis explicitar a quebra do loop
						}

						if (verificarColisao(inim, obst)) //se tem colisão
						{
							sf::FloatRect boundsO = obst->getFigura()->getGlobalBounds(); //variavel auxiliar para deixar o código mais simples
							sf::FloatRect intersection; //rect auxiliar
							boundsI.intersects(boundsO, intersection); // verficia se eles se interseptam e retorna a área dessa colisão em intersection
							if (intersection.width > intersection.height)
							{
								if (boundsI.top < boundsO.top) //colidiu por baixo
								{
									inim->setVel_Grav(0); //faz com que ele não sofra gravidade caso esteja na colisão por baixo
									inim->setY(boundsO.top - boundsI.height); //ajusta o desenho
								}
								else
								{
									inim->setVel_Grav(0);
									inim->setY(boundsO.top + boundsO.height); //ajusta o desenho
								}
							}
							else
							{
								if (boundsI.left < boundsO.left) //colisao 
									inim->setX(boundsO.left - boundsI.width); //ajusta o desenho
								else
									inim->setX(boundsO.left + boundsO.width); //ajusta o desenho
							}
							inim->setPosicaoGrafica(inim->getX(), inim->getY()); //define a nova posição
						}
					}
				}
			}
		}


		void Gerenciador_Colisoes::tratarColisoesProjeteisObstacs()//percorre lista de projeteis com obstaculos
		{
			std::set<Entidades::Projetil*>::iterator it_proj = LPs.begin();
			while (it_proj != LPs.end())//percorre o set
			{
				Entidades::Projetil* proj = *it_proj;
				if (!proj->getAtivo())
				{
					++it_proj;
					continue;
				}

				for (std::list<Obstaculos::Obstaculo*>::iterator it_obst = LOs.begin(); it_obst != LOs.end(); ++it_obst)//percorre a lista
				{
					Obstaculos::Obstaculo* obst = *it_obst;

					//normalizacao da dsitancia para raio de checkagem
					float dx = proj->getX() - obst->getX();
					float dy = proj->getY() - obst->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)//se os dois elementos estão a uma distância maior que a definida lá encima (64 pixels) ele não verifica colisão, já que eles estão geograficamente longe
					{
						continue;//quebra o looping, dava pra fazer a suficiencia da relação (ex: a tal b, fazer b tal a) o que talvez seria mais natural, porem a gente quis explicitar a quebra do loop
					}

					if (verificarColisao(proj, obst))//se tem colisão
					{
						proj->setAtivo(false);
						break;//quebra o looping
					}
				}
				++it_proj;
			}
		}

		void Gerenciador_Colisoes::tratarColisoesObstacsObstacs()//percorre lista de obstaculos com obstaculos
		{
			for (std::list<Obstaculos::Obstaculo*>::iterator it_A = LOs.begin(); it_A != LOs.end(); ++it_A)//percorre a lista
			{
				Obstaculos::Obstaculo* obstA = *it_A;
				if (!obstA || !obstA->getAtivo())
					continue;

				for (std::list<Obstaculos::Obstaculo*>::iterator it_B = LOs.begin(); it_B != LOs.end(); ++it_B)//percorre a lista
				{
					if (it_A == it_B)//nao colide consigo mesmo
					{
						continue;
					}


					Obstaculos::Obstaculo* obstB = *it_B;
					if (!obstB || !obstB->getAtivo())
					{
						continue;
					}

					//normalizacao da dsitancia para raio de checkagem
					float dx = obstA->getX() - obstB->getX();
					float dy = obstA->getY() - obstB->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)//se os dois elementos estão a uma distância maior que a definida lá encima (64 pixels) ele não verifica colisão, já que eles estão geograficamente longe
					{
						continue;//quebra o looping, dava pra fazer a suficiencia da relação (ex: a tal b, fazer b tal a) o que talvez seria mais natural, porem a gente quis explicitar a quebra do loop
					}

					if (verificarColisao(obstA, obstB))//se tem colisão
					{
						sf::FloatRect boundsA = obstA->getFigura()->getGlobalBounds();
						sf::FloatRect boundsB = obstB->getFigura()->getGlobalBounds();
						sf::FloatRect intersection;
						boundsA.intersects(boundsB, intersection);

						if (intersection.width > intersection.height)
						{
							if (boundsA.top < boundsB.top)//colidiu por baixo
							{
								obstA->setVel_Grav(0);
								obstA->setY(boundsB.top - boundsA.height);//ajusta o desenho
							}
							else
							{
								obstA->setVel_Grav(0);
								obstA->setY(boundsB.top + boundsB.height);//ajusta o desenho
							}
						}
						else
						{
							if (boundsA.left < boundsB.left)//colisao 
								obstA->setX(boundsB.left - boundsA.width);//ajusta o desenho
							else
								obstA->setX(boundsB.left + boundsB.width);//ajusta o desenho
						}
						obstA->setPosicaoGrafica(obstA->getX(), obstA->getY());//define a nova posição
					}
				}
			}
		}


		void Gerenciador_Colisoes::tratarColisoesJogsInimgs() //percorre a lista de inimigos com jogadores
		{
			if (p1 && p1->getAtivo())
			{
				for (std::vector<Personagens::Inimigo*>::iterator it_inim = LIs.begin(); it_inim != LIs.end(); ++it_inim) //percorre a lista
				{
					Personagens::Inimigo* inim = *it_inim;
					if (!inim || !inim->getAtivo())
					{
						continue;
					}

					//normalizacao da dsitancia para raio de checkagem
					float dx = p1->getX() - inim->getX(); //distância entre o centro x e y dos elementos
					float dy = p1->getY() - inim->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO) //se os dois elementos estão a uma distância maior que a definida lá encima (64 pixels) ele não verifica colisão, já que eles estão geograficamente longe
					{
						continue; //quebra o looping, dava pra fazer a suficiencia da relação (ex: a tal b, fazer b tal a) o que talvez seria mais natural, porem a gente quis explicitar a quebra do loop
					}

					if (verificarColisao(p1, inim)) //se tem colisão
					{
						inim->danificar(p1);
						sf::FloatRect boundsJ = p1->getFigura()->getGlobalBounds();
						sf::FloatRect boundsI = inim->getFigura()->getGlobalBounds();
						sf::FloatRect intersection;
						boundsJ.intersects(boundsI, intersection); // verficia se eles se interseptam e retorna a área dessa colisão em intersection
						if (intersection.width > intersection.height) {
							if (boundsJ.top < boundsI.top) p1->setY(boundsI.top - boundsJ.height); //empurra pra cima, ajusta o desenho
							else p1->setY(boundsI.top + boundsI.height); //empurra pra baixo, ajusta o desenho
						}
						else {
							if (boundsJ.left < boundsI.left) p1->setX(boundsI.left - boundsJ.width - 1.0f); //empurra pra esquerda, ajusta o desenho
							else p1->setX(boundsI.left + boundsI.width + 1.0f); //empurra pra direita, ajusta o desenho
						}
						p1->setPosicaoGrafica(p1->getX(), p1->getY()); //define a nova posição
					}
				}
			}

			if (p2 && p2->getAtivo())
			{
				for (std::vector<Personagens::Inimigo*>::iterator it_inim = LIs.begin(); it_inim != LIs.end(); ++it_inim) //percorre a lista
				{
					Personagens::Inimigo* inim = *it_inim;
					if (!inim || !inim->getAtivo())
					{
						continue;
					}

					//normalizacao da dsitancia para raio de checkagem
					float dx = p2->getX() - inim->getX(); //distância entre o centro x e y dos elementos
					float dy = p2->getY() - inim->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO) //se os dois elementos estão a uma distância maior que a definida lá encima (64 pixels) ele não verifica colisão, já que eles estão geograficamente longe
					{
						continue; //quebra o looping, dava pra fazer a suficiencia da relação (ex: a tal b, fazer b tal a) o que talvez seria mais natural, porem a gente quis explicitar a quebra do loop
					}

					if (verificarColisao(p2, inim)) //se tem colisão
					{
						inim->danificar(p2);
						sf::FloatRect boundsJ = p2->getFigura()->getGlobalBounds();
						sf::FloatRect boundsI = inim->getFigura()->getGlobalBounds();
						sf::FloatRect intersection;
						boundsJ.intersects(boundsI, intersection); // verficia se eles se interseptam e retorna a área dessa colisão em intersection
						if (intersection.width > intersection.height) {
							if (boundsJ.top < boundsI.top) p2->setY(boundsI.top - boundsJ.height); //empurra pra cima, ajusta o desenho
							else p2->setY(boundsI.top + boundsI.height); //empurra pra baixo, ajusta o desenho
						}
						else {
							if (boundsJ.left < boundsI.left) p2->setX(boundsI.left - boundsJ.width - 1.0f); //empurra pra esquerda, ajusta o desenho
							else p2->setX(boundsI.left + boundsI.width + 1.0f); //empurra pra direita, ajusta o desenho
						}
						p2->setPosicaoGrafica(p2->getX(), p2->getY()); //define a nova posição
					}
				}
			}
		}

		void Gerenciador_Colisoes::tratarColisoesJogsProjeteis() //percorre projeteis verificando se acertou jogador
		{
			if (p1 && p1->getAtivo())
			{
				std::set<Entidades::Projetil*>::iterator it_proj = LPs.begin();
				while (it_proj != LPs.end()) //percorre o set
				{
					Entidades::Projetil* proj = *it_proj;

					//normalizacao da dsitancia para raio de checkagem
					float dx = p1->getX() - proj->getX();
					float dy = p1->getY() - proj->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO) //se os dois elementos estão a uma distância maior que a definida lá encima (64 pixels) ele não verifica colisão, já que eles estão geograficamente longe
					{
						++it_proj;
						continue; //quebra o looping, dava pra fazer a suficiencia da relação (ex: a tal b, fazer b tal a) o que talvez seria mais natural, porem a gente quis explicitar a quebra do loop
					}

					//verifica se colidiu e se o dono do projetil NÃO é o jogador (idDono == 0 é inimigo)
					if (proj->getAtivo() && proj->getIdDono() == 0 && verificarColisao(p1, proj))
					{
						for (int i = 0; i < proj->getDano(); i++) {
							p1->operator--(); //aplica dano
						}
						proj->setAtivo(false); //destroi o projetil
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

					//normalizacao da dsitancia para raio de checkagem
					float dx = p2->getX() - proj->getX();
					float dy = p2->getY() - proj->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
					{
						++it_proj;
						continue;
					}

					//verifica se colidiu e se o dono do projetil NÃO é o jogador 
					if (proj->getAtivo() && proj->getIdDono() == 0 && verificarColisao(p2, proj))
					{
						for (int i = 0; i < proj->getDano(); i++) {
							p2->operator--(); //dano
						}
						proj->setAtivo(false); //destroi
					}
					++it_proj;
				}
			}
		}

		void Gerenciador_Colisoes::tratarColisoesInimgsProjeteis() //percorre projeteis com inimigos
		{
			std::set<Entidades::Projetil*>::iterator it_proj = LPs.begin();
			while (it_proj != LPs.end()) //percorre set de projeteis
			{
				Entidades::Projetil* proj = *it_proj;

				if (proj->getAtivo() && proj->getIdDono() > 0) //verifica se o projetil pertence a um jogador (id > 0)
				{
					std::vector<Personagens::Inimigo*>::iterator it_inim = LIs.begin();
					while (it_inim != LIs.end()) //percorre lista de inimigos
					{
						Personagens::Inimigo* inim = *it_inim;
						if (!inim || !inim->getAtivo())
						{
							++it_inim;
							continue;
						}

						//normalizacao da dsitancia para raio de checkagem
						float dx = proj->getX() - inim->getX();
						float dy = proj->getY() - inim->getY();
						float distQuadrada = (dx * dx) + (dy * dy);
						if (distQuadrada > RAIO_CHEKAGEM_QUADRADO) //se os dois elementos estão a uma distância maior que a definida lá encima (64 pixels) ele não verifica colisão, já que eles estão geograficamente longe
						{
							++it_inim;
							continue; //quebra o looping, dava pra fazer a suficiencia da relação (ex: a tal b, fazer b tal a) o que talvez seria mais natural, porem a gente quis explicitar a quebra do loop
						}

						if (verificarColisao(proj, inim)) //se tem colisão
						{
							for (int i = 0; i < proj->getDano(); i++)
							{
								inim->operator--(); //aplica dano no inimigo
							}

							if (!inim->getAtivo()) //se inimigo morreu
							{
								int pontos = inim->getPontosPorMorte();
								//atribui pontos ao dono do projetil
								if (proj->getIdDono() == 1 && p1) {
									p1->adicionarPontos(pontos);
								}
								else if (proj->getIdDono() == 2 && p2) {
									p2->adicionarPontos(pontos);
								}
							}

							proj->setAtivo(false); //destroi o projetil após impacto
							break; //sai do loop de inimigos pois o projetil já foi usado, quebra o looping
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

		void Gerenciador_Colisoes::tratarColisoesInimgsInimgs()//colisao inimigo com inimigo
		{
			for (std::vector<Personagens::Inimigo*>::iterator it_A = LIs.begin(); it_A != LIs.end(); ++it_A)//percorre vetor
			{
				Personagens::Inimigo* inimA = *it_A;
				if (!inimA->getAtivo())
				{
					continue;
				}

				for (std::vector<Personagens::Inimigo*>::iterator it_B = std::next(it_A); it_B != LIs.end(); ++it_B)//percorre vetor
				{
					Personagens::Inimigo* inimB = *it_B;
					if (!inimB->getAtivo()) continue;
					//normalizacao da dsitancia para raio de checkagem
					float dx = inimA->getX() - inimB->getX();
					float dy = inimA->getY() - inimB->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO) //se os dois elementos estão a uma distância maior que a definida lá encima (64 pixels) ele não verifica colisão, já que eles estão geograficamente longe
					{
						continue; //quebra o looping, dava pra fazer a suficiencia da relação (ex: a tal b, fazer b tal a) o que talvez seria mais natural, porem a gente quis explicitar a quebra do loop
					}

					if (verificarColisao(inimA, inimB))//se colidiu
					{
						sf::FloatRect boundsA = inimA->getFigura()->getGlobalBounds();
						sf::FloatRect boundsB = inimB->getFigura()->getGlobalBounds();
						sf::FloatRect intersection;
						boundsA.intersects(boundsB, intersection);

						float pushX = 0, pushY = 0;

						if (intersection.width > intersection.height)//empurra verticalmente
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
						else//empurra horizontalmente
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

		void Gerenciador_Colisoes::tratarColisoesJogsChao() //percorre a lista de jogadores com o chão
		{
			if (p1 && p1->getFigura() && p1->getAtivo())
			{
				sf::FloatRect boundsJ = p1->getFigura()->getGlobalBounds();
				for (std::list<Entidades::Chao*>::iterator it = LCs.begin(); it != LCs.end(); ++it) //percorre a lista
				{
					Entidades::Chao* chao = *it;

					//normalizacao da dsitancia para raio de checkagem
					float dx = p1->getX() - chao->getX(); //distância entre o centro x e y dos elementos
					float dy = p1->getY() - chao->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO) //se os dois elementos estão a uma distância maior que a definida lá encima (64 pixels) ele não verifica colisão, já que eles estão geograficamente longe
					{
						continue; //quebra o looping, dava pra fazer a suficiencia da relação (ex: a tal b, fazer b tal a) o que talvez seria mais natural, porem a gente quis explicitar a quebra do loop
					}

					if (verificarColisao(p1, chao)) //se tem colisão
					{
						sf::FloatRect boundsC = chao->getFigura()->getGlobalBounds(); //variavel auxiliar
						sf::FloatRect intersection; //rect auxiliar
						boundsJ.intersects(boundsC, intersection); //calcula intersecção
						if (intersection.width > intersection.height) {
							if (boundsJ.top < boundsC.top) //colisão vertical (jogador caindo no chão)
							{
								p1->setVel_Grav(0); //faz com que ele não sofra gravidade caso esteja na colisão por baixo
								p1->setY(boundsC.top - boundsJ.height); //ajusta o desenho
							}
							else
							{
								p1->setVel_Grav(p1->getVel_Grav() * 1.5); //rebote se bater a cabeça
								p1->setY(boundsC.top + boundsC.height); //ajusta o desenho
							}
						}
						else {
							if (boundsJ.left < boundsC.left) //colisão lateral esquerda
							{
								p1->setX(boundsC.left - boundsJ.width); //ajusta o desenho
							}
							else //colisão lateral direita
							{
								p1->setX(boundsC.left + boundsC.width); //ajusta o desenho
							}
							p1->setVel_Grav(0); //rdefine a velocidade da grav que foi acumulada
						}
						p1->setPosicaoGrafica(p1->getX(), p1->getY()); //define a nova posição
					}
				}
			}

			if (p2 && p2->getFigura() && p2->getAtivo())
			{
				sf::FloatRect boundsJ = p2->getFigura()->getGlobalBounds();
				for (std::list<Entidades::Chao*>::iterator it = LCs.begin(); it != LCs.end(); ++it) //percorre lista
				{
					Entidades::Chao* chao = *it;
					//normalizacao da dsitancia para raio de checkagem
					float dx = p2->getX() - chao->getX(); //distância entre o centro x e y dos elementos
					float dy = p2->getY() - chao->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO) //se os dois elementos estão a uma distância maior que a definida lá encima (64 pixels) ele não verifica colisão, já que eles estão geograficamente longe
					{
						continue; //quebra o looping, dava pra fazer a suficiencia da relação (ex: a tal b, fazer b tal a) o que talvez seria mais natural, porem a gente quis explicitar a quebra do loop
					}

					if (verificarColisao(p2, chao)) //se colidiu
					{
						sf::FloatRect boundsC = chao->getFigura()->getGlobalBounds();
						sf::FloatRect intersection;
						boundsJ.intersects(boundsC, intersection);

						if (intersection.width > intersection.height) {
							if (boundsJ.top < boundsC.top) //colisao vertical
							{
								p2->setVel_Grav(0); //faz com que ele não sofra gravidade caso esteja na colisão por baixo
								p2->setY(boundsC.top - boundsJ.height); //ajusta desenho
							}
							else
							{
								p2->setVel_Grav(p2->getVel_Grav() * 1.5); //rebote
								p2->setY(boundsC.top + boundsC.height); //ajusta desenho
							}
						}
						else {

							if (boundsJ.left < boundsC.left) //colisao lateral esquerda
							{
								p2->setX(boundsC.left - boundsJ.width);
							}

							else //colisao lateral direita
							{
								p2->setX(boundsC.left + boundsC.width);
							}
							p2->setVel_Grav(0); //rdefine a velocidade da grav que foi acumulada
						}
						p2->setPosicaoGrafica(p2->getX(), p2->getY()); //atualiza posicao
					}
				}
			}
		}

		void Gerenciador_Colisoes::tratarColisoesProjeteisChao() //percorre projeteis com chão
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

				for (std::list<Entidades::Chao*>::iterator it_chao = LCs.begin(); it_chao != LCs.end(); ++it_chao) //percorre lista de chão
				{
					Entidades::Chao* chao = *it_chao;
					//normalizacao da dsitancia para raio de checkagem
					float dx = proj->getX() - chao->getX();
					float dy = proj->getY() - chao->getY();
					float distQuadrada = (dx * dx) + (dy * dy);
					if (distQuadrada > RAIO_CHEKAGEM_QUADRADO) //se os dois elementos estão a uma distância maior que a definida lá encima (64 pixels) ele não verifica colisão, já que eles estão geograficamente longe
					{
						continue; //quebra o looping, dava pra fazer a suficiencia da relação (ex: a tal b, fazer b tal a) o que talvez seria mais natural, porem a gente quis explicitar a quebra do loop
					}

					if (verificarColisao(proj, chao)) //se tem colisão
					{
						proj->setAtivo(false); //destroi o projetil ao bater na parede/chão
						break; //quebra o looping
					}
				}
				++it_proj;
			}
		}

		void Gerenciador_Colisoes::tratarColisoesInimgsChao() //percorre inimigos com chao
		{
			for (std::vector<Personagens::Inimigo*>::iterator it_inim = LIs.begin(); it_inim != LIs.end(); ++it_inim)//percorre vetor
			{
				Personagens::Inimigo* inim = *it_inim;
				if (inim && inim->getFigura() && inim->getAtivo())
				{
					for (std::list<Entidades::Chao*>::iterator it_chao = LCs.begin(); it_chao != LCs.end(); ++it_chao)//percorre lista
					{
						Entidades::Chao* chao = *it_chao;

						//normalizacao distancia para raio de checkagem
						float dx = inim->getX() - chao->getX();
						float dy = inim->getY() - chao->getY();
						float distQuadrada = (dx * dx) + (dy * dy);
						if (distQuadrada > RAIO_CHEKAGEM_QUADRADO)
						{
							continue; //quebra o looping
						}

						if (verificarColisao(inim, chao))//se colidiu
						{
							sf::FloatRect boundsI = inim->getFigura()->getGlobalBounds();
							sf::FloatRect boundsC = chao->getFigura()->getGlobalBounds();
							sf::FloatRect intersection;
							boundsI.intersects(boundsC, intersection);
							inim->setVel_Grav(0);//anula gravidade
							if (intersection.width > intersection.height)//colisao vertical
							{
								if (boundsI.top < boundsC.top) //colidiu por cima
								{
									inim->setVel_Grav(0);
									inim->setY(boundsC.top - boundsI.height);//ajusta desenho
								}
								else
								{
									inim->setVel_Grav(0);
									inim->setY(boundsC.top + boundsC.height);//ajusta desenho
								}
							}
							else//colisao lateral
							{
								if (boundsI.left < boundsC.left)
									inim->setX(boundsC.left - boundsI.width);
								else
									inim->setX(boundsC.left + boundsC.width);
							}
							inim->setPosicaoGrafica(inim->getX(), inim->getY());//atualiza posicao
						}
					}
				}
			}
		}


		void Gerenciador_Colisoes::tratarColisoesObstacsChao() //percorre obstaculos com chão
		{
			for (std::list<Obstaculos::Obstaculo*>::iterator it_obst = LOs.begin(); it_obst != LOs.end(); ++it_obst) //percorre lista de obstaculos
			{
				Obstaculos::Obstaculo* obst = *it_obst;
				if (obst && obst->getFigura() && obst->getAtivo())
				{
					sf::FloatRect boundsO = obst->getFigura()->getGlobalBounds();

					for (std::list<Entidades::Chao*>::iterator it_chao = LCs.begin(); it_chao != LCs.end(); ++it_chao) //percorre lista de chão
					{
						Entidades::Chao* chao = *it_chao;
						//normalizacao da dsitancia para raio de checkagem
						float dx = obst->getX() - chao->getX();
						float dy = obst->getY() - chao->getY();
						float distQuadrada = (dx * dx) + (dy * dy);
						if (distQuadrada > RAIO_CHEKAGEM_QUADRADO) //se os dois elementos estão a uma distância maior que a definida lá encima (64 pixels) ele não verifica colisão, já que eles estão geograficamente longe
						{
							continue; //quebra o looping, dava pra fazer a suficiencia da relação (ex: a tal b, fazer b tal a) o que talvez seria mais natural, porem a gente quis explicitar a quebra do loop
						}

						if (verificarColisao(obst, chao)) //se tem colisão
						{
							sf::FloatRect boundsC = chao->getFigura()->getGlobalBounds(); //variavel auxiliar
							sf::FloatRect intersection; //rect auxiliar
							boundsO.intersects(boundsC, intersection);
							if (intersection.width > intersection.height)
							{
								if (boundsO.top < boundsC.top) //colidiu por cima
								{
									//obst->setVel_Grav(0);
									obst->setY(boundsC.top - boundsO.height); //ajusta o desenho
								}
								else
								{
									//obst->setVel_Grav(0);
									obst->setY(boundsC.top + boundsC.height); //ajusta o desenho
								}
							}
							//obst->setY(0);
							obst->setVel_Grav(0); //faz com que ele não sofra gravidade caso esteja na colisão por baixo
							obst->setPosicaoGrafica(obst->getX(), obst->getY()); //define a nova posição
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
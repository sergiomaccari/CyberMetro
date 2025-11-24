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
#include <fstream>   //leitura e escrita de arquivos     


using json = nlohmann::json;
using namespace CyberMetro::Entidades;
using namespace CyberMetro::Entidades::Personagens;
using namespace CyberMetro::Gerenciadores;
using namespace CyberMetro::Listas;
using namespace CyberMetro;

namespace CyberMetro {
	namespace Fases {

		//constantes minimo e maximo de obstaculos e inimigos

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
			areaDeSaida(0.f, 0.f, 0.f, 0.f),//se tocar na porta a fase termina
			terminou(false)
		{
			relogioFase.restart();

			if (!texturaSaida.loadFromFile("assets/saida.png"))
			{
				std::cerr << "Erro: Nao foi possivel carregar assets/saida.png" << std::endl;
			}
			spriteSaida.setTexture(texturaSaida);

			//define listas e gerenciador 
			pListaInimigos = new Listas::ListaEntidades();
			pListaObstaculos = new Listas::ListaEntidades();
			pListaChao = new Listas::ListaEntidades();
			pColisoes = new Gerenciadores::Gerenciador_Colisoes(pJogador1);

			if (pJogador2 && pJogador2->getAtivo()) 
			{
				pColisoes->setJogador2(pJogador2);//define o jogador 2 no gerenciadorcolisao se ele existir
			}
		}

		Fase::~Fase()
		{
			limpar();
		}

		void Fase::setJogo(CyberMetro::Jogo* pJ)//relação bidirecional
		{
			pJogo = pJ;
		}

		void Fase::adicionarJogador2(Entidades::Personagens::Jogador* j2)//metodo para adicionar o jogador2, chamado vai ser depois
		{
			pJogador2 = j2;

			if (pColisoes) 
			{
				pColisoes->setJogador2(pJogador2);
			}
		}

		void Fase::verificarFimDeFase()//aqui que tem o negocio da porta
		{
			if (terminou || areaDeSaida.width == 0 || !pJogo) //aqui a relaçãop bidirecional é usada, pega conceito
			{
				return;
			}

			if (pJogador1 && pJogador1->getAtivo()) //se jogador1 intersectar com a saida a fase termina
			{
				if (areaDeSaida.intersects(pJogador1->getFigura()->getGlobalBounds())) 
				{
					terminou = true;
				}
			}

			if (pJogador2 && pJogador2->getAtivo())  //se jogador2 intersectar com a saida a fase termina
			{
				if (areaDeSaida.intersects(pJogador2->getFigura()->getGlobalBounds())) 
				{
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
			{
				Lista<Entidades::Entidade>::Elemento* atualI = pListaInimigos->getPrimeiro();
				while (atualI) 
				{
					Entidades::Entidade* pEnt = atualI->getInfo();//variavel auxilar
					Entidades::Personagens::Robo_CEO* robCeo = dynamic_cast<Entidades::Personagens::Robo_CEO*>(pEnt);//testa para ver se é CEO
					if (robCeo && robCeo->getAtivo())
					{
						if (pJogador1 && pJogador1->getAtivo())//tem prioridade no jogador 1
						{
							robCeo->setJogador(pJogador1);
						}
						else if (pJogador2 && pJogador2->getAtivo())//se nao tiver jogador 1 ele foca no jogador 2
						{
							robCeo->setJogador(pJogador2);
						}
						else//para de atirar qunado não tem jogadores pois ele nao esta olhando para ninguem
						{
							robCeo->setJogador(nullptr);
						}
					}
					atualI = atualI->getProx();
				}

			}

			//vai executando as listas
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

			if (pColisoes) 
			{	//limpa as listas do gerenciador
				pColisoes->limparInimigos();
				pColisoes->limparObstaculos();
				pColisoes->limparProjeteis();
				pColisoes->limparChao();

				if (pListaInimigos)
				{ // incluindo inimigos na lista para verificar colisões
					Lista<Entidades::Entidade>::Elemento* atualI = pListaInimigos->getPrimeiro();
					while (atualI) 
					{
						Entidades::Entidade* pEnt = atualI->getInfo();
						Entidades::Personagens::Inimigo* inim = dynamic_cast<Entidades::Personagens::Inimigo*>(pEnt);//verifica se é inimigo
						if (inim && inim->getAtivo()) //se sim inclui na lista do gerenciador de colisões
						{
							pColisoes->incluirInimigo(inim);
						}
						atualI = atualI->getProx();
					}
				}

				if (pListaObstaculos)
				{ // incluindo obstaculos na lista para verificar colisões
					Lista<Entidades::Entidade>::Elemento* atualO = pListaObstaculos->getPrimeiro();
					while (atualO) {
						Entidades::Entidade* pEnt = atualO->getInfo();
						Entidades::Obstaculos::Obstaculo* obst = dynamic_cast<Entidades::Obstaculos::Obstaculo*>(pEnt);//verifica se é obstaculo
						if (obst && obst->getAtivo())//se sim coloca na lista
						{
							pColisoes->incluirObstaculo(obst);
						}
						atualO = atualO->getProx();
					}
				}

				if (pListaChao)
				{//inclui chao para ver colisoes
					Lista<Entidades::Entidade>::Elemento* atualC = pListaChao->getPrimeiro();
					while (atualC) 
					{
						Entidades::Entidade* pEnt = atualC->getInfo();
						Entidades::Chao* chao = dynamic_cast<Entidades::Chao*>(pEnt);//verificação se chao
						if (chao && chao->getAtivo()) //se sim coloca na lista
						{
							pColisoes->incluirChao(chao);
						}
						atualC = atualC->getProx();
					}
				}

				if (pJogador1)
				{//verificação de colisoes com projteis lançados por jogador 1
					Listas::ListaEntidades* projJog = pJogador1->getProjeteis();
					Lista<Entidades::Entidade>::Elemento* atualPJog = projJog->getPrimeiro();
					while (atualPJog) 
					{
						Entidades::Entidade* p = atualPJog->getInfo();
						if (p->getAtivo()) 
						{
							Entidades::Projetil* pProj = dynamic_cast<Entidades::Projetil*>(p);//verifica se é projetil
							if (pProj)//poe na lista
							{
								pColisoes->incluirProjetil(pProj);
							}
						}
						atualPJog = atualPJog->getProx();
					}
				}

				if (pJogador2)
				{//verificação de colisoes com projteis lançados por jogador 2
					Listas::ListaEntidades* projJog2 = pJogador2->getProjeteis();
					Lista<Entidades::Entidade>::Elemento* atualPJog2 = projJog2->getPrimeiro();
					while (atualPJog2) 
					{
						Entidades::Entidade* p = atualPJog2->getInfo();
						if (p->getAtivo()) 
						{
							Entidades::Projetil* pProj = dynamic_cast<Entidades::Projetil*>(p);//verifica se é projetil
							if (pProj) //colcoa na lsiat caso seja
							{
								pColisoes->incluirProjetil(pProj);//poe na lista
							}
						}
						atualPJog2 = atualPJog2->getProx();
					}
				}

				if (pListaInimigos)
				{//verificação projeteis robo_ceo
					Lista<Entidades::Entidade>::Elemento* atualI = pListaInimigos->getPrimeiro();
					while (atualI) 
					{
						if (atualI->getInfo()->getAtivo()) 
						{
							Entidades::Personagens::Robo_CEO* robCeo = dynamic_cast<Entidades::Personagens::Robo_CEO*>(atualI->getInfo());//testa se é ceo
							if (robCeo) //caso seja ceo
							{
								Listas::ListaEntidades* projrobCeo = robCeo->getProjeteis();
								Lista<Entidades::Entidade>::Elemento* atualProbCeo = projrobCeo->getPrimeiro();
								while (atualProbCeo) 
								{
									Entidades::Entidade* p = atualProbCeo->getInfo();
									if (p->getAtivo()) 
									{
										Entidades::Projetil* pProj = dynamic_cast<Entidades::Projetil*>(p);//testa se é projetil
										if (pProj) //inclui na lista
										{
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

				pColisoes->executar();//depois de tudo isso executa essa lista verificando se tem colisoes
			}

			verificarFimDeFase();
		}

		void Fase::desenharFase()
		{//desenha as coisas da fase
			Gerenciador_Grafico* pGG = Ente::getGerenciadorGrafico();
			if (pGG)
			{
				pGG->getJanela()->setView(pGG->getJanela()->getDefaultView());//padrao da camera
				pGG->desenharBackground();

				if (pJogador1 && pJogador1->getAtivo())
				{
					pGG->setCamera(pJogador1);//se o jogador 1 ta vivo a cameraq fica nele
				}
				else if (pJogador2 && pJogador2->getAtivo())
				{
					pGG->setCamera(pJogador2);//se o jogador 1 nao ta ativo e o 2 ta a camera fica no 2
				}

				desenharMapa();//desenha o mapa, chamada de função definida mais abaixo

				//abaixo ate o fim do metodo vai desenhando tudo se estiver ativo
				pGG->getJanela()->draw(spriteSaida);
				if (pListaObstaculos) 
				{
					pListaObstaculos->desenhar();
				}

				if (pJogador1) 
				{
					if (pJogador1->getAtivo()) 
					{
						pJogador1->desenhar();

					}

					pJogador1->getProjeteis()->desenhar();
				}
				if (pJogador2) 
				{
					if (pJogador2->getAtivo()) 
					{
						pJogador2->desenhar();
					}
					pJogador2->getProjeteis()->desenhar();
				}

				if (pListaInimigos)
				{
					pListaInimigos->desenhar();
					Lista<Entidades::Entidade>::Elemento* atualI = pListaInimigos->getPrimeiro();
					while (atualI) 
					{
						Entidades::Entidade* pEnt = atualI->getInfo();
						if (pEnt && pEnt->getAtivo()) 
						{
							Entidades::Personagens::Robo_CEO* robCeo = dynamic_cast<Entidades::Personagens::Robo_CEO*>(pEnt);//testa se é ceo
							if (robCeo) //se sim desenha os projeteis
							{
								robCeo->getProjeteis()->desenhar();
							}
						}
						atualI = atualI->getProx();
					}
				}
			}
		}


		void Fase::limpar()//limpa a fase
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

		//construtoras abaixo funcionam similarmente. verifica o ponteiro para gerenciador grafico, chama a construtora, define o ponteiro para gerenciador grafico, inclue na lista apropriada

		void Fase::criarRoboJunior(float x, float y)
		{
			Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
			if (!pGG_local) 
			{
				return;
			}

			Entidades::Personagens::Robo_Junior* inimigo = new Entidades::Personagens::Robo_Junior(x, y);
			inimigo->setGerenciadorGrafico(pGG_local);
			pListaInimigos->inserir(inimigo);
		}

		void Fase::criarRoboJunior(const sf::Vector2f& pos)//sobrecarga, requisito
		{
			this->criarRoboJunior(pos.x, pos.y);
		}

		void Fase::criarPlataforma(float x, float y, int altura)
		{
			Gerenciadores::Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
			if (!pGG_local)
			{
				return;
			}

			Entidades::Obstaculos::Plataforma* plat = new Entidades::Obstaculos::Plataforma(x, y, altura);
			plat->setGerenciadorGrafico(pGG_local);
			pListaObstaculos->inserir(plat);
		}

		void Fase::criarChao(float x, float y)
		{
			Gerenciadores::Gerenciador_Grafico* pGG_local = Ente::getGerenciadorGrafico();
			if (!pGG_local)
			{
				return;
			}

			Entidades::Chao* chao = new Entidades::Chao(x, y);
			chao->setGerenciadorGrafico(pGG_local);
			pListaChao->inserir(chao);
		}

		void Fase::desenharMapa()
		{
			//virtual
		}

		void Fase::criarCenario(const std::string& arquivoJson)//recebe um endereço pro arquivo
		{
			std::ifstream file(arquivoJson);
			if (!file.is_open()) //verifica erro
			{
				std::cerr << "nao foi possivel abrir '" << arquivoJson << "'" << std::endl;
				return;
			}

			json mapa;//variavel auxiliar
			file >> mapa;//carrego o conteúdo de mapa feito no tiled pro json
			file.close();//fecho por boas práticas

			int tileWidth = mapa["tilewidth"];//variáveis auxiliares para diminuir o tamanho do código
			int tileHeight = mapa["tileheight"];
			int width = mapa["width"];
			int height = mapa["height"];

			gridMapa.assign(height, std::vector<unsigned int>(width, 0));//inicia a matriz de vector de int como 0 em tudo

			json data = mapa["layers"][0]["data"];//
			int i = 0;
			for (int y = 0; y < height; ++y) 
			{
				for (int x = 0; x < width; ++x) 
				{
					gridMapa[y][x] = data[i++];//auemnta o valor das posições que tem coisa
				}
			}
			
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					if (gridMapa[y][x] != 0)//se tem algo nessa posição
					{//vai verificar se a posição do grid vizinha é válida para ter a possibilidade de ter colisao, se uma delas for ele vai criar o chao. ATENÇÃO: CHAO NAO VAI SER DESENHADO
						bool vazioCima = (y - 1 < 0) || (gridMapa[y - 1][x] == 0);
						bool vazioBaixo = (y + 1 >= height) || (gridMapa[y + 1][x] == 0);
						bool vazioEsquerda = (x - 1 < 0) || (gridMapa[y][x - 1] == 0);
						bool vazioDireita = (x + 1 >= width) || (gridMapa[y][x + 1] == 0);

						if (vazioCima || vazioBaixo || vazioEsquerda || vazioDireita)
						{
							float posX = x * tileWidth;
							float posY = y * tileHeight;
							Fase::criarChao(posX, posY);
						}
					}
				}
			}
		}

		bool Fase::getTerminou() const//retorna se a fase terminou
		{
			return terminou;
		}

		sf::FloatRect Fase::getAreaSaida() const//retorna a area para termianr a fase
		{
			return areaDeSaida;
		}

		float Fase::getTempoDecorrido() const//util pra determinar a pontuacao
		{
			return relogioFase.getElapsedTime().asSeconds();
		}

		Listas::ListaEntidades* Fase::getListaInimigos() const//retorna o primeiro da lista
		{
			return pListaInimigos;
		}

		Listas::ListaEntidades* Fase::getListaObstaculos() const//retorna o primeiro da lista
		{
			return pListaObstaculos;
		}

		Listas::ListaEntidades* Fase::getListaChao() const//retorna o primeiro da lista
		{
			return pListaChao;
		}


		//POR QUE PERCORRE VARIAS LISTAS?
		void Fase::salvarFase(json& arq)//recebe um endereço de arquivo
		{
			arq["inimigos"] = json::array();
			arq["obstaculos"] = json::array();
			arq["projeteis"] = json::array();

			auto* elem = pListaInimigos->getPrimeiro(); //auto substitue CyberMetro::Listas::Lista<CyberMetro::Entidades::Entidade>::Elemento*
			while (elem)
			{
				Entidade* ent = elem->getInfo();

				Inimigo* inim = dynamic_cast<Inimigo*>(ent);//verifica se é inimigo

				if (inim && inim->getAtivo())//se for inimigo e está vivo
				{
					json j_inim;//arquivo auxiliar
					inim->salvarDataBuffer(j_inim);//chama o salvar polimórfico
					arq["inimigos"].push_back(j_inim);//coloca no arquivo

					if (Robo_CEO* ceo = dynamic_cast<Robo_CEO*>(inim))//testa se é ceo
					{
						auto* projCeo = ceo->getProjeteis()->getPrimeiro();
						while (projCeo)//enquanto existir projéteis
						{
							auto* projEnt = projCeo->getInfo();
							if (projEnt && projEnt->getAtivo())//se o projetil esta ativo
							{
								json j_proj;//cria variavel auxiliar
								projEnt->salvarDataBuffer(j_proj);//chama o salvar polimorfico
								arq["projeteis"].push_back(j_proj);//coloca no json
							}
							projCeo = projCeo->getProx();
						}
					}
				}
				elem = elem->getProx();
			}

			
			if (pListaObstaculos) 
			{
				auto* elemObst = pListaObstaculos->getPrimeiro();//auto substitue CyberMetro::Listas::Lista<CyberMetro::Entidades::Entidade>::Elemento*
				while (elemObst) //enquanto existirem obstáculos
				{
					CyberMetro::Entidades::Entidade* ent = elemObst->getInfo();

					auto* obst = dynamic_cast<CyberMetro::Entidades::Obstaculos::Obstaculo*>(ent);//auto subistui auto* obst = dynamic_cast<CyberMetro::Entidades::Obstaculos::Obstaculo*>(ent);. Testa se é obstaculo
					if (obst && obst->getAtivo()) //se for e se estiver ativo
					{
						json j_obst;//cria arquivo auxiliar
						obst->salvarDataBuffer(j_obst);//chama o salvar polimórfico
						arq["obstaculos"].push_back(j_obst);//coloca na lista
					}

					elemObst = elemObst->getProx();
				}
			}

			
			if (pJogador1) 
			{
				auto* elemProj1 = pJogador1->getProjeteis()->getPrimeiro();//auto substitue CyberMetro::Listas::Lista<CyberMetro::Entidades::Entidade>::Elemento*
				while (elemProj1) //enquanto existirem projéteis
				{
					CyberMetro::Entidades::Entidade* ent = elemProj1->getInfo();
					Projetil* proj = dynamic_cast<CyberMetro::Entidades::Projetil*>(ent);//testa se é projetil

					if (proj && proj->getAtivo()) //se for projetil e se estiver ativo
					{
						json j_proj;//cria variavel auxiliar
						proj->salvarDataBuffer(j_proj);//chama o salvar polimorfico
						arq["projeteis"].push_back(j_proj);//poe na lista
					}

					elemProj1 = elemProj1->getProx();
				}
			}

			if (pJogador2 && pJogador2->getAtivo()) 
			{
				auto* elemProj2 = pJogador2->getProjeteis()->getPrimeiro();//auto substitute CyberMetro::Listas::Lista<CyberMetro::Entidades::Entidade>::Elemento*
				while (elemProj2) 
				{
					CyberMetro::Entidades::Entidade* ent = elemProj2->getInfo();
					Projetil* proj = dynamic_cast<CyberMetro::Entidades::Projetil*>(ent);//testa se é projétil

					if (proj && proj->getAtivo()) //se sim e se está ativo
					{
						json j_proj;//cria variável auxiliar
						proj->salvarDataBuffer(j_proj);//chama o salvar polimorfico
						arq["projeteis"].push_back(j_proj);//coloca na lista
					}

					elemProj2 = elemProj2->getProx();
				}
			}
		}
	}
}
 
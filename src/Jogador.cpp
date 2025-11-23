#include "Jogador.h"

namespace CyberMetro {
	using namespace Entidades::Personagens;
	using namespace Listas;
	namespace Entidades {
		namespace Personagens
		{
			const float Jogador::PROJETIL_VELOCIDADE_JOGADOR = 400.0f;

			Jogador::Jogador(int numJogador) :
				Personagem(),
				pontos(0),
				cooldown(0),
				playerNum(numJogador),
				ultimaDirecaoX(1.0f),
				stun(sf::Time::Zero),
				obstaculoCooldown(sf::Time::Zero)
			{
				this->x = 32;
				this->y = 32;
				this->velocidade = 300.0f;
				this->n_vidas = 150;
				this->n_vidas_max = 150;

				std::string caminhoTextura;

				if (playerNum == 1)
					caminhoTextura = "assets/jogador1.png";

				else
					caminhoTextura = "assets/jogador2.png";

				if (pGG)
				{
					sf::Texture* tex = pGG->getTextura(caminhoTextura);
					if (tex)
					{
						pFigura->setTexture(*tex);
						pFigura->setScale(32.0f / tex->getSize().x, 32.0f / tex->getSize().y);
					}
				}
				else
				{
					std::cerr << "Erro jogador" << std::endl;
				}

				pFigura->setPosition(sf::Vector2f(static_cast<float>(this->x), static_cast<float>(this->y)));
				atualizarBarraVida();
			}


			Jogador::Jogador(int numJogador, float xi, float yi) :
				Personagem(xi, yi),
				pontos(0),
				cooldown(0),
				playerNum(numJogador),
				ultimaDirecaoX(1.0f),
				stun(sf::Time::Zero),
				obstaculoCooldown(sf::Time::Zero)
			{
				this->n_vidas = 50;
				this->n_vidas_max = 50;
				atualizarBarraVida();
			}


			Jogador::~Jogador()
			{

			}


			void Jogador::resetar(float posX, float posY)
			{
				this->x = posX;
				this->y = posY;
				setPosicaoGrafica(x, y);

				this->n_vidas = n_vidas_max;
				this->vel_grav = 0.0f;
				this->estaAtivo = true;
				this->stun = sf::Time::Zero;
				this->obstaculoCooldown = sf::Time::Zero;
				this->pontos = 0;

				atualizarBarraVida();
			}


			void Jogador::mover()
			{
				this->movimento = sf::Vector2f(0.0f, 0.0f);
				float dt = Ente::g_dt;

				if (obstaculoCooldown > sf::Time::Zero)
				{
					obstaculoCooldown -= sf::seconds(dt);
					if (obstaculoCooldown < sf::Time::Zero)
					{
						obstaculoCooldown = sf::Time::Zero;
					}
				}

				if (stun > sf::Time::Zero)
				{
					stun -= sf::seconds(dt);

					if (stun < sf::Time::Zero)
					{
						stun = sf::Time::Zero;
					}

					movimento = sf::Vector2f(0.0f, 0.0f);
					this->x += movimento.x * dt * velocidade;
					this->y += movimento.y * dt * velocidade;
					setPosicaoGrafica(this->x, this->y);
					return;
				}


				if (playerNum == 1)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
					{
						movimento += sf::Vector2f(0.0f, -1.0f);
					}

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
					{
						movimento += sf::Vector2f(1.0f, 0.0f);
						ultimaDirecaoX = 1.0f;
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
					{
						movimento += sf::Vector2f(-1.0f, 0.0f);
						ultimaDirecaoX = -1.0f;
					}
				}
				else if (playerNum == 2)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					{
						movimento += sf::Vector2f(0.0f, -1.0f);
					}

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
					{
						movimento += sf::Vector2f(1.0f, 0.0f);
						ultimaDirecaoX = 1.0f;
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					{
						movimento += sf::Vector2f(-1.0f, 0.0f);
						ultimaDirecaoX = -1.0f;
					}
				}

				Personagem::gravitar(&movimento);

				this->x += movimento.x * dt * velocidade;
				this->y += movimento.y * dt * velocidade;

				setPosicaoGrafica(this->x, this->y);
			}

			void Jogador::executar()
			{
				if (this->getAtivo())
				{
					this->mover();
					this->lancar();
					atualizarBarraVida();

					if (this->y > ALTURA_TELA + 200)//evitar que caia pra sempre
					{
						this->setAtivo(false);
					}
				}
				projeteis.executar();
			}


			void Jogador::salvarDataBuffer(json& arq)
			{
				Personagem::salvarDataBuffer(arq);
				arq["tipo"] = "Jogador";
				arq["pontos"] = this->pontos;
				arq["playerNum"] = this->playerNum;
				arq["ultimaDirecaoX"] = this->ultimaDirecaoX;
			}

			void Jogador::carregarDeBuffer(const json& data)
			{
				Personagem::carregarDeBuffer(data);
				this->pontos = data.value("pontos", this->pontos);
				this->playerNum = data.value("playerNum", this->playerNum);
				this->ultimaDirecaoX = data.value("ultimaDirecaoX", this->ultimaDirecaoX);

				this->stun = sf::Time::Zero;
				this->obstaculoCooldown = sf::Time::Zero;
				this->cooldown = 0;
				std::string caminhoTextura = (playerNum == 1) ? "assets/jogador1.png" : "assets/jogador2.png";
				if (pGG)
				{
					sf::Texture* tex = pGG->getTextura(caminhoTextura);
					if (tex)
					{
						pFigura->setTexture(*tex);
						pFigura->setScale(32.0f / tex->getSize().x, 32.0f / tex->getSize().y);
					}
				}
			}

			void Jogador::lancar() {
				if (cooldown > 0)
				{
					cooldown--;
				}
					

				if (((playerNum == 1 && sf::Mouse::isButtonPressed(sf::Mouse::Left)) || playerNum == 2 && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))&& cooldown <= 0) {
					Projetil* novoProjetil = new Projetil();
					if (pGG)
					{
						novoProjetil->setGerenciadorGrafico(pGG);
					}

					float spawnX;

					if (ultimaDirecaoX > 0)
					{
						spawnX = this->x + pFigura->getGlobalBounds().width;
					}
					else
					{
						spawnX = this->x - 5.0f;
					}

					novoProjetil->setX(spawnX);
					novoProjetil->setY(y + 7.5f);
					novoProjetil->setVelocidade(PROJETIL_VELOCIDADE_JOGADOR * ultimaDirecaoX, 1.0f);
					novoProjetil->setIdDono(this->playerNum);
					projeteis.inserir(novoProjetil);
					cooldown = 60;
				}
			}


			ListaEntidades* Jogador::getProjeteis()
			{
				return &projeteis;
			}


			void Jogador::trava_mov(float t)
			{	
				stun = sf::seconds(t);;
			}

			void Jogador::iniciarObstaculoCooldown(sf::Time duracao)
			{
				obstaculoCooldown = duracao;
			}

			sf::Time Jogador::getObstaculoCooldown() const
			{
				return obstaculoCooldown;
			}

			void Jogador::adicionarPontos(int p)
			{
				if (p > 0) 
				{
					this->pontos += p;
				}
			}

			int Jogador::getPontos() const
			{
				return this->pontos;
			}
		}
	}
}
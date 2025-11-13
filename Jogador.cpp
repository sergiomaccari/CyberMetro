#include "Jogador.h"
#include "Projetil.h"
#include "Fase.h" 
#include "Gerenciador_Grafico.h"

using namespace Entidades;
namespace Personagens
{
	const float Jogador::PROJETIL_VELOCIDADE_JOGADOR = 3.0f;

	Jogador::Jogador(int numJogador) :
		Personagem(),
		pontos(0),
		cooldown(0),
		playerNum(numJogador),
		ultimaDirecaoX(1.0f),
		stun(sf::Time::Zero),
		obstaculoCooldown(sf::Time::Zero),
		pFaseAtual(nullptr)
	{
		this->x = 32;
		this->y = 32;
		this->velocidade = 300.0f;
		this->n_vidas = 1000; //ver esses valores, tava usando como teste
		this->n_vidas_max = 1000;

		std::string caminhoTextura = (playerNum == 1) ? "Imagens/jogador1.png" : "Imagens/jogador2.png";

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
		obstaculoCooldown(sf::Time::Zero),
		pFaseAtual(nullptr)
	{
		this->n_vidas = 1000;
		this->n_vidas_max = 1000;
		atualizarBarraVida();
	}


	Jogador::~Jogador()
	{
		movimento = sf::Vector2f(0.0f, 0.0f);
	}


	//ATENCION POOKIE, ESSA FUNCAO QUE EU FIZ EH MT IMPORTANTE. RESETA O JOGADOR CONFORME TU FINALIZA UMA FASE OU QUITA, EVITA OS BUGS DE RESPAWN QUE EU TINHA DITO
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
		movimento = sf::Vector2f(0.0f, 0.0f);
		tempo = clock.restart();

		if (obstaculoCooldown > sf::Time::Zero)
		{
			obstaculoCooldown -= tempo;
			if (obstaculoCooldown < sf::Time::Zero)
			{
				obstaculoCooldown = sf::Time::Zero;
			}
		}

		if (stun > sf::Time::Zero)
		{
			stun -= tempo;

			if (stun < sf::Time::Zero)
			{
				stun = sf::Time::Zero;
			}

			movimento = { 0.f, 0.f };
			this->x += movimento.x * tempo.asSeconds() * velocidade;
			this->y += movimento.y * tempo.asSeconds() * velocidade;
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

		this->x += movimento.x * tempo.asSeconds() * velocidade;
		this->y += movimento.y * tempo.asSeconds() * velocidade;

		setPosicaoGrafica(this->x, this->y);
	}

	void Jogador::executar()
	{
		if (this->getAtivo())
		{
			this->mover();
			this->atirar();
			atualizarBarraVida();

			// ATENCION POOKIE, ESSA PARTE EVITA BUGS DE SPAWN QUANDO O JOGADOR FICAVA CAINDO ENTRE FASES. TALVEZ SEJA MASSA FAZER 
			if (this->y > MUNDO_Y_MAX + 200)
			{
				this->setAtivo(false);
			}
		}
		projeteis.executar();
	}

	void Jogador::salvar()
	{

	}

	void Jogador::atirar() {
		if (cooldown > 0)
			cooldown--;

		bool tiroPressionado = false;
		if (playerNum == 1 && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			tiroPressionado = true;
		else if (playerNum == 2 && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			tiroPressionado = true;


		if (tiroPressionado && cooldown == 0) {
			Projetil* novoProjetil = new Projetil();
			if (pGG)
			{
				novoProjetil->setGerenciadorGrafico(pGG);
			}

			float spawnX;
			float playerWidth = pFigura->getGlobalBounds().width;
			float projetilWidth = 5.0f;

			if (ultimaDirecaoX > 0)
			{
				spawnX = x + playerWidth;
			}
			else
			{
				spawnX = x - projetilWidth;
			}

			novoProjetil->setX(spawnX);
			novoProjetil->setY(y + 7.5f);

			novoProjetil->setVelocidade(PROJETIL_VELOCIDADE_JOGADOR * ultimaDirecaoX, 0.0f);

			novoProjetil->setIdDono(this->playerNum);

			projeteis.inserir(novoProjetil);
			cooldown = 120;
		}
	}


	ListaEntidades* Jogador::getProjeteis()
	{
		return &projeteis;
	}


	void Jogador::trava_mov(float t)
	{
		const sf::Time novo_st = sf::seconds(t);
		if (stun < novo_st)
		{
			stun = novo_st;
		}
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
		if (p > 0) {
			this->pontos += p;
		}
	}

	int Jogador::getPontos() const
	{
		return this->pontos;
	}
}
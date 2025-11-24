#include "Gerenciador_Grafico.h"
#include "Jogador.h" 
#include "Personagem.h" 
#include "Ente.h" 


namespace CyberMetro {
	namespace Gerenciadores {

		using namespace CyberMetro::Entidades::Personagens;
		using namespace CyberMetro::Entidades;

		void Gerenciador_Grafico::iniciaJanela()
		{
			this->videoMode.height = ALTURA_TELA;
			this->videoMode.width = LARGURA_TELA;

			this->janela = new sf::RenderWindow(this->videoMode, "Meu jogo", sf::Style::Titlebar | sf::Style::Close);
			janela->setFramerateLimit(150);
			camera = janela->getDefaultView();
			camera.zoom(1);
		}

		void Gerenciador_Grafico::setCamera(CyberMetro::Entidades::Personagens::Jogador* pJog)
		{
			if (pJog && janela)
			{
				camera.setCenter(pJog->getX(), ALTURA_TELA / 2);
				janela->setView(camera);
			}
		}

		Gerenciador_Grafico::Gerenciador_Grafico()
		{
			this->janela = NULL;
			this->iniciaJanela();
			if (!texturaBackground.loadFromFile("assets/fundo_fixo.png"))
			{
				std::cerr << "Erro: nao carregou fundo" << std::endl;
			}
			spriteBackground.setTexture(texturaBackground);
			spriteBackground.setScale
			(
				LARGURA_TELA / spriteBackground.getLocalBounds().width,
				ALTURA_TELA / spriteBackground.getLocalBounds().height
			);
		}
		Gerenciador_Grafico::~Gerenciador_Grafico()
		{
			delete this->janela;
		}

		sf::Texture* Gerenciador_Grafico::getTextura(std::string caminho)
		{
			std::unordered_map<std::string, sf::Texture>::iterator it = texturas.find(caminho);//mapa, conceito utilizado
			if (it != texturas.end())
			{
				return &(it->second);//já tinha carregado essa textura
			}

			sf::Texture novaTextura;
			if (!novaTextura.loadFromFile(caminho))
			{
				std::cerr << "Erro nao carregou textura" << caminho << std::endl;
				return nullptr;
			}

			texturas[caminho] = novaTextura;
			return &texturas[caminho];
		}

		void Gerenciador_Grafico::desenharBackground()//coloca imagem de fundo
		{
			if (this->janela)
			{
				this->janela->draw(spriteBackground);
			}
		}


		void Gerenciador_Grafico::desenharEnte(const Ente* pe)//função responsavel por desenhar ente na tela
		{
			if (this->janela && pe && pe->getFigura())
			{
				this->janela->draw(*(pe->getFigura()));
				const Personagem* pPersonagem = dynamic_cast<const Personagem*>(pe);//cast para verificar que é personagem
				if (pPersonagem && pPersonagem->getAtivo())//se for personagem desenha a barra de vida
				{
					this->janela->draw(pPersonagem->getBarraFundo());
					this->janela->draw(pPersonagem->getBarraAtual());
				}
			}
		}


		void Gerenciador_Grafico::update()
		{
			if (!this->janela) {
				std::cerr << "erro 2" << std::endl;
				return;
			}
			while (this->janela->pollEvent(this->ev))
			{
				switch (this->ev.type)
				{
				case sf::Event::Closed:
				{
					this->janela->close();
				}
					break;


				case sf::Event::KeyPressed:
					if (this->ev.key.code == sf::Keyboard::Escape)
					{
						this->janela->close();
					}
					break;
				}
			}
		}
		void Gerenciador_Grafico::render()
		{
			if (this->janela)
			{
				this->janela->display();
			}
		}

		void Gerenciador_Grafico::clear()
		{
			if (!this->janela) 
			{
				std::cerr << "erro 3" << std::endl;//testes
				return;
			}
			this->janela->clear();
		}


		const bool Gerenciador_Grafico::getJanelaOpen() const
		{
			if (!this->janela) {
				std::cerr << "erro 1" << std::endl;//testes
				return false;
			}
			return this->janela->isOpen();
		}

		sf::RenderWindow* Gerenciador_Grafico::getJanela() const
		{
			return janela;
		}
	}
}
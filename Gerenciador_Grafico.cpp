#include "Gerenciador_Grafico.h"
#include "Jogador.h" // Ja inclui Personagem.h
// #include "Personagem.h" // Adicionado para a logica da barra de vida

using namespace Personagens;

void Gerenciador_Grafico::iniciaJanela()
{
	this->videoMode.height = ALTURA_TELA;
	this->videoMode.width = LARGURA_TELA;

	this->janela = new sf::RenderWindow(this->videoMode, "Meu jogo", sf::Style::Titlebar | sf::Style::Close);
	//janela->setFramerateLimit(150);
	camera = janela->getDefaultView();
	camera.zoom(1);
}

void Gerenciador_Grafico::setCamera(Jogador* pJog)
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
	if (!texturaBackground.loadFromFile("Imagens/fundo_fixo.png"))
	{
		std::cerr << "Erro: nao carregou fundo" << std::endl;
	}
	spriteBackground.setTexture(texturaBackground);
	spriteBackground.setScale(
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
	std::unordered_map<std::string, sf::Texture>::iterator it = texturas.find(caminho);
	if (it != texturas.end())
	{
		return &(it->second);
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

void Gerenciador_Grafico::desenharBackground()
{
	if (this->janela)
	{
		this->janela->draw(spriteBackground);
	}
}


void Gerenciador_Grafico::desenharEnte(const Ente* pe)
{
	if (this->janela && pe && pe->getFigura())
	{
		this->janela->draw(*(pe->getFigura()));

		const Personagens::Personagem* pPersonagem = dynamic_cast<const Personagens::Personagem*>(pe);

		if (pPersonagem && pPersonagem->getAtivo())
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
			this->janela->close();
			break;


		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->janela->close();
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
	if (!this->janela) {
		std::cerr << "erro 3" << std::endl;
		return;
	}
	this->janela->clear();
}


const bool Gerenciador_Grafico::getJanelaOpen() const
{
	if (!this->janela) {
		std::cerr << "erro 1" << std::endl;
		return false;
	}
	return this->janela->isOpen();
}

sf::RenderWindow* Gerenciador_Grafico::getJanela() const
{
	return janela;
}
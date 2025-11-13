#include "Menu.h"
#include "Gerenciador_Grafico.h"
#include <iostream>

Menu::Menu()
    : indiceOpcaoSelecionada(0),
    estadoAtualInterno(EstadoJogo::Menu),
    nivelAtual(MenuNivel::PRINCIPAL),
    pOpcoesAtuais(nullptr)
{
}

Menu::~Menu() {}

void Menu::inicializar()
{
    if (!pGG) {
        std::cerr << "Gerenciador Grafico (pGG) erro" << std::endl;
        return;
    }
    carregarFonte();
    configurarOpcoes();
    setNivelMenu(MenuNivel::PRINCIPAL);
}

void Menu::carregarFonte()
{
    if (!fonte.loadFromFile("Pixeboy.ttf"))
    {
        std::cerr << "Nao carregou a fonte" << std::endl;
    }
}

void Menu::posicionarTextos(std::vector<sf::Text>& textos)
{
    for (size_t i = 0; i < textos.size(); ++i)
    {
        sf::FloatRect bounds = textos[i].getLocalBounds();
        textos[i].setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        textos[i].setPosition(LARGURA_TELA / 2.0f, 400.0f + i * 80.0f);
    }
}

void Menu::configurarOpcoes()
{
    opcoesPrincipal.clear();
    opcoesFase1.clear();
    opcoesFase2.clear();

    std::vector<std::string> textosPrincipal = { "Fase 1", "Fase 2", "Sair" };
    for (const std::string& str : textosPrincipal) {
        sf::Text texto(str, fonte, TAMANHO_FONTE);
        texto.setFillColor(COR_NORMAL);
        opcoesPrincipal.push_back(texto);
    }
    posicionarTextos(opcoesPrincipal);
    std::vector<std::string> textosSubmenu = { "Novo Jogo", "Carregar Jogo", "Ranking", "Voltar" };

    for (const std::string& str : textosSubmenu) {
        sf::Text texto(str, fonte, TAMANHO_FONTE);
        texto.setFillColor(COR_NORMAL);
        opcoesFase1.push_back(texto);
    }
    posicionarTextos(opcoesFase1);

    for (const std::string& str : textosSubmenu) {
        sf::Text texto(str, fonte, TAMANHO_FONTE);
        texto.setFillColor(COR_NORMAL);
        opcoesFase2.push_back(texto);
    }
    posicionarTextos(opcoesFase2);

    pOpcoesAtuais = &opcoesPrincipal;
    atualizarDestaque();
}

void Menu::setNivelMenu(MenuNivel novoNivel, int novoIndice)
{
    nivelAtual = novoNivel;
    indiceOpcaoSelecionada = novoIndice;

    if (nivelAtual == MenuNivel::PRINCIPAL) {
        pOpcoesAtuais = &opcoesPrincipal;
    }
    else if (nivelAtual == MenuNivel::FASE1) {
        pOpcoesAtuais = &opcoesFase1;
    }
    else if (nivelAtual == MenuNivel::FASE2) {
        pOpcoesAtuais = &opcoesFase2;
    }
    atualizarDestaque();
}

void Menu::atualizarDestaque()
{
    if (!pOpcoesAtuais) return;
    for (size_t i = 0; i < pOpcoesAtuais->size(); ++i) {
        (*pOpcoesAtuais)[i].setFillColor((i == indiceOpcaoSelecionada) ? COR_SELECIONADA : COR_NORMAL);
    }
}


void Menu::moverCima()
{
    if (!pOpcoesAtuais || pOpcoesAtuais->empty()) return;

    indiceOpcaoSelecionada = (int)((indiceOpcaoSelecionada - 1 + pOpcoesAtuais->size()) % pOpcoesAtuais->size());

    atualizarDestaque();
}

void Menu::moverBaixo()
{
    if (!pOpcoesAtuais || pOpcoesAtuais->empty()) return;

    indiceOpcaoSelecionada = (int)((indiceOpcaoSelecionada + 1) % pOpcoesAtuais->size());

    atualizarDestaque();
}

void Menu::executar()
{
    //logica eh controlada pelo loop principal em jogo.cpp
}


void Menu::processarEntrada(sf::Event& evento)
{
    estadoAtualInterno = EstadoJogo::Menu;

    if (evento.type == sf::Event::Closed)
    {
        estadoAtualInterno = EstadoJogo::Sair;
        return;
    }

    if (evento.type == sf::Event::KeyPressed)
    {
        if (evento.key.code == sf::Keyboard::Up) {
            moverCima();
        }
        else if (evento.key.code == sf::Keyboard::Down) {
            moverBaixo();
        }
        else if (evento.key.code == sf::Keyboard::Escape) {
            if (nivelAtual != MenuNivel::PRINCIPAL) {
                setNivelMenu(MenuNivel::PRINCIPAL, (nivelAtual == MenuNivel::FASE1) ? 0 : 1);
            }
        }
    }

    if (evento.type == sf::Event::KeyReleased && evento.key.code == sf::Keyboard::Enter)
    {
        if (!pOpcoesAtuais) return;

        std::string selecionado = (*pOpcoesAtuais)[indiceOpcaoSelecionada].getString();

        if (nivelAtual == MenuNivel::PRINCIPAL) {
            if (selecionado == "Fase 1") {
                setNivelMenu(MenuNivel::FASE1);
            }
            else if (selecionado == "Fase 2") {
                setNivelMenu(MenuNivel::FASE2);
            }
            else if (selecionado == "Sair") {
                estadoAtualInterno = EstadoJogo::Sair;
            }
        }
        else if (nivelAtual == MenuNivel::FASE1) {
            if (selecionado == "Novo Jogo") {
                estadoAtualInterno = EstadoJogo::Jogando;
            }
            else if (selecionado == "Carregar Jogo") {
                std::cout << "teste Carregar Jogo Fase 1" << std::endl;
            }
            else if (selecionado == "Ranking") {
                estadoAtualInterno = EstadoJogo::MostrandoRanking;
            }
            else if (selecionado == "Voltar") {
                setNivelMenu(MenuNivel::PRINCIPAL, 0);
            }
        }
        else if (nivelAtual == MenuNivel::FASE2) {
            if (selecionado == "Novo Jogo") {
                estadoAtualInterno = EstadoJogo::FaseDois;
            }
            else if (selecionado == "Carregar Jogo") {
                std::cout << "teste Carregar Jogo Fase 2" << std::endl;
            }
            else if (selecionado == "Ranking") {
                estadoAtualInterno = EstadoJogo::MostrandoRanking;
            }
            else if (selecionado == "Voltar") {
                setNivelMenu(MenuNivel::PRINCIPAL, 1);
            }
        }

        if (estadoAtualInterno != EstadoJogo::Menu)
        {
            // Nao reseta o menu aqui jogo que faz isso
        }
    }
}


void Menu::desenharOpcoes()
{
    if (!pGG || !pGG->getJanela()) return;
    pGG->getJanela()->setView(pGG->getJanela()->getDefaultView());

    pGG->desenharBackground();

    sf::Text titulo("Meu Jogo SFML", fonte, 80);
    titulo.setFillColor(sf::Color::Cyan);
    sf::FloatRect boundsT = titulo.getLocalBounds();
    titulo.setOrigin(boundsT.left + boundsT.width / 2.0f, boundsT.top + boundsT.height / 2.0f);
    titulo.setPosition(LARGURA_TELA / 2.0f, 200.0f);
    pGG->getJanela()->draw(titulo);

    sf::Text subTitulo("", fonte, 20);
    subTitulo.setFillColor(sf::Color::Cyan);
    if (nivelAtual == MenuNivel::FASE1) {
        subTitulo.setString("Fase 1");
    }
    else if (nivelAtual == MenuNivel::FASE2) {
        subTitulo.setString("Fase 2");
    }

    if (nivelAtual != MenuNivel::PRINCIPAL) {
        sf::FloatRect boundsST = subTitulo.getLocalBounds();
        subTitulo.setOrigin(boundsST.left + boundsST.width / 2.0f, boundsST.top + boundsST.height / 2.0f);
        subTitulo.setPosition(LARGURA_TELA / 2.0f, 350.0f);
        pGG->getJanela()->draw(subTitulo);
    }

    if (!pOpcoesAtuais) return;
    for (const sf::Text& opcao : *pOpcoesAtuais)
    {
        pGG->getJanela()->draw(opcao);
    }
}

EstadoJogo Menu::getProximoEstado() const
{
    return estadoAtualInterno;
}

void Menu::resetarEstadoInterno()
{
    estadoAtualInterno = EstadoJogo::Menu;
    setNivelMenu(MenuNivel::PRINCIPAL, 0);
}

Menu::MenuNivel Menu::getNivelAtual() const
{
    return nivelAtual;
}
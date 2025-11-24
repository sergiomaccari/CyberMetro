#include "Menu.h"
#include "Gerenciador_Grafico.h"
#include <iostream>
namespace CyberMetro {

    Menu::Menu()
        : indiceOpcaoSelecionada(0),
        estadoAtualInterno(EstadoJogo::Menu),
        nivelAtual(MenuNivel::PRINCIPAL),
        pOpcoesAtuais(nullptr)
    {

    }

    Menu::~Menu()
    {
    
    }

    void Menu::inicializar()//define gerenciador grafico e define o estado interno inicial
    {
        if (!pGG) {
            std::cerr << "Gerenciador Grafico (pGG) erro" << std::endl;
            return;
        }
        carregarFonte();
        configurarOpcoes();
        setNivelMenu(MenuNivel::PRINCIPAL);
    }

    void Menu::carregarFonte()//carrega a fonte
    {
        if (!fonte.loadFromFile("PIxeboy.ttf"))
        {
            std::cerr << "Nao carregou a fonte" << std::endl;
        }
    }

    void Menu::posicionarTextos(std::vector<sf::Text>& textos)//define as posições do texto
    {
        for (size_t i = 0; i < textos.size(); ++i)
        {
            sf::FloatRect bounds = textos[i].getLocalBounds();
            textos[i].setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);//coloca na origem de verdade do texto
            textos[i].setPosition(LARGURA_TELA / 2.0f, 400.0f + i * 80.0f);//seta a posicao do texto para aparecer no ponto do centro
        }
    }

    void Menu::configurarOpcoes()
    {
        //limpa por boas praticas
        opcoesPrincipal.clear();
        opcoesFase1.clear();
        opcoesFase2.clear();


        //a se guir é posicionado as opões de menu ne submenus corretamente

        std::vector<std::string> textosPrincipal = { "Fase 1", "Fase 2", "Carregar Jogo", "Sair" };//opções de submeno nesse vetor auxilair
        for (size_t i = 0; i < textosPrincipal.size(); ++i)//seta o lugar do texto corretamente
        {
            sf::Text texto(textosPrincipal[i], fonte, TAMANHO_FONTE);
            texto.setFillColor(COR_NORMAL);
            opcoesPrincipal.push_back(texto);
        }
        posicionarTextos(opcoesPrincipal);

        std::vector<std::string> textosSubmenu = { "Novo Jogo", "Ranking", "Voltar" };

        for (size_t i = 0; i < textosSubmenu.size(); ++i)
        {//seta a posição dos textos do submenu
            sf::Text texto(textosSubmenu[i], fonte, TAMANHO_FONTE);
            texto.setFillColor(COR_NORMAL);
            opcoesFase1.push_back(texto);
        }
        posicionarTextos(opcoesFase1);

        for (size_t i = 0; i < textosSubmenu.size(); ++i)
        {
            sf::Text texto(textosSubmenu[i], fonte, TAMANHO_FONTE);
            texto.setFillColor(COR_NORMAL);
            opcoesFase2.push_back(texto);
        }
        posicionarTextos(opcoesFase2);

        pOpcoesAtuais = &opcoesPrincipal;
        atualizarDestaque();
    }

    void Menu::setNivelMenu(MenuNivel novoNivel, int novoIndice)//contrrola a navegaçao do menu
    {
        nivelAtual = novoNivel;
        indiceOpcaoSelecionada = novoIndice;

        if (nivelAtual == MenuNivel::PRINCIPAL) 
        {
            pOpcoesAtuais = &opcoesPrincipal;
        }
        else if (nivelAtual == MenuNivel::FASE1) 
        {
            pOpcoesAtuais = &opcoesFase1;
        }
        else if (nivelAtual == MenuNivel::FASE2) 
        {
            pOpcoesAtuais = &opcoesFase2;
        }
        atualizarDestaque();
    }

    void Menu::atualizarDestaque()//muda corzinha de onde o usuario esta podendo selecioanr
    {
        if (!pOpcoesAtuais) return;
        for (size_t i = 0; i < pOpcoesAtuais->size(); ++i) 
        {
            (*pOpcoesAtuais)[i].setFillColor((i == indiceOpcaoSelecionada) ? COR_SELECIONADA : COR_NORMAL);//amarelo para selecionado e branco para normal
        }
    }


    void Menu::moverCima() //move o seletor do menu pra cima
    {
        if (!pOpcoesAtuais || pOpcoesAtuais->empty())
        {
            return;
        }
        indiceOpcaoSelecionada = (int)((indiceOpcaoSelecionada - 1 + pOpcoesAtuais->size()) % pOpcoesAtuais->size());//operador % para ciclar

        atualizarDestaque();
    }

    void Menu::moverBaixo()//move o seletor de menu pra baixo
    {
        if (!pOpcoesAtuais || pOpcoesAtuais->empty())
        {
			return;
        }

        indiceOpcaoSelecionada = (int)((indiceOpcaoSelecionada + 1) % pOpcoesAtuais->size());//operador % para ciclar

        atualizarDestaque();
    }

    void Menu::executar()
    {
        //logica eh controlada pelo loop principal em jogo.cpp
    }


    void Menu::processarEntrada(sf::Event& evento)//controlador de eventos. Fica processando input do usuario e definbindo as ações necessarias
    {
        estadoAtualInterno = EstadoJogo::Menu;

        if (evento.type == sf::Event::Closed)
        {
            estadoAtualInterno = EstadoJogo::Sair;
            return;
        }

        if (evento.type == sf::Event::KeyPressed)
        {
            if (evento.key.code == sf::Keyboard::Up) 
            {
                moverCima();
            }
            else if (evento.key.code == sf::Keyboard::Down) 
            {
                moverBaixo();
            }
            else if (evento.key.code == sf::Keyboard::Escape) 
            {
                if (nivelAtual != MenuNivel::PRINCIPAL) 
                {
                    setNivelMenu(MenuNivel::PRINCIPAL, (nivelAtual == MenuNivel::FASE1) ? 0 : 1);
                }
            }
        }

        if (evento.type == sf::Event::KeyReleased && evento.key.code == sf::Keyboard::Enter)
        {
            if (!pOpcoesAtuais)
            {
                return;
            }
            
            std::string selecionado = (*pOpcoesAtuais)[indiceOpcaoSelecionada].getString();

            if (nivelAtual == MenuNivel::PRINCIPAL) 
            {
                if (selecionado == "Fase 1")
                {
                    setNivelMenu(MenuNivel::FASE1);
                }
                else if (selecionado == "Fase 2") 
                {
                    setNivelMenu(MenuNivel::FASE2);
                }
                else if (selecionado == "Carregar Jogo") 
                {
                    estadoAtualInterno = EstadoJogo::CarregandoJogo;
                }
                else if (selecionado == "Sair") {
                    estadoAtualInterno = EstadoJogo::Sair;
                }
            }
            else if (nivelAtual == MenuNivel::FASE1) 
            {
                if (selecionado == "Novo Jogo") 
                {
                    estadoAtualInterno = EstadoJogo::Jogando;
                }
                else if (selecionado == "Ranking") 
                {
                    estadoAtualInterno = EstadoJogo::MostrandoRanking;
                }
                else if (selecionado == "Voltar") 
                {
                    setNivelMenu(MenuNivel::PRINCIPAL, 0);
                }
            }
            else if (nivelAtual == MenuNivel::FASE2) 
            {
                if (selecionado == "Novo Jogo") 
                {
                    estadoAtualInterno = EstadoJogo::FaseDois;
                }
                else if (selecionado == "Ranking") 
                {
                    estadoAtualInterno = EstadoJogo::MostrandoRanking;
                }
                else if (selecionado == "Voltar") 
                {
                    setNivelMenu(MenuNivel::PRINCIPAL, 1);
                }
            }

            if (estadoAtualInterno != EstadoJogo::Menu)
            {

            }
        }
    }


    void Menu::desenharOpcoes()//desenha coisas do menu
    {
        if (!pGG || !pGG->getJanela())
        {
			return;
        }
        
        pGG->getJanela()->setView(pGG->getJanela()->getDefaultView());

        pGG->desenharBackground();

        sf::Text titulo("CYBERMETRO", fonte, 120);
        titulo.setFillColor(sf::Color::Cyan);
        sf::FloatRect boundsT = titulo.getLocalBounds();
        titulo.setOrigin(boundsT.left + boundsT.width / 2.0f, boundsT.top + boundsT.height / 2.0f);
        titulo.setPosition(LARGURA_TELA / 2.0f, 200.0f);
        pGG->getJanela()->draw(titulo);

        sf::Text subTitulo("", fonte, 50);
        subTitulo.setFillColor(sf::Color::Cyan);


        if (nivelAtual == MenuNivel::FASE1) 
        {
            subTitulo.setString("Fase 1");
        }
        else if (nivelAtual == MenuNivel::FASE2) 
        {
            subTitulo.setString("Fase 2");
        }

        if (nivelAtual != MenuNivel::PRINCIPAL) 
        {
            sf::FloatRect boundsST = subTitulo.getLocalBounds();
            subTitulo.setOrigin(boundsST.left + boundsST.width / 2.0f, boundsST.top + boundsST.height / 2.0f);
            subTitulo.setPosition(LARGURA_TELA / 2.0f, 300.0f);
            pGG->getJanela()->draw(subTitulo);
        }

        if (!pOpcoesAtuais) return;
        for (const sf::Text& opcao : *pOpcoesAtuais)
        {
            pGG->getJanela()->draw(opcao);
        }
    }

    EstadoJogo Menu::getProximoEstado() const//retorna proximo estado
    {
        return estadoAtualInterno;
    }

    void Menu::resetarEstadoInterno()//reseta o estado para o principal que é o inicial
    {
        estadoAtualInterno = EstadoJogo::Menu;
        setNivelMenu(MenuNivel::PRINCIPAL, 0);
    }

    Menu::MenuNivel Menu::getNivelAtual() const
    {
        return nivelAtual;
    }
}
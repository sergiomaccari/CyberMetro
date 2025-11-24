#include "Jogo.h"
#include "FasePrimeira.h" 
#include "FaseSegunda.h"  
#include "Gerenciador_Grafico.h"
#include <string> 
#include <nlohmann/json.hpp> 
#include <fstream> //leitura e escrita de arquivos          
#include <algorithm> // swap      
#include <iomanip> // std::setw. setw(4) [e definido largura de 4 espa;os para identaçao
#include <vector>//vectors
#include <iostream>//o de sempre
#include <filesystem>//organização dos arquivos em pastas, serve para importar arquivos
#include "Robo_Junior.h"
#include "Robo_Senior.h"
#include "Robo_CEO.h"
#include "Plataforma.h"
#include "Gelinho.h"
#include "Choquinho.h"
#include "Projetil.h" 

using namespace CyberMetro::Entidades;
using namespace CyberMetro::Entidades::Personagens;
using namespace CyberMetro::Entidades::Obstaculos;
using json = nlohmann::json;


namespace CyberMetro {
    Jogo::Jogo()
        : gg(),
        menu(),
        pJogador1(nullptr),
        pJogador2(nullptr),
        faseAtual(nullptr),
        estadoAtual(EstadoJogo::Menu),
        estadoAnterior(EstadoJogo::Menu),
        indicePausa(0),
        proximaFaseAposNome(EstadoJogo::Menu),
        nomeSaveFile(""),
        cursorVisivel(true)
    {
        Ente::setGerenciadorGrafico(&gg);

        menu.inicializar();
        carregarRecursosPausa();

        carregarRecursosUI();

        pJogador1 = new Personagens::Jogador(1);
        pJogador2 = new Personagens::Jogador(2);

        pJogador2->setAtivo(false);//o segundo jogador começa como inativo e só muda quando aperta espaço
    }

    Jogo::~Jogo()
    {//precisa limapr e deletar jogadores, o resto outras destrutoras fazem e ta de boa
        limparFaseAtual();
        if (pJogador1) {
            delete pJogador1;
            pJogador1 = nullptr;
        }
        if (pJogador2) {
            delete pJogador2;
            pJogador2 = nullptr;
        }

        std::cout << "Jogo encerrado" << std::endl;//texto importante pros testes que foram realizados

    }

    void Jogo::carregarRecursosPausa()//faz a interface de pause
    {
        if (!fontePausa.loadFromFile("assets/Pixeboy.ttf"))
        {
            std::cerr << "Erro ao carregar fonte assets/Pixeboy.ttf para pausa" << std::endl;
        }

        fundoPausa.setSize(sf::Vector2f(LARGURA_TELA, ALTURA_TELA));
        fundoPausa.setFillColor(sf::Color(0, 0, 0, 150));

        std::vector<std::string> textos = { "Continuar", "Salvar Jogo", "Voltar ao Menu" };//vetor auxiliar no qual os textos serão empurrados no vetor atributo opcoesPausa
        for (size_t i = 0; i < textos.size(); ++i)//faz o tamanho do vetor auxiliar
        {
            sf::Text texto(textos[i], fontePausa, 50);
            sf::FloatRect bounds = texto.getLocalBounds();
            texto.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);// coloca a origem no centro de verdade
            texto.setPosition(LARGURA_TELA / 2.0f, 350.0f + i * 100.0f);// posiciona no centro calculado acima
            opcoesPausa.push_back(texto); //coloca as opções centralizadas e coloridas no vetor de pausa
        }
        atualizarDestaquePausa(); //parte selecionada fica amarela
    }

    void Jogo::atualizarDestaquePausa()
    {
        for (size_t i = 0; i < opcoesPausa.size(); ++i)
        {
            opcoesPausa[i].setFillColor((i == indicePausa) ? sf::Color::Yellow : sf::Color::White);//deixa a opcao amarelada, atualizando a cor do texto
        }
    }

    void Jogo::processarInputPausa(sf::Event& evento)//le o que tu ta selecionando no menu
    {
        if (evento.type == sf::Event::KeyPressed)
        {
            if (evento.key.code == sf::Keyboard::P) {//pausa se aperta p
                estadoAtual = estadoAnterior;
                return;
            }
            else if (evento.key.code == sf::Keyboard::Up) {//sobe opcao se tecla pra cima
                indicePausa = (indicePausa - 1 + opcoesPausa.size()) % opcoesPausa.size();//operador % pra ciclar
                atualizarDestaquePausa();//amarela
            }
            else if (evento.key.code == sf::Keyboard::Down)
            { //desce opcao se setinha baio
                indicePausa = (indicePausa + 1) % opcoesPausa.size();//operador % pra ciclar
                atualizarDestaquePausa();//amarelo
            }
        }
        else if (evento.type == sf::Event::KeyReleased)//tipo no sistema academico do professor simao
        {
            if (evento.key.code == sf::Keyboard::Enter) {
                switch (indicePausa)//indice pausa definido no if acima
                {//cases para opções
                case 0:
                    estadoAtual = estadoAnterior;//resume o jogo, volta pro estado que tava
                    break;

                case 1:
                    salvarJogo(nomeSaveFile + ".save.json"); // chama salvar e faz o file
                    break;

                case 2:
                    voltarAoMenu(); //volta pro menu
                    break;
                }
            }
        }
    }

    void Jogo::desenharPausa()//desenha graficamente a pausa
    {
        gg.getJanela()->setView(gg.getJanela()->getDefaultView()); // view padrão pra a câmera não ficar no jogador
        //abaixo define local cor e outras coisas do texto
        sf::Text titulo("PAUSADO", fontePausa, 80);
        titulo.setFillColor(sf::Color::White);
        sf::FloatRect boundsT = titulo.getLocalBounds();
        titulo.setOrigin(boundsT.left + boundsT.width / 2.0f, boundsT.top + boundsT.height / 2.0f); //centraliza 
        titulo.setPosition(LARGURA_TELA / 2.0f, 200.0f); //
        gg.getJanela()->draw(titulo);

        for (const sf::Text& opcao : opcoesPausa)
        {
            gg.getJanela()->draw(opcao);
        }
    }


    void Jogo::carregarRecursosUI()
    {
        // carrega a fonte
        if (!fonteUI.loadFromFile("assets/Pixeboy.ttf"))
        {
            std::cerr << "Erro ao carregar fonte assets/Pixeboy.ttf para UI" << std::endl;//erro se deu caca 
        }

        //abaixo so define as caracteristicas dos textos, nao desenha. APENAS DEFINE

        textoPontosP1.setFont(fonteUI);
        textoPontosP1.setCharacterSize(24);
        textoPontosP1.setFillColor(sf::Color::White);
        textoPontosP1.setPosition(10, 10);

        textoPontosP2.setFont(fonteUI);
        textoPontosP2.setCharacterSize(24);
        textoPontosP2.setFillColor(sf::Color(200, 200, 200));
        textoPontosP2.setPosition(10, 40);

        textoPromptNome.setFont(fonteUI);
        textoPromptNome.setCharacterSize(40);
        textoPromptNome.setFillColor(sf::Color::White);
        textoPromptNome.setString("Digite o nome do Save:");
        sf::FloatRect boundsP = textoPromptNome.getLocalBounds();
        textoPromptNome.setOrigin(boundsP.left + boundsP.width / 2.0f, boundsP.top + boundsP.height / 2.0f);
        textoPromptNome.setPosition(LARGURA_TELA / 2.0f, 350.0f);

        textoInputNome.setFont(fonteUI);
        textoInputNome.setCharacterSize(50);
        textoInputNome.setFillColor(sf::Color::Yellow);
        textoInputNome.setPosition(LARGURA_TELA / 2.0f, 420.0f);

        rankingTitulo.setFont(fonteUI);
        rankingTitulo.setCharacterSize(70);
        rankingTitulo.setFillColor(sf::Color::Cyan);

        rankingVoltar.setFont(fonteUI);
        rankingVoltar.setCharacterSize(30);
        rankingVoltar.setFillColor(sf::Color::Yellow);
        rankingVoltar.setString("Aperte ENTER ou ESC para voltar");
        sf::FloatRect boundsV = rankingVoltar.getLocalBounds();
        rankingVoltar.setOrigin(boundsV.left + boundsV.width / 2.0f, boundsV.top + boundsV.height / 2.0f);
        rankingVoltar.setPosition(LARGURA_TELA / 2.0f, ALTURA_TELA - 60.0f);
    }

    void Jogo::atualizarUI()
    {//atualiza os pontos dos jogadores
        if (pJogador1) {
            textoPontosP1.setString("P1: " + std::to_string(pJogador1->getPontos()));
        }

        if (pJogador2 && pJogador2->getAtivo()) {
            textoPontosP2.setString("P2: " + std::to_string(pJogador2->getPontos()));
        }
        else {
            textoPontosP2.setString("");
        }
    }

    void Jogo::desenharUI()//desenha as pontuações. La so define aqui so desenha
    {
        gg.getJanela()->setView(gg.getJanela()->getDefaultView());

        gg.getJanela()->draw(textoPontosP1);
        if (pJogador2 && pJogador2->getAtivo()) {
            gg.getJanela()->draw(textoPontosP2);
        }
    }


    void Jogo::processarInputNome(sf::Event& evento)
    {
        if (evento.type == sf::Event::TextEntered)
        {
            if (evento.text.unicode == '\b' && !nomeSaveFile.empty()) // se pressiona backspace
            {
                nomeSaveFile.pop_back(); // tira o caractere
            }
            else if (evento.text.unicode >= 32 && evento.text.unicode < 128 && nomeSaveFile.size() < 12) // só aceita caracteres simples
            {
                nomeSaveFile += static_cast<char>(evento.text.unicode);//coloca no arquivo
            }
        }
        else if (evento.type == sf::Event::KeyPressed)
        {
            if (evento.key.code == sf::Keyboard::Enter && !nomeSaveFile.empty())// pressionou enter com algum caractere
            {
                if (proximaFaseAposNome == EstadoJogo::Jogando || proximaFaseAposNome == EstadoJogo::FaseDois) //inicia a fase
                {
                    estadoAtual = proximaFaseAposNome;//proximaFaseAposNome é uma instancia do enum EstadoJogo
                    configurarNovaFase(estadoAtual);
                }
                else if (proximaFaseAposNome == EstadoJogo::CarregandoJogo)
                {
                    carregarJogo(nomeSaveFile + ".save.json");//carrega o arquivo com o nome digitado, se existir
                }
            }
            else if (evento.key.code == sf::Keyboard::Escape)//menu
            {
                voltarAoMenu();//menu
            }
        }
    }

    void Jogo::atualizarInputNome()
    {
        if (cursorClock.getElapsedTime().asSeconds() > 0.5f)//define o tempo piscandinho
        {
            cursorVisivel = !cursorVisivel;
            cursorClock.restart();
        }

        std::string str = nomeSaveFile + (cursorVisivel ? "_" : " "); //faz ele piscar
        textoInputNome.setString(str);// atualiza o texto para piscar junto
        //posição e centraliza
        sf::FloatRect bounds = textoInputNome.getLocalBounds();//
        textoInputNome.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        textoInputNome.setPosition(LARGURA_TELA / 2.0f, 420.0f);
    }

    void Jogo::desenharInputNome()
    {//desenha
        gg.getJanela()->setView(gg.getJanela()->getDefaultView());
        gg.desenharBackground();
        gg.getJanela()->draw(textoPromptNome);
        gg.getJanela()->draw(textoInputNome);
    }


    void Jogo::voltarAoMenu()
    {
        //lógica para voltar ao menu
        limparFaseAtual();
        estadoAtual = EstadoJogo::Menu;//define estado
        menu.resetarEstadoInterno();
        nomeSaveFile.clear();

        if (pJogador2) {
            pJogador2->setAtivo(false);//isso ta aqui pra quando volta pro menu nao iniciar outra fase com 2 jogadores
        }
    }


    void Jogo::configurarNovaFase(EstadoJogo novoEstado)
    {
        limparFaseAtual();

        if (novoEstado == EstadoJogo::Jogando)//jogando é fase1
        {
            faseAtual = new Fases::FasePrimeira(pJogador1, pJogador2, false);//consatrutora fase 1
        }
        else if (novoEstado == EstadoJogo::FaseDois)
        {
            faseAtual = new Fases::FaseSegunda(pJogador1, pJogador2, false);//construtora fase 2
        }


        if (faseAtual) {
            faseAtual->setJogo(this);//relaão bidirecional, define qual o jogo da fse pela perspectiva da fase
        }
    }

    void Jogo::limparFaseAtual()//limpaa a fase
    {
        if (faseAtual) {
            delete faseAtual;
            faseAtual = nullptr;
        }
    }


    void Jogo::executar()
    {
        while (estadoAtual != EstadoJogo::Sair && gg.getJanelaOpen())//while principal, loop do jogo, joguinho aberto
        {
            sf::Event ev;
            while (gg.getJanela()->pollEvent(ev))
            {
                if (ev.type == sf::Event::Closed) {
                    estadoAtual = EstadoJogo::Sair;
                }

                if (estadoAtual == EstadoJogo::Menu) {
                    menu.processarEntrada(ev);
                }
                else if (estadoAtual == EstadoJogo::EntrandoNome) {
                    processarInputNome(ev);
                }
                else if (estadoAtual == EstadoJogo::Pausado) {
                    processarInputPausa(ev);
                }
                else if (estadoAtual == EstadoJogo::MostrandoRanking) {
                    processarInputRanking(ev);
                }
                else if (estadoAtual == EstadoJogo::Jogando || estadoAtual == EstadoJogo::FaseDois)
                {
                    if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::P)
                    {
                        estadoAnterior = estadoAtual;
                        estadoAtual = EstadoJogo::Pausado;
                    }
                }
            }

            Ente::atualizarClockGlobal(estadoAtual == EstadoJogo::Pausado);//atualiza o clock global


            if (estadoAtual == EstadoJogo::Jogando || estadoAtual == EstadoJogo::FaseDois)// se estiver na fase 1(jogando) ou na fasedois
            {
                if (faseAtual)
                {
                    faseAtual->executar();//lógica de execução da fase

                    if (faseAtual->getTerminou())//quando a fase termina caclula a pontuacao
                    {//variaveis que determinam pontuacao e salvamento, pdoeria ser atributo
                        float tempoDecorrido = faseAtual->getTempoDecorrido();
                        const float BONUS_MAXIMO = 5000.0f;
                        const float PENALIDADE_POR_SEGUNDO = 50.0f;
                        int bonusTempo = (int)(BONUS_MAXIMO - (tempoDecorrido * PENALIDADE_POR_SEGUNDO));
                        if (bonusTempo < 0)
                        {
                            bonusTempo = 0;
                        }
                        const int PONTOS_POR_FASE = 1000;
                        sf::FloatRect saida = faseAtual->getAreaSaida();

                        if (pJogador1 && pJogador1->getAtivo()) {
                            if (saida.intersects(pJogador1->getFigura()->getGlobalBounds())) {
                                pJogador1->adicionarPontos(PONTOS_POR_FASE + bonusTempo);
                            }
                        }
                        if (pJogador2 && pJogador2->getAtivo()) {
                            if (saida.intersects(pJogador2->getFigura()->getGlobalBounds())) {
                                pJogador2->adicionarPontos(PONTOS_POR_FASE + bonusTempo);
                            }
                        }

                        int p1pts = (pJogador1) ? pJogador1->getPontos() : 0;//se jogador 1 existe, pega pontos
                        int p2pts = (pJogador2 && pJogador2->getAtivo()) ? pJogador2->getPontos() : 0;//se jogador existe, pega pontos
                        int totalPontos = p1pts + p2pts;

                        std::string arquivoRanking;//variavel auxiliar
                        if (estadoAtual == EstadoJogo::Jogando) //se tu ta jogando
                        {
                            arquivoRanking = "ranking_fase1.json";//endereco do arquivo com os rankings fase 1
                        }
                        else {
                            arquivoRanking = "ranking_fase2.json";//endereco do arquivo com os rankings fase 2
                        }
                        salvarPontuacao(arquivoRanking, nomeSaveFile, totalPontos);//salva a pontuacao

                        std::cout << "Save: '" << nomeSaveFile << "' Pontuacao: " << totalPontos << std::endl;//erros para teste

                        voltarAoMenu();//volta ao menu quando termina tudo
                    }
                }

                if (pJogador2->getAtivo() == false && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) // cria o p2 se aperta espaço
                {
                    if (pJogador1 && pJogador1->getAtivo())
                    {
                        std::cout << "Jogador 2 spawnnado" << std::endl;//testes
                        const float offsetX = 50.0f;//evita o problema do jogador 2 nascer em cima do jogador 1
                        const float offsetY = 40.0f;
                        float spawnX = pJogador1->getX() + offsetX; // cria do lado do p1
                        float spawnY = pJogador1->getY() - offsetY;

                        if (spawnX > MUNDO_X_MAX - 32.0f) //evita que jnogador 2 spwane fora dos limites do mundo
                        {
                            spawnX = pJogador1->getX() - offsetX;
                        }
                        if (spawnY < MUNDO_Y_MIN) 
                        {
                            spawnY = MUNDO_Y_MIN + 10.0f;
                        }

                        pJogador2->resetar(spawnX, spawnY);//nao da set ativo para o jogador2 entrar no jogo e sim usa a função resetar de jogador para isso
                        pJogador2->setGerenciadorGrafico(&gg);
                        if (faseAtual) //boas praticas
                        {
                            faseAtual->adicionarJogador2(pJogador2);
                        }
                    }
                }

                atualizarUI();
            }
            else if (estadoAtual == EstadoJogo::Menu)//se tiver no menu
            {

                EstadoJogo proximo = menu.getProximoEstado();//variavel auxiliar

                if (proximo == EstadoJogo::MostrandoRanking)
                {
                    Menu::MenuNivel nivelMenu = menu.getNivelAtual();
                    std::string titulo;//variavel auxiliar
                    if (nivelMenu == Menu::MenuNivel::FASE1) 
                    {
                        arquivoRankingAtual = "ranking_fase1.json";//se tiver na fase 1 abre ranking fase 1
                        titulo = "Ranking - Fase 1";
                    }
                    else if (nivelMenu == Menu::MenuNivel::FASE2) 
                    {
                        arquivoRankingAtual = "ranking_fase2.json";//se tiver na fase 2 abre ranking fase 2
                        titulo = "Ranking - Fase 2";
                    }

                    carregarRanking(arquivoRankingAtual, titulo);
                    estadoAtual = EstadoJogo::MostrandoRanking;
                    menu.resetarEstadoInterno();
                }
                else if (proximo == EstadoJogo::Jogando || proximo == EstadoJogo::FaseDois || proximo == EstadoJogo::CarregandoJogo)
                {//define para onde o jogo vai e faz ele ir
                    proximaFaseAposNome = proximo;
                    estadoAtual = EstadoJogo::EntrandoNome;
                    nomeSaveFile.clear();
                    cursorClock.restart();
                    menu.resetarEstadoInterno();
                }
                else if (proximo != EstadoJogo::Menu) 
                {//se o proximo nao for o menu(o mesmo), ele redefine o estado atual
                    estadoAtual = proximo;
                }
            }
            else if (estadoAtual == EstadoJogo::EntrandoNome)
            {
                atualizarInputNome();//se tiver na tela de colocar nome
            }

            /*
            else if (estadoAtual == EstadoJogo::Pausado)
            {
            }
            else if (estadoAtual == EstadoJogo::MostrandoRanking)
            {

            }
            */

            gg.clear();
            //desenhos diferentes a seguir
            if (estadoAtual == EstadoJogo::Menu)
            {
                menu.desenharOpcoes();
            }
            else if (estadoAtual == EstadoJogo::EntrandoNome)
            {
                desenharInputNome();
            }
            else if (estadoAtual == EstadoJogo::MostrandoRanking)
            {
                desenharRanking();
            }
            else if ((estadoAtual == EstadoJogo::Jogando || estadoAtual == EstadoJogo::FaseDois || estadoAtual == EstadoJogo::Pausado) && faseAtual)
            {
                faseAtual->desenharFase();
                desenharUI();
                if (estadoAtual == EstadoJogo::Pausado) {
                    desenharPausa();
                }
            }

            gg.render();
        }
    }


    void Jogo::salvarJogo(const std::string& arquivo)
    {
        if (!faseAtual || !pJogador1 || !pJogador2)//so pra evitar erros
        {
            return;
        }

        if (!std::filesystem::exists("saves")) //so pra evitar erros
        {
            std::filesystem::create_directory("saves");//cria a pasta save caso nao exista mas sempre existe
        }

        json j_save;//json auxiliar

        if (estadoAtual == EstadoJogo::Jogando || (estadoAnterior == EstadoJogo::Jogando && estadoAtual == EstadoJogo::Pausado))//salva como fase1
        {
            j_save["fase_id"] = "FasePrimeira";
        }
        else if (estadoAtual == EstadoJogo::FaseDois || (estadoAnterior == EstadoJogo::FaseDois && estadoAtual == EstadoJogo::Pausado))//salva como fase 2
        {
            j_save["fase_id"] = "FaseSegunda";
        }

        //

        pJogador1->salvarDataBuffer(j_save["jogador1"]);//salvar polimórfico do jogador 1
        pJogador2->salvarDataBuffer(j_save["jogador2"]);//salvar polimórfico do jogador 2

        this->faseAtual->salvarFase(j_save);//salvar polimórfico da fase

        std::string caminhoCompleto = "saves/" + arquivo;//caminho do save
        std::ofstream o(caminhoCompleto);//cria um arquivo no endereço do caminhoCompleto 
        if (o.is_open()) 
        {
            o << std::setw(4) << j_save << std::endl;//espaco de 4 espaços para identacao, setw supostamente significa set width. Fica mais fácil pra ler se tu quiser ler o json
            o.close();//fecha o arquivo, que deveria ser realizado automaticamente(acho) porem boas praticas 
            std::cout << "jogo salvo " << caminhoCompleto << std::endl;//testes
        }
        else 
        {
            std::cerr << " nao foi possivel salvar o jogo " << caminhoCompleto << std::endl;
        }
    }

    void Jogo::carregarJogo(const std::string& arquivo)
    {
        std::string caminhoCompleto = "saves/" + arquivo;//caminho do save

        std::ifstream i(caminhoCompleto);//abre um arqvuico no caminhoCompleto 
        if (!i.is_open()) {//caso de erro saber onde
            std::cerr << "nao foi possivel carregar o save: " << caminhoCompleto << std::endl;
            estadoAtual = EstadoJogo::EntrandoNome;
            return;
        }

        json j_save;
        try {//try catch para requisitos e para saves
            i >> j_save;
        }
        catch (json::exception& e) {//caso de erro saber onde
            std::cerr << "nao deu para ler JSON do save: " << e.what() << std::endl;
            i.close();
            estadoAtual = EstadoJogo::EntrandoNome;
            return;
        }
        i.close();

        limparFaseAtual();

        std::string fase_id = j_save.value("fase_id", "");//se a chave fase_id nao existe ele coloca o que ta dentro de "", projo nao crashar
        if (fase_id == "FasePrimeira") //se na primeira fase
        {
            faseAtual = new Fases::FasePrimeira(pJogador1, pJogador2, true);
            estadoAtual = EstadoJogo::Jogando;
        }
        else if (fase_id == "FaseSegunda") // se na segunda fase
        {
            faseAtual = new Fases::FaseSegunda(pJogador1, pJogador2, true);
            estadoAtual = EstadoJogo::FaseDois;
        }
        else {
            std::cerr << "Erro: ID de fase desconhecido no save." << std::endl;
            voltarAoMenu();
            return;
        }

        faseAtual->setJogo(this);//diz qual o jogo da fase
        pJogador1->carregarDeBuffer(j_save["jogador1"]);//carrega jogador1
        pJogador2->carregarDeBuffer(j_save["jogador2"]);//carrega jogadopr 2
        if (pJogador2->getAtivo()) {
            faseAtual->adicionarJogador2(pJogador2);
        }
        
        auto& listaInimigos = j_save["inimigos"];
        for (size_t i = 0; i < listaInimigos.size(); i++) 
        {//percorre a sub-sessão do json de iminigos, auto substitui nlohmann::basic_json<>& inim_data
            criarEntidade(listaInimigos[i]);//cria a entidade
        }

        auto& listaObstaculos = j_save["obstaculos"];// auto substitui  nlohmann::basic_json<>& obst_data
        for (size_t i = 0; i < listaObstaculos.size(); i++) //percorre a lista, 
        {
            criarEntidade(listaObstaculos[i]);//cria a entidade
        }
        // auto substitue const nlohmann::basic_json<>&
        
        auto listaProjeteis = j_save.value("projeteis", json::array()); //auto substitui nlohmann::basic_json<>& inim_data
        for (size_t i = 0; i < listaProjeteis.size(); i++)
        {
            const auto& proj_data = listaProjeteis[i];//auto substitue CyberMetro::Listas::Lista<CyberMetro::Entidades::Entidade>::Elemento*

            Entidades::Projetil* pProj = new Entidades::Projetil();//cria projetil
            pProj->setGerenciadorGrafico(&gg);
            pProj->carregarDeBuffer(proj_data); //chama carregar do projetil

            if (!pProj->getAtivo()) {//se nao tiver ativo no salvamento ele deleta
                delete pProj;
                continue;
            }

            int dono = pProj->getIdDono();
            //insere os projteis caregados na lista de quem os pertence
            if (dono == 1 && pJogador1) 
            {
                pJogador1->getProjeteis()->inserir(pProj);
            }
            else if (dono == 2 && pJogador2) {
                pJogador2->getProjeteis()->inserir(pProj);
            }
            else if (dono == 0 && faseAtual) { // testa se o dono é CEO e se existe fase
                auto* inimigos = faseAtual->getListaInimigos()->getPrimeiro(); // definição sem auto fica gigantesca
                bool adicionado = false;
                while (inimigos)
                {
                    Personagens::Robo_CEO* ceo = dynamic_cast<Personagens::Robo_CEO*>(inimigos->getInfo());//da um cast no robo ceo para confirmar que ele é roboCeo
                    if (ceo && ceo->getAtivo()) 
                    {//se tem algo no ponteiro CEO e se tá ativo
                        ceo->getProjeteis()->inserir(pProj);//insre o projetil e seta adicionado true
                        adicionado = true;
                        break;
                    }
                    inimigos = inimigos->getProx();//avança a lista
                }
                if (!adicionado) 
                {
                    delete pProj;
                }
            }
            else
            {
                delete pProj;
            }
        }

        Ente::atualizarClockGlobal(false);//tem que ser falso para o projetil nao se teletransportar após o carregamento, já que tem um tempo hábil para o arquivo ser lido

        std::cout << "Jogo carregado de: " << caminhoCompleto << std::endl;//teste
    }

    //ATENÇÃO ESSE CRIAR ENTIDADE É UM MÉTODO QUE AJUDA A CARREAGR/RECRIAR A ENTIDADE PARA O METODO CARREGAR, QUEM CRIA PELA PRIMERIA VEZ ENTIDADES É FASE
    void Jogo::criarEntidade(const json& data)
    {
        std::string tipo = data.value("tipo", " ");//variavel auxiliar
        Entidades::Entidade* pEnt = nullptr;//ponteiro auxiliar
        //Chama a construtora com base no tipo de entidade que está definido no arquivo salvar
        if (tipo == "Robo_Junior") 
        {
            pEnt = new Personagens::Robo_Junior(0, 0);//valor arbitrario pra não passar algo não inicializado ou sem valor
        }
        else if (tipo == "Robo_Senior") 
        {
            pEnt = new Personagens::Robo_Senior(0, 0);//valor arbitrario pra não passar algo não inicializado ou sem valor
        }
        else if (tipo == "Robo_CEO") 
        {
            pEnt = new Personagens::Robo_CEO(0, 0);//valor arbitrario pra não passar algo não inicializado ou sem valor
        }

        else if (tipo == "Plataforma") 
        {
            pEnt = new Obstaculos::Plataforma(0, 0, 64);//valor arbitrario pra não passar algo não inicializado ou sem valor
        }
        else if (tipo == "Gelinho")
        {
            pEnt = new Obstaculos::Gelinho(0, 0);//valor arbitrario pra não passar algo não inicializado ou sem valor
        }
        else if (tipo == "Choquinho") 
        {
            pEnt = new Obstaculos::Choquinho(0, 0);//valor arbitrario pra não passar algo não inicializado ou sem valor
        }

        if (pEnt)//verificação evita erro
        {
            pEnt->setGerenciadorGrafico(&gg);
            pEnt->carregarDeBuffer(data);//REDEFINE OS VALORES AQUI

            if (dynamic_cast<Personagens::Inimigo*>(pEnt)) //evita erro
            {
                faseAtual->getListaInimigos()->inserir(pEnt);//insre na lista
            }
            else if (dynamic_cast<Obstaculos::Obstaculo*>(pEnt)) //evita erro
            {
                faseAtual->getListaObstaculos()->inserir(pEnt);//insere na lista
            }
        }
    }


    void Jogo::salvarPontuacao(const std::string& arquivo, const std::string& nome, int pontuacao)
    {
        if (!std::filesystem::exists("saves"))//evitar erro caso pasta save nao existe
        {
            std::filesystem::create_directory("saves");//se não existe a pasta ele cria
        }

        std::string caminhoCompleto = "saves/" + arquivo;//variavel auxiliar

        rankingNomes.clear();//limpa tela em duvida
        rankingPontuacoes.clear();//limpa tela em duvida

        std::ifstream i(caminhoCompleto);//abre um arquivo com caminho caminhocompleto
        json j;
        if (i.is_open() && i.peek() != std::ifstream::traits_type::eof()) //se abriu o json e se ele não está vazio
        {//requisitos try/catch e para pegar erro
            try
            {
                i >> j;//tenta carregar para o json
				for (size_t i = 0; i < j.size(); i++) //percorre a lista e vai adicionando os dados no arquivo json
                {
					rankingNomes.push_back(j[i].value("nome", ""));
					rankingPontuacoes.push_back(j[i].value("pontuacao", 0));
				}
                /*
                for (const auto& entry : j) {
                    rankingNomes.push_back(entry.value("nome", ""));
                    rankingPontuacoes.push_back(entry.value("pontuacao", 0));
                }
                */
            }
            catch (json::exception& e) 
            {
                std::cerr << "erro ao ler JSON ranking" << e.what() << std::endl;
            }
        }
        i.close();//fecha o arquivo, que deve ser automatico mas boas praticas
        rankingNomes.push_back(nome);//adiciona novo nome
        rankingPontuacoes.push_back(pontuacao);//adiciona nova pontuação
        sortRanking(rankingNomes, rankingPontuacoes);//faz o sort pro ranking ficar em ordem

        if (rankingNomes.size() > 10)//limita o tamanho do ranking em 10
        {
            rankingNomes.resize(10);
            rankingPontuacoes.resize(10);
        }

        json j_out = json::array();//json auxiliar 
        for (size_t k = 0; k < rankingNomes.size(); ++k) 
        {
            j_out.push_back({ {"nome", rankingNomes[k]}, {"pontuacao", rankingPontuacoes[k]} });
        }

        std::ofstream o(caminhoCompleto);//abre um arquivo com caminho caminhocompleto
        if (!o.is_open()) 
        {
            std::cerr << "nao foi possivel abrir arquivo de ranking para salvar: " << caminhoCompleto << std::endl;
            return;
        }

        o << std::setw(4) << j_out << std::endl;//identação de 4 espaços pra ler miais fácil
        o.close();//boas praticas
    }


    void Jogo::carregarRanking(const std::string& arquivo, const std::string& titulo)
    {
        std::string caminhoCompleto = "saves/" + arquivo;//variavel auxiliar
        //limpa por boas praticas e certeza
        rankingTextos.clear();
        rankingNomes.clear();
        rankingPontuacoes.clear();
        //formatação ranking
        rankingTitulo.setString(titulo);
        sf::FloatRect boundsT = rankingTitulo.getLocalBounds();
        rankingTitulo.setOrigin(boundsT.left + boundsT.width / 2.0f, boundsT.top + boundsT.height / 2.0f);
        rankingTitulo.setPosition(LARGURA_TELA / 2.0f, 100.0f);
        std::ifstream i(caminhoCompleto);//abre um arquivo com caminho caminhocompleto
        json j;//json que vai guardar os valores de nomes e pontuações para colocar nas listas de texto
        // nlohmann::basic_json<>& entry
        if (i.is_open() && i.peek() != std::ifstream::traits_type::eof())// se abriu o arquivo e se ele não está vazio
        {
            try 
                {
                i >> j;
                for (size_t i = 0; i < j.size(); i++) //percorre a lista e vai adicionando os dados no arquivo json
                {
                    rankingNomes.push_back(j[i].value("nome", ""));
                    rankingPontuacoes.push_back(j[i].value("pontuacao", 0));
                }
                /*
                for (const auto& entry : j) 
                { 
                    rankingNomes.push_back(entry.value("nome", ""));
                    rankingPontuacoes.push_back(entry.value("pontuacao", 0));
                }
                */
            }
            catch (json::exception& e) //print erro cas
               {
                std::cerr << "erro ao carregar JSON ranking" << e.what() << std::endl;
            }
        }
        i.close();//provavcel automatico porem boas praticas 
        //define a posição dos trextos e outras caracteristicas como cor
        if (rankingNomes.empty()) 
        {
            sf::Text texto("Nenhuma pontuacao registrada.", fonteUI, 30);
            texto.setFillColor(sf::Color::White);
            sf::FloatRect bounds = texto.getLocalBounds();
            texto.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
            texto.setPosition(LARGURA_TELA / 2.0f, ALTURA_TELA / 2.0f);
            rankingTextos.push_back(texto);
        }
        else 
            {
            float yPos = 200.0f;
            for (size_t i = 0; i < rankingNomes.size(); ++i)
            {
                std::string str = std::to_string(i + 1) + ". " + rankingNomes[i] + " - " + std::to_string(rankingPontuacoes[i]);
                sf::Text texto(str, fonteUI, 40);
                texto.setFillColor(sf::Color::White);
                sf::FloatRect bounds = texto.getLocalBounds();
                texto.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
                texto.setPosition(LARGURA_TELA / 2.0f, yPos);
                rankingTextos.push_back(texto);
                yPos += 50.0f;
            }
        }
    }

    void Jogo::processarInputRanking(sf::Event& evento)//pra tu sair do menu apertando esc
    {
        if (evento.type == sf::Event::KeyReleased)
        {
            if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Escape)
            {
                voltarAoMenu();
            }
        }
    }

    void Jogo::desenharRanking()//desenha o ranking
    {
        gg.getJanela()->setView(gg.getJanela()->getDefaultView());
        gg.desenharBackground();

        gg.getJanela()->draw(rankingTitulo);
        gg.getJanela()->draw(rankingVoltar);

        for (const auto& texto : rankingTextos)
        {
            gg.getJanela()->draw(texto);
        }
    }

    void Jogo::sortRanking(std::vector<std::string>& nomes, std::vector<int>& pontuacoes)
    {
        if (nomes.size() != pontuacoes.size()) //pra que não fiquem pontos sem nome ou nomes sem ponto
        {
            return;
        }
        std::size_t n = nomes.size();//se der int ele chora
        for (std::size_t i = 0; i < n; ++i)
        {
            std::size_t Maior = i; //o maior é o primeiro só pra comparaçao
            for (std::size_t j = i + 1; j < n; ++j)
            {
                if (pontuacoes[j] > pontuacoes[Maior])
                {
                    Maior = j;
                }
            }
            if (Maior != i) // troca as posições de nome e pontos
            {
                std::swap(pontuacoes[i], pontuacoes[Maior]);
                std::swap(nomes[i], nomes[Maior]);
            }
        }
    }
}
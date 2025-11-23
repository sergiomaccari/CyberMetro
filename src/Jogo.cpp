#include "Jogo.h"
#include "FasePrimeira.h" 
#include "FaseSegunda.h"  
#include "Gerenciador_Grafico.h"
#include <string> 
#include <nlohmann/json.hpp> 
#include <fstream>             
#include <algorithm>           
#include <iomanip>             
#include <utility>             
#include <vector>              
#include <iostream>
#include <filesystem>

#include "Robo_Junior.h"
#include "Robo_Senior.h"
#include "Robo_CEO.h"
#include "Plataforma.h"
#include "Gelinho.h"
#include "Choquinho.h"
#include "Projetil.h" 
#include "Ente.h" 

using namespace CyberMetro::Entidades;
using namespace CyberMetro::Entidades::Personagens;
using namespace CyberMetro::Entidades::Obstaculos;
using json = nlohmann::json;

void sortRanking(std::vector<std::string>& nomes, std::vector<int>& pontuacoes) {
    if (nomes.size() != pontuacoes.size()) return;

    std::vector<std::pair<int, std::string>> pares;
    for (size_t i = 0; i < nomes.size(); ++i) {
        pares.push_back({ pontuacoes[i], nomes[i] });
    }

    std::sort(pares.begin(), pares.end(), [](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {
        return a.first > b.first;
        });

    nomes.clear();
    pontuacoes.clear();
    for (const auto& p : pares) {
        nomes.push_back(p.second);
        pontuacoes.push_back(p.first);
    }
}


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

        pJogador2->setAtivo(false);
    }

    Jogo::~Jogo()
    {
        limparFaseAtual();
        if (pJogador1) {
            delete pJogador1;
            pJogador1 = nullptr;
        }
        if (pJogador2) {
            delete pJogador2;
            pJogador2 = nullptr;
        }

        std::cout << "Jogo encerrado" << std::endl;

    }

    void Jogo::carregarRecursosPausa()
    {
        // CAMINHO ATUALIZADO PARA ASSETS/
        if (!fontePausa.loadFromFile("assets/Pixeboy.ttf"))
        {
            std::cerr << "Erro ao carregar fonte assets/Pixeboy.ttf para pausa" << std::endl;
        }

        fundoPausa.setSize(sf::Vector2f(LARGURA_TELA, ALTURA_TELA));
        fundoPausa.setFillColor(sf::Color(0, 0, 0, 150));

        std::vector<std::string> textos = { "Continuar", "Salvar Jogo", "Voltar ao Menu" };
        for (size_t i = 0; i < textos.size(); ++i)
        {
            sf::Text texto(textos[i], fontePausa, 50);
            sf::FloatRect bounds = texto.getLocalBounds();
            texto.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
            texto.setPosition(LARGURA_TELA / 2.0f, 350.0f + i * 100.0f);
            opcoesPausa.push_back(texto);
        }
        atualizarDestaquePausa();
    }

    void Jogo::atualizarDestaquePausa()
    {
        for (size_t i = 0; i < opcoesPausa.size(); ++i)
        {
            opcoesPausa[i].setFillColor((i == indicePausa) ? sf::Color::Yellow : sf::Color::White);
        }
    }

    void Jogo::processarInputPausa(sf::Event& evento)
    {
        if (evento.type == sf::Event::KeyPressed)
        {
            if (evento.key.code == sf::Keyboard::P) {
                estadoAtual = estadoAnterior;
                return;
            }
            else if (evento.key.code == sf::Keyboard::Up) {
                indicePausa = (indicePausa - 1 + opcoesPausa.size()) % opcoesPausa.size();
                atualizarDestaquePausa();
            }
            else if (evento.key.code == sf::Keyboard::Down) {
                indicePausa = (indicePausa + 1) % opcoesPausa.size();
                atualizarDestaquePausa();
            }
        }
        else if (evento.type == sf::Event::KeyReleased)
        {
            if (evento.key.code == sf::Keyboard::Enter) {
                switch (indicePausa)
                {
                case 0:
                    estadoAtual = estadoAnterior;
                    break;
                case 1:
                    salvarJogo(nomeSaveFile + ".save.json");
                    break;
                case 2:
                    voltarAoMenu();
                    break;
                }
            }
        }
    }

    void Jogo::desenharPausa()
    {
        gg.getJanela()->setView(gg.getJanela()->getDefaultView());

        sf::Text titulo("PAUSADO", fontePausa, 80);
        titulo.setFillColor(sf::Color::White);
        sf::FloatRect boundsT = titulo.getLocalBounds();
        titulo.setOrigin(boundsT.left + boundsT.width / 2.0f, boundsT.top + boundsT.height / 2.0f);
        titulo.setPosition(LARGURA_TELA / 2.0f, 200.0f);
        gg.getJanela()->draw(titulo);

        for (const sf::Text& opcao : opcoesPausa)
        {
            gg.getJanela()->draw(opcao);
        }
    }


    void Jogo::carregarRecursosUI()
    {
        // CAMINHO ATUALIZADO PARA ASSETS/
        if (!fonteUI.loadFromFile("assets/Pixeboy.ttf"))
        {
            std::cerr << "Erro ao carregar fonte assets/Pixeboy.ttf para UI" << std::endl;
        }

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
    {
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

    void Jogo::desenharUI()//pontuacao
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
            if (evento.text.unicode == '\b' && !nomeSaveFile.empty()) {
                nomeSaveFile.pop_back();
            }
            else if (evento.text.unicode >= 32 && evento.text.unicode < 128 && nomeSaveFile.size() < 12) {
                nomeSaveFile += static_cast<char>(evento.text.unicode);
            }
        }
        else if (evento.type == sf::Event::KeyPressed)
        {
            if (evento.key.code == sf::Keyboard::Enter && !nomeSaveFile.empty())
            {
                if (proximaFaseAposNome == EstadoJogo::Jogando || proximaFaseAposNome == EstadoJogo::FaseDois)
                {
                    estadoAtual = proximaFaseAposNome;
                    configurarNovaFase(estadoAtual);
                }
                else if (proximaFaseAposNome == EstadoJogo::CarregandoJogo)
                {
                    carregarJogo(nomeSaveFile + ".save.json");
                }
            }
            else if (evento.key.code == sf::Keyboard::Escape)
            {
                voltarAoMenu();
            }
        }
    }

    void Jogo::atualizarInputNome()
    {
        if (cursorClock.getElapsedTime().asSeconds() > 0.5f)
        {
            cursorVisivel = !cursorVisivel;
            cursorClock.restart();
        }

        std::string str = nomeSaveFile + (cursorVisivel ? "_" : " ");
        textoInputNome.setString(str);

        sf::FloatRect bounds = textoInputNome.getLocalBounds();
        textoInputNome.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        textoInputNome.setPosition(LARGURA_TELA / 2.0f, 420.0f);
    }

    void Jogo::desenharInputNome()
    {
        gg.getJanela()->setView(gg.getJanela()->getDefaultView());
        gg.desenharBackground();
        gg.getJanela()->draw(textoPromptNome);
        gg.getJanela()->draw(textoInputNome);
    }


    void Jogo::voltarAoMenu()
    {
        limparFaseAtual();
        estadoAtual = EstadoJogo::Menu;
        menu.resetarEstadoInterno();
        nomeSaveFile.clear();

        if (pJogador2) {
            pJogador2->setAtivo(false);
        }
    }


    void Jogo::configurarNovaFase(EstadoJogo novoEstado)
    {
        limparFaseAtual();

        if (novoEstado == EstadoJogo::Jogando)
        {
            faseAtual = new Fases::FasePrimeira(pJogador1, pJogador2, false);
        }
        else if (novoEstado == EstadoJogo::FaseDois)
        {
            faseAtual = new Fases::FaseSegunda(pJogador1, pJogador2, false);
        }


        if (faseAtual) {
            faseAtual->setJogo(this);
        }
    }

    void Jogo::limparFaseAtual()
    {
        if (faseAtual) {
            delete faseAtual;
            faseAtual = nullptr;
        }
    }


    void Jogo::executar()
    {
        while (estadoAtual != EstadoJogo::Sair && gg.getJanelaOpen())
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

            Ente::atualizarClockGlobal(estadoAtual == EstadoJogo::Pausado);


            if (estadoAtual == EstadoJogo::Jogando || estadoAtual == EstadoJogo::FaseDois)
            {
                if (faseAtual)
                {
                    faseAtual->executar();

                    if (faseAtual->getTerminou())
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

                        int p1pts = (pJogador1) ? pJogador1->getPontos() : 0;
                        int p2pts = (pJogador2 && pJogador2->getAtivo()) ? pJogador2->getPontos() : 0;
                        int totalPontos = p1pts + p2pts;

                        std::string arquivoRanking;
                        if (estadoAtual == EstadoJogo::Jogando) {
                            arquivoRanking = "ranking_fase1.json";
                        }
                        else {
                            arquivoRanking = "ranking_fase2.json";
                        }
                        salvarPontuacao(arquivoRanking, nomeSaveFile, totalPontos);

                        std::cout << "Save: '" << nomeSaveFile << "' Pontuacao Total: " << totalPontos << std::endl;

                        voltarAoMenu();
                    }
                }

                if (pJogador2->getAtivo() == false && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    if (pJogador1 && pJogador1->getAtivo())
                    {
                        std::cout << "Jogador 2 spawnnado" << std::endl;
                        const float offsetX = 50.0f;
                        const float offsetY = 40.0f;
                        float spawnX = pJogador1->getX() + offsetX;
                        float spawnY = pJogador1->getY() - offsetY;

                        if (spawnX > MUNDO_X_MAX - 32.0f) {
                            spawnX = pJogador1->getX() - offsetX;
                        }
                        if (spawnY < MUNDO_Y_MIN) {
                            spawnY = MUNDO_Y_MIN + 10.0f;
                        }

                        pJogador2->resetar(spawnX, spawnY);
                        pJogador2->setGerenciadorGrafico(&gg);
                        if (faseAtual) {
                            faseAtual->adicionarJogador2(pJogador2);
                        }
                    }
                }

                atualizarUI();
            }
            else if (estadoAtual == EstadoJogo::Menu)
            {
                menu.executar();

                EstadoJogo proximo = menu.getProximoEstado();

                if (proximo == EstadoJogo::MostrandoRanking)
                {
                    Menu::MenuNivel nivelMenu = menu.getNivelAtual();
                    std::string titulo;
                    if (nivelMenu == Menu::MenuNivel::FASE1) {
                        arquivoRankingAtual = "ranking_fase1.json";
                        titulo = "Ranking - Fase 1";
                    }
                    else if (nivelMenu == Menu::MenuNivel::FASE2) {
                        arquivoRankingAtual = "ranking_fase2.json";
                        titulo = "Ranking - Fase 2";
                    }

                    carregarRanking(arquivoRankingAtual, titulo);
                    estadoAtual = EstadoJogo::MostrandoRanking;
                    menu.resetarEstadoInterno();
                }
                else if (proximo == EstadoJogo::Jogando || proximo == EstadoJogo::FaseDois ||
                    proximo == EstadoJogo::CarregandoJogo)
                {
                    proximaFaseAposNome = proximo;
                    estadoAtual = EstadoJogo::EntrandoNome;
                    nomeSaveFile.clear();
                    cursorClock.restart();
                    menu.resetarEstadoInterno();
                }
                else if (proximo != EstadoJogo::Menu) {
                    estadoAtual = proximo;
                }
            }
            else if (estadoAtual == EstadoJogo::EntrandoNome)
            {
                atualizarInputNome();
            }
            else if (estadoAtual == EstadoJogo::Pausado)
            {
            }
            else if (estadoAtual == EstadoJogo::MostrandoRanking)
            {

            }

            gg.clear();

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
        if (!faseAtual || !pJogador1 || !pJogador2)
        {
            return;
        }

        if (!std::filesystem::exists("saves")) {
            std::filesystem::create_directory("saves");
        }

        json j_save;

        if (estadoAtual == EstadoJogo::Jogando || (estadoAnterior == EstadoJogo::Jogando && estadoAtual == EstadoJogo::Pausado))
        {
            j_save["fase_id"] = "FasePrimeira";
        }
        else if (estadoAtual == EstadoJogo::FaseDois || (estadoAnterior == EstadoJogo::FaseDois && estadoAtual == EstadoJogo::Pausado))
        {

            j_save["fase_id"] = "FaseSegunda";
        }
        else
        {
            j_save["fase_id"] = "Nenhuma";
        }


        pJogador1->salvarDataBuffer(j_save["jogador1"]);
        pJogador2->salvarDataBuffer(j_save["jogador2"]);

        this->faseAtual->salvarFase(j_save);
        std::string caminhoCompleto = "saves/" + arquivo;

        std::ofstream o(caminhoCompleto);
        if (o.is_open()) {
            o << std::setw(4) << j_save << std::endl;
            o.close();
            std::cout << "Jogo salvo em: " << caminhoCompleto << std::endl;
        }
        else {
            std::cerr << "Erro: Nao foi possivel salvar o jogo em: " << caminhoCompleto << std::endl;
        }
    }

    void Jogo::carregarJogo(const std::string& arquivo)
    {
        std::string caminhoCompleto = "saves/" + arquivo;

        std::ifstream i(caminhoCompleto);
        if (!i.is_open()) {
            std::cerr << "Erro: Nao foi possivel carregar o save: " << caminhoCompleto << std::endl;
            estadoAtual = EstadoJogo::EntrandoNome;
            return;
        }

        json j_save;
        try {
            i >> j_save;
        }
        catch (json::exception& e) {
            std::cerr << "Erro ao ler JSON do save: " << e.what() << std::endl;
            i.close();
            estadoAtual = EstadoJogo::EntrandoNome;
            return;
        }
        i.close();

        limparFaseAtual();

        std::string fase_id = j_save.value("fase_id", "Nenhuma");
        if (fase_id == "FasePrimeira") {
            faseAtual = new Fases::FasePrimeira(pJogador1, pJogador2, true);
            estadoAtual = EstadoJogo::Jogando;
        }
        else if (fase_id == "FaseSegunda") {
            faseAtual = new Fases::FaseSegunda(pJogador1, pJogador2, true);
            estadoAtual = EstadoJogo::FaseDois;
        }
        else {
            std::cerr << "Erro: ID de fase desconhecido no save." << std::endl;
            voltarAoMenu();
            return;
        }

        faseAtual->setJogo(this);
        pJogador1->carregarDeBuffer(j_save["jogador1"]);
        pJogador2->carregarDeBuffer(j_save["jogador2"]);
        if (pJogador2->getAtivo()) {
            faseAtual->adicionarJogador2(pJogador2);
        }

        for (const auto& inim_data : j_save["inimigos"]) {
            criarEntidade(inim_data);
        }

        for (const auto& obst_data : j_save["obstaculos"]) {
            criarEntidade(obst_data);
        }

        for (const auto& proj_data : j_save.value("projeteis", json::array()))
        {
            Entidades::Projetil* pProj = new Entidades::Projetil();
            pProj->setGerenciadorGrafico(&gg);
            pProj->carregarDeBuffer(proj_data);

            if (!pProj->getAtivo()) {
                delete pProj;
                continue;
            }

            int dono = pProj->getIdDono();
            if (dono == 1 && pJogador1) {
                pJogador1->getProjeteis()->inserir(pProj);
            }
            else if (dono == 2 && pJogador2) {
                pJogador2->getProjeteis()->inserir(pProj);
            }
            else if (dono == 0 && faseAtual) {
                auto* inimigos = faseAtual->getListaInimigos()->getPrimeiro();
                bool adicionado = false;
                while (inimigos) {
                    Personagens::Robo_CEO* ceo = dynamic_cast<Personagens::Robo_CEO*>(inimigos->getInfo());
                    if (ceo && ceo->getAtivo()) {
                        ceo->getProjeteis()->inserir(pProj);
                        adicionado = true;
                        break;
                    }
                    inimigos = inimigos->getProx();
                }
                if (!adicionado) {
                    delete pProj;
                }
            }
            else {
                delete pProj;
            }
        }

        Ente::atualizarClockGlobal(false);

        std::cout << "Jogo carregado de: " << caminhoCompleto << std::endl;
    }

    void Jogo::criarEntidade(const json& data)
    {
        std::string tipo = data.value("tipo", "DESCONHECIDO");
        Entidades::Entidade* pEnt = nullptr;

        if (tipo == "Robo_Junior") {
            pEnt = new Personagens::Robo_Junior(0, 0);
        }
        else if (tipo == "Robo_Senior") {
            pEnt = new Personagens::Robo_Senior(0, 0);
        }
        else if (tipo == "Robo_CEO") {
            pEnt = new Personagens::Robo_CEO(0, 0);
        }
        else if (tipo == "Plataforma") {
            pEnt = new Obstaculos::Plataforma(0, 0, 64);
        }
        else if (tipo == "Gelinho") {
            pEnt = new Obstaculos::Gelinho(0, 0);
        }
        else if (tipo == "Choquinho") {
            pEnt = new Obstaculos::Choquinho(0, 0);
        }

        if (pEnt) {
            pEnt->setGerenciadorGrafico(&gg);
            pEnt->carregarDeBuffer(data);

            if (dynamic_cast<Personagens::Inimigo*>(pEnt)) {
                faseAtual->getListaInimigos()->inserir(pEnt);
            }
            else if (dynamic_cast<Obstaculos::Obstaculo*>(pEnt)) {
                faseAtual->getListaObstaculos()->inserir(pEnt);
            }
        }
    }


    void Jogo::salvarPontuacao(const std::string& arquivo, const std::string& nome, int pontuacao)
    {
        if (!std::filesystem::exists("saves")) {
            std::filesystem::create_directory("saves");
        }

        std::string caminhoCompleto = "saves/" + arquivo;

        rankingNomes.clear();
        rankingPontuacoes.clear();

        std::ifstream i(caminhoCompleto);
        json j;
        if (i.is_open() && i.peek() != std::ifstream::traits_type::eof()) {
            try {
                i >> j;
                for (const auto& entry : j) {
                    rankingNomes.push_back(entry.value("nome", ""));
                    rankingPontuacoes.push_back(entry.value("pontuacao", 0));
                }
            }
            catch (json::exception& e) {
                std::cerr << "Erro ao ler JSON do ranking: " << e.what() << std::endl;
            }
        }
        i.close();
        rankingNomes.push_back(nome);
        rankingPontuacoes.push_back(pontuacao);
        sortRanking(rankingNomes, rankingPontuacoes);

        if (rankingNomes.size() > 10) {
            rankingNomes.resize(10);
            rankingPontuacoes.resize(10);
        }

        json j_out = json::array();
        for (size_t k = 0; k < rankingNomes.size(); ++k) {
            j_out.push_back({
                {"nome", rankingNomes[k]},
                {"pontuacao", rankingPontuacoes[k]}
                });
        }

        std::ofstream o(caminhoCompleto);
        if (!o.is_open()) {
            std::cerr << "Erro: Nao foi possivel abrir arquivo de ranking para salvar: " << caminhoCompleto << std::endl;
            return;
        }

        o << std::setw(4) << j_out << std::endl;
        o.close();
    }


    void Jogo::carregarRanking(const std::string& arquivo, const std::string& titulo)
    {
        std::string caminhoCompleto = "saves/" + arquivo;

        rankingTextos.clear();
        rankingNomes.clear();
        rankingPontuacoes.clear();

        rankingTitulo.setString(titulo);
        sf::FloatRect boundsT = rankingTitulo.getLocalBounds();
        rankingTitulo.setOrigin(boundsT.left + boundsT.width / 2.0f, boundsT.top + boundsT.height / 2.0f);
        rankingTitulo.setPosition(LARGURA_TELA / 2.0f, 100.0f);
        std::ifstream i(caminhoCompleto);
        json j;

        if (i.is_open() && i.peek() != std::ifstream::traits_type::eof())
        {
            try {
                i >> j;
                for (const auto& entry : j) {
                    rankingNomes.push_back(entry.value("nome", ""));
                    rankingPontuacoes.push_back(entry.value("pontuacao", 0));
                }
            }
            catch (json::exception& e) {
                std::cerr << "Erro ao carregar JSON do ranking: " << e.what() << std::endl;
            }
        }
        i.close();

        if (rankingNomes.empty()) {
            sf::Text texto("Nenhuma pontuacao registrada.", fonteUI, 30);
            texto.setFillColor(sf::Color::White);
            sf::FloatRect bounds = texto.getLocalBounds();
            texto.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
            texto.setPosition(LARGURA_TELA / 2.0f, ALTURA_TELA / 2.0f);
            rankingTextos.push_back(texto);
        }
        else {
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

    void Jogo::processarInputRanking(sf::Event& evento)
    {
        if (evento.type == sf::Event::KeyReleased)
        {
            if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Escape)
            {
                voltarAoMenu();
            }
        }
    }

    void Jogo::desenharRanking()
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
}
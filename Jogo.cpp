#include "Jogo.h"
#include "FasePrimeira.h" 
#include "FaseSegunda.h"  
#include "Gerenciador_Grafico.h"
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <algorithm>
#include <iomanip>

using json = nlohmann::json;

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
    if (!fontePausa.loadFromFile("Pixeboy.ttf"))
    {
        std::cerr << "Erro ao carregar fonte Pixeboy.ttf para pausa" << std::endl;
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
                salvarJogo();
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

    gg.getJanela()->draw(fundoPausa);

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
    if (!fonteUI.loadFromFile("Pixeboy.ttf"))
    {
        std::cerr << "Erro ao carregar fonte Pixeboy.ttf para UI" << std::endl;
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

void Jogo::desenharUI()
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
            estadoAtual = proximaFaseAposNome;
            configurarNovaFase(estadoAtual);
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



void Jogo::salvarJogo()
{
    std::cout << "teste Jogo salvo" << std::endl;
}

void Jogo::voltarAoMenu()
{
    limparFaseAtual();
    estadoAtual = EstadoJogo::Menu;
    menu.resetarEstadoInterno();

    if (pJogador2) {
        pJogador2->setAtivo(false);
    }
}


void Jogo::configurarNovaFase(EstadoJogo novoEstado)
{
    limparFaseAtual();

    if (novoEstado == EstadoJogo::Jogando)
    {
        faseAtual = new Fases::FasePrimeira(pJogador1, pJogador2);
    }
    else if (novoEstado == EstadoJogo::FaseDois)
    {
        faseAtual = new Fases::FaseSegunda(pJogador1, pJogador2);
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

        if (estadoAtual == EstadoJogo::Jogando || estadoAtual == EstadoJogo::FaseDois)
        {
            if (faseAtual)
            {
                faseAtual->executar();

                if (faseAtual->getTerminou())
                {

                    float tempoDecorrido = faseAtual->getTempoDecorrido();
                    const float BONUS_MAXIMO = 5000.0f;
                    const float PENALIDADE_POR_SEGUNDO = 50.0f;
                    int bonusTempo = (int)(BONUS_MAXIMO - (tempoDecorrido * PENALIDADE_POR_SEGUNDO));

                    if (bonusTempo < 0) bonusTempo = 0;

                    const int PONTOS_POR_FASE = 1000;
                    sf::FloatRect saida = faseAtual->getAreaSaida();

                    if (pJogador1 && pJogador1->getAtivo()) {
                        if (saida.intersects(pJogador1->getFigura()->getGlobalBounds())) {
                            pJogador1->adicionarPontos(PONTOS_POR_FASE + bonusTempo);
                            std::cout << "Jogador 1 ganhou " << (PONTOS_POR_FASE + bonusTempo) << " pontos!" << std::endl;
                        }
                    }
                    if (pJogador2 && pJogador2->getAtivo()) {
                        if (saida.intersects(pJogador2->getFigura()->getGlobalBounds())) {
                            pJogador2->adicionarPontos(PONTOS_POR_FASE + bonusTempo);
                            std::cout << "Jogador 2 ganhou " << (PONTOS_POR_FASE + bonusTempo) << " pontos!" << std::endl;
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
            else if (proximo == EstadoJogo::Jogando || proximo == EstadoJogo::FaseDois)
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
        else if (estadoAtual == EstadoJogo::MostrandoRanking)
        {
            // Nenhuma lógica de atualização necessária por enquanto
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
        // --- ADICIONADO ---
        else if (estadoAtual == EstadoJogo::MostrandoRanking)
        {
            desenharRanking();
        }
        // --- FIM DA ADIÇÃO ---
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

// ==================================================================
// --- MÉTODOS DE RANKING ADICIONADOS ---
// ==================================================================

// Estrutura auxiliar para ordenação
struct RankingEntry {
    std::string nome;
    int pontuacao;
    // Overload do operador '<' para ordenar do maior para o menor
    bool operator < (const RankingEntry& other) const {
        return pontuacao > other.pontuacao;
    }
};

// Funções para serialização JSON da estrutura
void to_json(json& j, const RankingEntry& e) {
    j = json{ {"nome", e.nome}, {"pontuacao", e.pontuacao} };
}
void from_json(const json& j, RankingEntry& e) {
    j.at("nome").get_to(e.nome);
    j.at("pontuacao").get_to(e.pontuacao);
}


void Jogo::salvarPontuacao(const std::string& arquivo, const std::string& nome, int pontuacao)
{
    std::vector<RankingEntry> rankings;
    std::ifstream i(arquivo);
    json j;

    // 1. Tenta ler o arquivo de ranking existente
    if (i.is_open() && i.peek() != std::ifstream::traits_type::eof())
    {
        try {
            i >> j;
            rankings = j.get<std::vector<RankingEntry>>();
        }
        catch (json::exception& e) {
            std::cerr << "Erro ao ler JSON do ranking: " << e.what() << std::endl;
            i.close();
        }
    }
    i.close();

    // 2. Adiciona a nova pontuação
    rankings.push_back({ nome, pontuacao });

    // 3. Ordena o ranking (do maior para o menor)
    std::sort(rankings.begin(), rankings.end());

    // 4. (Opcional) Limita o ranking aos 10 melhores
    if (rankings.size() > 10) {
        rankings.resize(10);
    }

    // 5. Salva o ranking de volta no arquivo
    std::ofstream o(arquivo);
    if (!o.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir arquivo de ranking para salvar: " << arquivo << std::endl;
        return;
    }

    j = rankings; // Converte o vetor de volta para JSON
    o << std::setw(4) << j << std::endl; // Salva com formatação bonita
    o.close();
}


void Jogo::carregarRanking(const std::string& arquivo, const std::string& titulo)
{
    rankingTextos.clear();

    // Configura o título
    rankingTitulo.setString(titulo);
    sf::FloatRect boundsT = rankingTitulo.getLocalBounds();
    rankingTitulo.setOrigin(boundsT.left + boundsT.width / 2.0f, boundsT.top + boundsT.height / 2.0f);
    rankingTitulo.setPosition(LARGURA_TELA / 2.0f, 100.0f);

    std::vector<RankingEntry> rankings;
    std::ifstream i(arquivo);
    json j;

    // Tenta ler o arquivo
    if (i.is_open() && i.peek() != std::ifstream::traits_type::eof())
    {
        try {
            i >> j;
            rankings = j.get<std::vector<RankingEntry>>();
        }
        catch (json::exception& e) {
            std::cerr << "Erro ao carregar JSON do ranking: " << e.what() << std::endl;
        }
    }
    i.close();

    // Cria os objetos sf::Text para exibir
    if (rankings.empty()) {
        sf::Text texto("Nenhuma pontuacao registrada.", fonteUI, 30);
        texto.setFillColor(sf::Color::White);
        sf::FloatRect bounds = texto.getLocalBounds();
        texto.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        texto.setPosition(LARGURA_TELA / 2.0f, ALTURA_TELA / 2.0f);
        rankingTextos.push_back(texto);
    }
    else {
        float yPos = 200.0f;
        int rankNum = 1;
        for (const auto& entry : rankings)
        {
            std::string str = std::to_string(rankNum) + ". " + entry.nome + " - " + std::to_string(entry.pontuacao);
            sf::Text texto(str, fonteUI, 40);
            texto.setFillColor(sf::Color::White);
            sf::FloatRect bounds = texto.getLocalBounds();
            texto.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
            texto.setPosition(LARGURA_TELA / 2.0f, yPos);
            rankingTextos.push_back(texto);
            yPos += 50.0f;
            rankNum++;
        }
    }
}

void Jogo::processarInputRanking(sf::Event& evento)
{
    if (evento.type == sf::Event::KeyReleased)
    {
        if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Escape)
        {
            voltarAoMenu(); // Ação simples: apenas volta ao menu principal
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
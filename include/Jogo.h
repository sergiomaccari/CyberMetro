#pragma once
#include "Jogador.h"
#include "Menu.h" 


using json = nlohmann::json;

namespace Fases {
    class Fase;
}
namespace CyberMetro {
    class Jogo
    {
    private:
        CyberMetro::Gerenciadores::Gerenciador_Grafico gg;
        Menu menu;
        Entidades::Personagens::Jogador* pJogador1;
        Entidades::Personagens::Jogador* pJogador2;
        Fases::Fase* faseAtual;
        EstadoJogo estadoAtual;

        EstadoJogo estadoAnterior;

        sf::Font fontePausa;
        std::vector<sf::Text> opcoesPausa;
        int indicePausa;
        sf::RectangleShape fundoPausa;

        void carregarRecursosPausa();
        void processarInputPausa(sf::Event& evento);
        void desenharPausa();
        void salvarJogo(const std::string& arquivo);
        void carregarJogo(const std::string& arquivo);
        void criarEntidade(const json& data);
        void atualizarDestaquePausa();

        void configurarNovaFase(EstadoJogo novoEstado);
        void limparFaseAtual();

        sf::Font fonteUI;
        sf::Text textoPontosP1;
        sf::Text textoPontosP2;

        void carregarRecursosUI();
        void atualizarUI();
        void desenharUI();

        EstadoJogo proximaFaseAposNome;
        std::string nomeSaveFile;
        sf::Text textoInputNome;
        sf::Text textoPromptNome;
        sf::Clock cursorClock;
        bool cursorVisivel;

        void processarInputNome(sf::Event& evento);
        void atualizarInputNome();
        void desenharInputNome();


        std::vector<sf::Text> rankingTextos;
        sf::Text rankingTitulo;
        sf::Text rankingVoltar;
        std::string arquivoRankingAtual;

        std::vector<std::string> rankingNomes;
        std::vector<int> rankingPontuacoes;

        void salvarPontuacao(const std::string& arquivo, const std::string& nome, int pontuacao);
        void carregarRanking(const std::string& arquivo, const std::string& titulo);
        void processarInputRanking(sf::Event& evento);
        void desenharRanking();
        void sortRanking(std::vector<std::string>& nomes, std::vector<int>& pontuacoes);


    public:
        Jogo();
        ~Jogo();
        void executar();
        void voltarAoMenu();
    };
}
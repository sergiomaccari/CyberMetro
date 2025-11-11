#pragma once
#include "Ente.h" 

enum class EstadoJogo {
    Menu,
    Jogando,
    FaseDois,
    Sair
};


class Menu : public Ente
{
private:
    sf::Font fonte;

    enum class MenuNivel 
    { 
        PRINCIPAL, 
        FASE1, 
        FASE2 
    };
    MenuNivel nivelAtual;

    // opcoes
    std::vector<sf::Text> opcoesPrincipal;
    std::vector<sf::Text> opcoesFase1;
    std::vector<sf::Text> opcoesFase2;

    // para o vetor ativo
    std::vector<sf::Text>* pOpcoesAtuais;

    int indiceOpcaoSelecionada;

    const sf::Color COR_SELECIONADA = sf::Color::Yellow;
    const sf::Color COR_NORMAL = sf::Color::White;
    const int TAMANHO_FONTE = 40;

    EstadoJogo estadoAtualInterno;

    void carregarFonte();
    void configurarOpcoes();
    void desenharOpcoes();
    void processarEntrada(sf::Event& evento);

    // Funcoes auxiliares para o novo menu, acho que ajuda mas nao sao essenciais
    void atualizarDestaque();
    void setNivelMenu(MenuNivel novoNivel, int novoIndice = 0);
    void posicionarTextos(std::vector<sf::Text>& textos);

public:
    Menu();
    ~Menu();

    void inicializar();

    void moverCima();
    void moverBaixo();
    void executar() override;

    EstadoJogo getProximoEstado() const;
};
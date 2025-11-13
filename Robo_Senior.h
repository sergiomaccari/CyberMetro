#pragma once
#include "Inimigo.h"


namespace Personagens {
    class Robo_Senior : public Inimigo
    {
    private:
        int tamanho;
        sf::Clock tempoCura;
        sf::Time intervaloCura;
        bool estaNoChao;
        static const float ALTURA_PULO;
        static const float FORCA_PULO;

    protected:
        float xINI;

    public:
        Robo_Senior(float xi, float yi);
        ~Robo_Senior();
        void mover();
        void executar();
        void danificar(Jogador* pJogador);
        void salvar();
        void atualar();
    };
}
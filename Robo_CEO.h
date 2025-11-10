#pragma once
#include "Inimigo.h"
#include "Jogador.h" 
#include "ListaEntidades.h"
#include "Projetil.h"

namespace Personagens {
    class Robo_CEO : public Inimigo {
    private:
        int forca;
        Jogador* pJogador;
        ListaEntidades projeteis;
        const float PROJETIL_VELOCIDADE_CHEFE;
        const int COOLDOWN_TIRO_CHEFE;
        const float FORCA_GRAVIDADE_CHEFE;
        int cooldownTiro;

        void atirar();

    public:
        Robo_CEO(float xi, float yi);
        ~Robo_CEO();
        void mover();
        void executar();
        void danificar(Jogador* pJogador);
        void salvar();
        void setJogador(Jogador* j);
        ListaEntidades* getProjeteis();
    };
}
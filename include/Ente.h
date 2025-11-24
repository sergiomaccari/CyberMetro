#pragma once
#include "Gerenciador_Grafico.h"

namespace CyberMetro {
    namespace Gerenciadores {
        class Gerenciador_Grafico;
    }

    class Ente
    {
    protected:
        int id;
        static int cont_id;

        static Gerenciadores::Gerenciador_Grafico* pGG;
        sf::Sprite* pFigura;

        static sf::Clock g_clock;
        static float g_dt;
        static float g_tempoTotal;
        
    public:
        Ente();
        virtual ~Ente();

        virtual void executar() = 0;
        int getId() const;
        void desenhar();
        sf::Sprite* getFigura() const;

        static void setGerenciadorGrafico(Gerenciadores::Gerenciador_Grafico* g);
        static Gerenciadores::Gerenciador_Grafico* getGerenciadorGrafico();

        static void atualizarClockGlobal(bool pausado);

    };
}
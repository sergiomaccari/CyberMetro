
#include "Ente.h"

CyberMetro::Gerenciadores::Gerenciador_Grafico* CyberMetro::Ente::pGG = nullptr;//revisar
namespace CyberMetro {
    int Ente::cont_id = 0;
    sf::Clock Ente::g_clock;
    float Ente::g_dt = 0.0f;
    float Ente::g_tempoTotal = 0.0f;

    void Ente::atualizarClockGlobal(bool pausado)
    {
        if (pausado)
        {
            g_dt = 0.0f;
            g_clock.restart();
        }
        else
        {
            g_dt = g_clock.restart().asSeconds();
            g_tempoTotal += g_dt;
        }
    }


    Ente::Ente() : id(cont_id), pFigura(new sf::Sprite())
    {
        this->id = Ente::cont_id++;
    }

    Ente::~Ente()
    {
        if (pFigura)
        {
            delete pFigura;
            pFigura = nullptr;
        }
    }

    int Ente::getId() const
    {
        return id;
    }

    void Ente::setGerenciadorGrafico(Gerenciadores::Gerenciador_Grafico* g)
    {
        pGG = g;
    }

    Gerenciadores::Gerenciador_Grafico* Ente::getGerenciadorGrafico()
    {
        return pGG;
    }

    void Ente::desenhar()
    {
        Ente::pGG->desenharEnte(this);
    }

    sf::Sprite* Ente::getFigura() const
    {
        return pFigura;
    }
}
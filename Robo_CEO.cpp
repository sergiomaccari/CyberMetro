#include "Robo_CEO.h" 
#include "Projetil.h" 

namespace Personagens {

    Robo_CEO::Robo_CEO(float xi, float yi) :
        Inimigo(xi, yi),
        forca(rand() % 10 + 1),
        pJogador(nullptr),
        cooldownTiro(COOLDOWN_TIRO_CHEFE),
        PROJETIL_VELOCIDADE_CHEFE(1.2f + 0.06f * (float)forca),//analisar valores
        COOLDOWN_TIRO_CHEFE(540 - 120 / forca),
        FORCA_GRAVIDADE_CHEFE(-0.024999f)
    {
        this->x = xi;
        this->y = yi;
        this->velocidade = 150.0f;
        this->n_vidas = 10;

        if (pGG)
        {
            sf::Texture* tex = pGG->getTextura("Imagens/chefe.png");
            if (tex)
            {
                pFigura->setTexture(*tex);
                pFigura->setScale(64.0f / tex->getSize().x, 64.0f / tex->getSize().y);
            }
        }
        else
        {
            std::cerr << "Erro RoboCEO" << std::endl;
        }

        pFigura->setPosition(this->x, this->y);
    }

    Robo_CEO::~Robo_CEO()
    {

    }

    void Robo_CEO::setJogador(Jogador* j)
    {
        this->pJogador = j;
    }

    void Robo_CEO::mover()
    {
        sf::Vector2f movimento = sf::Vector2f(0.0f, 0.0f);
        tempo = clock.restart();

        vel_grav += grav + FORCA_GRAVIDADE_CHEFE;
        movimento += sf::Vector2f(0.0f, vel_grav);

        this->x += movimento.x * tempo.asSeconds() * velocidade;
        this->y += movimento.y * tempo.asSeconds() * velocidade;

        setPosicaoGrafica(this->x, this->y);
    }

    void Robo_CEO::atirar()
    {
        if (cooldownTiro > 0)
        {
            cooldownTiro--;
            return;
        }
        if (pJogador == nullptr)
        {
            return;
        }

        cooldownTiro = COOLDOWN_TIRO_CHEFE;

        Entidades::Projetil* p = new Entidades::Projetil();
        p->setGerenciadorGrafico(Ente::getGerenciadorGrafico());

        float centroX = this->x + (60.0f / 2.0f) - (10.0f / 2.0f); 
        float centroY = this->y + (60.0f / 2.0f) - (10.0f / 2.0f); 
        p->setX(centroX);
        p->setY(centroY);

        float dx = pJogador->getX() - this->x;
        float dy = pJogador->getY() - this->y;
        float mag = std::sqrt(dx * dx + dy * dy);

        float vx = 0.0f;
        float vy = 0.0f;

        if (mag > 0.0f)
        {
            vx = (dx / mag) * PROJETIL_VELOCIDADE_CHEFE;
            vy = (dy / mag) * PROJETIL_VELOCIDADE_CHEFE;
        }

        p->setVelocidade(vx, vy);
        p->setDoBem(false);
        projeteis.inserir(p);
    }

    void Robo_CEO::danificar(Jogador* pJogador)
    {
        if (pJogador)
        {
            for (int i = 0; i < this->nivel_maldade; i++)
                pJogador->operator--();
        }
    }

    void Robo_CEO::salvar()
    {
    }

    void Robo_CEO::executar()
    {
        this->mover();
        this->atirar();
        this->projeteis.executar();
    }

    ListaEntidades* Robo_CEO::getProjeteis()
    {
        return &projeteis;
    }
}
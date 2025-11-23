#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector3.hpp> 

#include <nlohmann/json.hpp>

#include <time.h>
#include <unordered_map> 
#include <string>        
#include <iostream>      
#include <filesystem>
#include <stdlib.h>



#define ALTURA_TELA 900
#define LARGURA_TELA 1600
#define MUNDO_X_MIN 0
#define MUNDO_X_MAX 3200
#define MUNDO_Y_MIN 0


namespace CyberMetro {

    class Ente;
    namespace Entidades
    {
        namespace Personagens
        {
            class Jogador;
            class Personagem;
        }

    }
    namespace Gerenciadores {
        class Gerenciador_Grafico
        {
        private:
            sf::RenderWindow* janela;
            sf::VideoMode videoMode;
            sf::Event ev;
            sf::RectangleShape rect;
            sf::CircleShape cir;
            sf::View camera;

            std::unordered_map<std::string, sf::Texture> texturas;

            sf::Texture texturaBackground;
            sf::Sprite spriteBackground;

            void iniciaJanela();


        public:
            Gerenciador_Grafico();
            ~Gerenciador_Grafico();

            sf::Texture* getTextura(std::string caminho);

            void desenharBackground();

            void desenharEnte(const Ente* pe);
            void update();
            void render();
            void clear();
            void setCamera(Entidades::Personagens::Jogador* pJog);

            const bool getJanelaOpen() const;
            sf::RenderWindow* getJanela() const;
        };
    }
}
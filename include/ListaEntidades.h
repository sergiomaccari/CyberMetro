#pragma once
#include "Lista.h"
#include "Entidade.h"
namespace CyberMetro {
    namespace Listas {
        class ListaEntidades
        {
        private:
            Lista<Entidades::Entidade> LEs;
        public:
            ListaEntidades();
            ~ListaEntidades();

            void inserir(Entidades::Entidade* e);
            void remover(Entidades::Entidade* e);
            std::size_t size() const;

            Lista<Entidades::Entidade>* getLista();
            typename Lista<Entidades::Entidade>::Elemento* getPrimeiro() const;

            void executar();
            void desenhar();
        };
    }
}
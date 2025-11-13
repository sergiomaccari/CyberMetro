#include "ListaEntidades.h"
#include <iostream>
#include "Projetil.h" 
#include "Lista.h" 
#include "Obstaculo.h"
#include "Chao.h"
using namespace Entidades;


ListaEntidades::ListaEntidades() {}

ListaEntidades::~ListaEntidades()
{
    Lista<Entidades::Entidade>::Elemento* atual = LEs.getPrimeiro();
    while (atual)
    {
        if (atual->getInfo())
        {
            delete atual->getInfo();
        }
        atual = atual->getProx();
    }
}

void ListaEntidades::inserir(Entidade* e)
{
    LEs.inserir(e);
}
void ListaEntidades::remover(Entidade* e)
{
    LEs.remover(e);
}
std::size_t ListaEntidades::size() const
{
    return LEs.size();
}

Lista<Entidades::Entidade>* ListaEntidades::getLista()
{
    return &LEs;
}

typename Lista<Entidades::Entidade>::Elemento* ListaEntidades::getPrimeiro() const
{
    return LEs.getPrimeiro();
}


void ListaEntidades::executar()
{
    Lista<Entidades::Entidade>::Elemento* atual = LEs.getPrimeiro();
    while (atual)
    {
        Entidade* pEnt = atual->getInfo();

        Lista<Entidades::Entidade>::Elemento* proximo = atual->getProx();
        if (pEnt->getAtivo() == false)
        {
            LEs.remover(pEnt);
            delete pEnt;
        }
        else
        {
            pEnt->executar();
        }

        atual = proximo;
    }
}

void ListaEntidades::desenhar()
{
    Lista<Entidades::Entidade>::Elemento* atual = LEs.getPrimeiro();
    while (atual)
    {
        Entidade* pEnt = atual->getInfo();
        if (pEnt && pEnt->getAtivo())
        {
            Entidades::Chao* pChao = dynamic_cast<Entidades::Chao*>(pEnt);
            if (!pChao)
            {
                pEnt->desenhar();
            }
        }
        atual = atual->getProx();
    }
}
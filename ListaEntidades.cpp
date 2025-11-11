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
    auto cur = LEs.getPrimeiro();
    while (cur)
    {
        if (cur->getInfo())
        {
            delete cur->getInfo();
        }
        cur = cur->getProx();
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
    auto cur = LEs.getPrimeiro();
    while (cur)
    {
        Entidade* pEnt = cur->getInfo();
        auto proximo = cur->getProx();
        if (pEnt->getAtivo() == false)
        {
            LEs.remover(pEnt);
            delete pEnt;
        }
        else
        {
            pEnt->executar();
        }

        cur = proximo;
    }
}

void ListaEntidades::desenhar()
{
    auto cur = LEs.getPrimeiro();
    while (cur)
    {
        Entidade* pEnt = cur->getInfo();
        if (pEnt && pEnt->getAtivo())
        {
            Entidades::Chao* pChao = dynamic_cast<Entidades::Chao*>(pEnt);
            if (!pChao)
            {
                pEnt->desenhar();
            }
        }
        cur = cur->getProx();
    }
}
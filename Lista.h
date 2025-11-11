#pragma once
#include <cstddef>
#include <stdexcept>


template<typename T>
class Lista
{
public:
    class Elemento
    {
    private:
        T* pInfo;
        Elemento* pProx;

    public:
        Elemento() : pInfo(nullptr), pProx(nullptr) {}
        ~Elemento() {}

        void incluir(T* p) { pInfo = p; }
        void setProx(Elemento* pe) 
        { 
            pProx = pe; 
        }
        Elemento* getProx() const 
        {
            return pProx; 
        }
        T* getInfo() const 
        {
            return pInfo; 
        }
    };

private:
    Elemento* pPrimeiro;
    Elemento* pUltimo;
    std::size_t tamanho;

public:
    Lista();
    ~Lista();

    Elemento* getPrimeiro() const
    {
        return pPrimeiro;
    }
    Elemento* getUltimo() const
    {
        return pUltimo;
    }

    void inserir(T* item);
    void remover(T* item);
    void limpar();
    std::size_t size() const;
    bool vazia() const;
};


template<typename T>
Lista<T>::Lista() : pPrimeiro(nullptr), pUltimo(nullptr), tamanho(0) {}

template<typename T>
Lista<T>::~Lista()
{
    limpar();
}

template<typename T>
void Lista<T>::limpar()
{
    typename Lista<T>::Elemento* cur = pPrimeiro;
    while (cur)
    {
        typename Lista<T>::Elemento* nxt = cur->getProx();
        delete cur;
        cur = nxt;
    }
    pPrimeiro = nullptr;
    pUltimo = nullptr;
    tamanho = 0;
}


template<typename T>
void Lista<T>::inserir(T* item)
{
    typename Lista<T>::Elemento* e = new typename Lista<T>::Elemento();
    e->incluir(item);

    if (!pPrimeiro)
    {
        pPrimeiro = pUltimo = e;
    }
    else
    {
        pUltimo->setProx(e);
        pUltimo = e;
    }
    ++tamanho;
}

template<typename T>
void Lista<T>::remover(T* item)
{
    typename Lista<T>::Elemento* prev = nullptr;
    typename Lista<T>::Elemento* cur = pPrimeiro;
    while (cur)
    {
        if (cur->getInfo() == item)
        {
            if (prev) prev->setProx(cur->getProx());
            else pPrimeiro = cur->getProx();
            if (cur == pUltimo) pUltimo = prev;
            delete cur;
            --tamanho;
            return;
        }
        prev = cur;
        cur = cur->getProx();
    }
}

template<typename T>
std::size_t Lista<T>::size() const
{
    return tamanho;
}

template<typename T>
bool Lista<T>::vazia() const
{
    return tamanho == 0;
}
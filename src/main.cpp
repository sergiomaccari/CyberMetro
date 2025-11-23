#include "Jogo.h"
#include <iostream>
using namespace CyberMetro;
int main()
{
	srand(time(NULL));

	try
	{
		Jogo meuJogo;
		meuJogo.executar();
	}
	catch (const std::bad_alloc& e)
	{
		std::cerr << "Falta de memoria" << std::endl;
		std::cerr << "Excecao: " << e.what() << std::endl;
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Ocorreu uma excecao" << std::endl;
		std::cerr << "Excecao: " << e.what() << std::endl;
		return 2;
	}
	catch (...)
	{
		std::cerr << "excecao diferente" << std::endl;
		return 3;
	}
	return 0;
}
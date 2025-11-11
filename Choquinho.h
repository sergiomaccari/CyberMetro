#pragma once
#include "Obstaculo.h"

using namespace Personagens;

namespace Obstaculos {
	class Choquinho : public Obstaculos::Obstaculo
	{
	private:
		short int danosidade;

	public:
		Choquinho(float xi, float yi);
		~Choquinho();
		virtual void obstaculizar(Jogador* pj);
		virtual void salvar();
		virtual void executar();
	};
}
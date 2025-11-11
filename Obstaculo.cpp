#include "Obstaculo.h"

namespace Obstaculos
{
	Obstaculo::Obstaculo(float xi, float yi) :
		danoso(false)
	{
		this->x = xi;
		this->y = yi;

	}
	Obstaculo::~Obstaculo() {}
	bool Obstaculo::getDanoso() const
	{
		return danoso;
	}

}
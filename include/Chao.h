#pragma once
#include "Entidade.h"


namespace CyberMetro {
	namespace Entidades {
		using json = nlohmann::json;

		class Chao : public Entidades::Entidade
		{
		private:
			static const float largura;
			static const float altura;

		public:
			Chao(float x, float y);
			~Chao();
			void executar() ;

			void salvarDataBuffer(json& arq);
			void carregarDeBuffer(const json& data) ;
		};
	}
}
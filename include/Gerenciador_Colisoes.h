#pragma once
#include <vector>
#include <list>
#include <set>

namespace CyberMetro {
	namespace Entidades {
		class Entidade;
		namespace Personagens {
			class Jogador;
			class Inimigo;
		}
		namespace Obstaculos {
			class Obstaculo;
		}
		class Projetil;
		class Chao;
	}

	namespace Gerenciadores
	{
		class Gerenciador_Colisoes
		{
		private:
			Entidades::Personagens::Jogador* p1;
			Entidades::Personagens::Jogador* p2;

			std::vector<Entidades::Personagens::Inimigo*> LIs;
			std::list<Entidades::Obstaculos::Obstaculo*> LOs;
			std::list<Entidades::Chao*> LCs;
			std::set<Entidades::Projetil*> LPs;
			static const float RAIO_CHEKAGEM_QUADRADO;

			const bool verificarColisao(Entidades::Entidade* pe1, Entidades::Entidade* pe2) const;
			void tratarColisoesJogsObstacs();
			void tratarColisoesInimgsObstacs();
			void tratarColisoesProjeteisObstacs();
			void tratarColisoesObstacsObstacs();
			void tratarColisoesJogsInimgs();
			void tratarColisoesJogsProjeteis();
			void tratarColisoesInimgsProjeteis();
			void tratarColisoesInimgsInimgs();
			void tratarColisoesJogsChao();
			void tratarColisoesInimgsChao();
			void tratarColisoesProjeteisChao();
			void tratarColisoesObstacsChao();


		public:
			Gerenciador_Colisoes(Entidades::Personagens::Jogador* p1);
			~Gerenciador_Colisoes();

			void setJogador2(Entidades::Personagens::Jogador* j2);

			void incluirInimigo(Entidades::Personagens::Inimigo* pi);
			void incluirObstaculo(Entidades::Obstaculos::Obstaculo* po);
			void incluirProjetil(Entidades::Projetil* pj);
			void incluirChao(Entidades::Chao* pc);

			void limparProjeteis();
			void limparInimigos();
			void limparObstaculos();
			void limparChao();

			void executar();
		};
	}
}
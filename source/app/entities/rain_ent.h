#pragma once
#include "engine/entity/entity.h"
#include "entity/physics_ent.h"

namespace app
{
	namespace entity
	{
		class RainEnt : public baka::entity::IPhysicsEnt
		{
		public:
			DECLARE_ENTITY_TYPE_INFO(RainEnt);

		private:
			b2ParticleSystem* m_pParticleSystem;

			float minX;
			float maxX;
			float altY;
			sf::Time m_timer;

			b2ParticleGroupDef pd;

		public:
			RainEnt();

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

			virtual void Setup(b2Body* pBody, b2dJson* pJson);

			void SetPosY(const float f) { altY = f; }
		};
	}
}
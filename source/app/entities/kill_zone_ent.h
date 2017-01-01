#pragma once
#include "engine/entity/entity.h"
#include "entity/physics_ent.h"

namespace app
{
	namespace entity
	{
		//forward declare
		class PlayerEnt;

		class KillZoneEnt : public baka::entity::IPhysicsEnt
		{
		public:
			DECLARE_ENTITY_TYPE_INFO(KillZoneEnt);
			static util::Publisher<PlayerEnt*> s_OutOfBounds;

		private:
			b2Body* m_pBody;
			b2Fixture* m_pSensor;

			void OnContactBegin(b2Contact*);
			void OnContactEnd(b2Contact*);

		public:
			KillZoneEnt();

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

			virtual void Setup(b2Body* pBody, b2dJson* pJson);

		};
	}
}
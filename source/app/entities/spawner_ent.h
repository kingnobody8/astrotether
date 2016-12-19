#pragma once
#include "player_ent.h"


namespace app
{
	namespace entity
	{
		class SpawnerEnt : public baka::entity::IPhysicsEnt
		{
		public:
			DECLARE_ENTITY_TYPE_INFO(SpawnerEnt);

		public:
			SpawnerEnt();

			virtual void Setup(b2Body* pBody, b2dJson* pJson);

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

		};
	}
}
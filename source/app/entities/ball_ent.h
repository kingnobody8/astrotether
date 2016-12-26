#pragma once
#include "engine/entity/entity.h"
#include "entity/physics_ent.h"

namespace app
{
	namespace entity
	{
		class BallEnt : public baka::entity::IPhysicsEnt
		{
		public:
			DECLARE_ENTITY_TYPE_INFO(BallEnt);

		private:
			b2Body* m_pBody;
			void OnContactBegin(b2Contact*);
			void OnContactEnd(b2Contact*);

		public:
			BallEnt();

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

			virtual void Setup(b2Body* pBody, b2dJson* pJson);

		};
	}
}
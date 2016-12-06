#pragma once
#include "SFML/Graphics.hpp"
#include "engine/entity/entity.h"
#include "spinecpp/spinecpp.h"
#include "render/spine-sfml.h"
#include "physics/callbacks/aabb_callback.h"

namespace app
{
	namespace entity
	{
		class TetherEnt : public baka::entity::IEntity
		{
		private:
			b2Body* m_pPlayerBody;
			b2Body* m_pObjectBody;
			b2Vec2 m_startLocalPoint;
			b2Vec2 m_endLocalPoint;

			b2RopeJoint* m_pRopeJoint;


		public:
			TetherEnt(b2Body* pPlayerBody, b2Body* pObjectBody, b2Vec2 startPoint, b2Vec2 endPoint);
			virtual ~TetherEnt();

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

		};
	}
}
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
		struct RopeSegment
		{
			void Destroy();
			void Update(const sf::Time& dt);

			sf::RectangleShape* m_pDrawable;
			b2RopeJoint* m_pRopeJoint;
			b2Vec2 m_prevStart;
			b2Vec2 m_prevEnd;
			float m_fMaxAngle;
		};

		class TetherEnt : public baka::entity::IEntity
		{
		public:
			DECLARE_ENTITY_TYPE_INFO(TetherEnt);


		private:
			b2Body* m_pPlayerBody;
			b2Body* m_pObjectBody;
			b2Vec2 m_startLocalPoint;
			b2Vec2 m_endLocalPoint;

			//b2RopeJoint* m_pRopeJoint;
			//sf::RectangleShape m_drawableRect;
			std::vector<RopeSegment*> m_vRopeSegments;

			void CreateTether(b2Body* pBodyA, b2Body* pBodyB, b2Vec2 localPointA, b2Vec2 localPointB);
			void CheckSegment(int& index, RopeSegment* alpha);

		public:
			TetherEnt();
			virtual ~TetherEnt();

			void Setup(b2Body* pPlayerBody, b2Body* pObjectBody, b2Vec2 startPoint, b2Vec2 endPoint);

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

		};
	}
}
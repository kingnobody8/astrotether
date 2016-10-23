#pragma once
#include "plugin.h"
#include "box2d/Box2D.h"
#include "debug_draw.h"
#include "../input/input_event.h"
#include "../b2djson/b2dJson.h"

namespace baka
{
	namespace physics
	{
		class PhysicsPlugin : public IPlugin
		{
		public:
			DECLARE_PLUGIN_TYPE_INFO(PhysicsPlugin);

			/*enum EInputState { TESTBED, LARK, };
			enum ELarkClick { PUSH, LINE, BLAST, COUNT };*/

		private:
			b2dJson m_json;
			b2World* m_pWorld;
			//DebugDraw m_debugDraw;
			util::Subscriber m_sub;
			b2MouseJoint* m_pMouseJoint;
			b2Body* m_pGroundBody;
			bool m_bLeftMouseBtn;
			bool m_bRightMouseBtn;
			b2Vec2 lastp;
			b2ParticleSystem* m_pParticleSystem;
			bool m_bIsTestbed;
			b2PolygonShape m_pParticleEmitterShape;
			b2Vec2 m_pParticleEmitterPosition;
			float angle;
			void MakeParticles(int count, int lifetime, b2Vec2 dir);

		public:
			PhysicsPlugin();
			virtual ~PhysicsPlugin();

			virtual void Init();
			virtual void Exit();
			virtual bool Update(const util::Time& dt);
		};
	}
}
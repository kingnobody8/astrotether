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

		private:
			b2dJson m_json;
			b2World* m_pWorld;
			DebugDraw m_debugDraw;
			sf::View m_view;
			int steps;

		public:
			PhysicsPlugin();
			virtual ~PhysicsPlugin();
			virtual const EPluginOrder GetPriority() const { return EPO_PHYSICS; }


			virtual void Init();
			virtual void Exit();
			virtual bool Update(const sf::Time& dt);
			virtual const std::string DebugRender(sf::RenderWindow* pRenWin);


			const b2dJson& LoadWorld(const std::string filepath);
		};
	}
}
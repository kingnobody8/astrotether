#pragma once
#include "state/base_state.h"

#include "utility/event/subscriber.h"
#include "input/input_event.h"

#include "spinecpp/spinecpp.h"
#include "render/spine-sfml.h"

#include "physics/physics_plugin.h"
#include "entities/player_ent.h"


namespace app
{
	namespace state
	{
		class TestbedState : public baka::state::IBaseState
		{
		private:
			util::Subscriber sub;
			baka::physics::PhysicsPlugin* m_pPhysicsPlugin;
			entity::PlayerEnt* m_pPlayer;

			sf::Vertex shooter[2];

		public:
			TestbedState();
			virtual ~TestbedState();

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

			virtual const std::string DebugRender(sf::RenderWindow* pRenWin);
		};
	}
}
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
		class KnockOffState : public baka::state::IBaseState
		{
		private:
			util::Subscriber sub;
			baka::physics::PhysicsPlugin* m_pPhysicsPlugin;
			entity::PlayerEnt* m_vPlayer[2];

		public:
			KnockOffState();

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

			virtual const std::string DebugRender(sf::RenderWindow* pRenWin);
		};
	}
}
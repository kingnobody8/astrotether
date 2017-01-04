#pragma once
#include "state/base_state.h"

#include "utility/event/subscriber.h"
#include "input/input_event.h"

#include "spinecpp/spinecpp.h"
#include "render/spine-sfml.h"

#include "physics/physics_plugin.h"
#include "entities/player_ent.h"
#include "entities/logo_ent.h"
#include "entities/rain_ent.h"


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

			sf::Time m_timer;
			entity::LogoEnt* m_pLogo;
			entity::RainEnt* m_pRain;

			void BeginPan();

			void OnJoystickConnected(const int id);
			void OnJoystickDisconnected(const int id);
			void OnKeyUp(const baka::key_events::KeyAction& action);

			void FindControllers();

		public:
			KnockOffState();

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

			virtual const std::string DebugRender(sf::RenderWindow* pRenWin);
		};
	}
}
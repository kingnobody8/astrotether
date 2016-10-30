#pragma once
#include "state/base_state.h"

#include "utility/event/subscriber.h"
#include "input/input_event.h"

#include "spinecpp/spinecpp.h"
#include "render/spine-sfml.h"
#include "app/entities/logo_ent.h"

namespace app
{
	namespace state
	{
		class SplashState : public baka::state::IBaseState
		{
		private:
			entity::LogoEnt m_logoEnt;

		public:
			SplashState();
			virtual ~SplashState();

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

			virtual const std::string DebugRender(sf::RenderWindow* pRenWin);
		};
	}
}
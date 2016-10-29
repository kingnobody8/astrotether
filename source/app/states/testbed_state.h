#pragma once
#include "state/base_state.h"

#include "utility/event/subscriber.h"
#include "input/input_event.h"

#include "spinecpp/spinecpp.h"
#include "render/spine-sfml.h"

namespace app
{
	namespace state
	{
		class TestbedState : public baka::state::IBaseState
		{
		private:
			util::Subscriber sub;

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
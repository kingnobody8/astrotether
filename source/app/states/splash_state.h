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
		public:
			SplashState();

			virtual void Init();
			virtual void Exit();
		};
	}
}
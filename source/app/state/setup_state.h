#pragma once
#include "engine/state/base_state.h"

namespace app
{
	namespace state
	{
		class SetupState : public engine::state::IBaseState
		{
		private:

		public:
			SetupState();
			virtual ~SetupState();

			virtual void Init();
			virtual void Exit();
		};
	}
}
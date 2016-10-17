#pragma once
#include "plugin.h"
#include "base_state.h"

namespace baka
{
	namespace state
	{
		class StatePlugin : public IPlugin
		{
		public:
			DECLARE_PLUGIN_TYPE_INFO(StatePlugin);

		private:
			IBaseState* m_pCurrState;
			IBaseState* m_pNextState;

		private:
			void FlushState();

		public:
			StatePlugin();
			virtual ~StatePlugin();

			virtual void Init();
			virtual void Exit();
			virtual bool Update(const util::Time& dt);

			void TransitionState(IBaseState* const pNextState);
		};
	}
}
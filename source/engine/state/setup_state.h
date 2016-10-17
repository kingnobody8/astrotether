#pragma once
#include "base_state.h"

namespace baka
{
	namespace state
	{
		class SetupState : public IBaseState
		{
		private:
			IBaseState* m_pTransitionState;

		public:
			SetupState();
			virtual ~SetupState();

			virtual void Init();
			virtual void Exit();

			void SetTransitionState(IBaseState* const pTransitionState) { m_pTransitionState = pTransitionState; }
		};
	}
}
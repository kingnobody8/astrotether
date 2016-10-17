#pragma once
#include "base_state.h"

#include "utility/event/subscriber.h"
#include "input/input_event.h"

namespace baka
{
	namespace state
	{
		class SetupState : public IBaseState
		{
		private:
			IBaseState* m_pTransitionState;
			util::Subscriber sub;

		public:
			SetupState();
			virtual ~SetupState();

			virtual void Init();
			virtual void Exit();

			virtual const std::string DebugRender(const sf::RenderWindow* pRenWin);

			void SetTransitionState(IBaseState* const pTransitionState) { m_pTransitionState = pTransitionState; }

			void OnScroll( const mouse_events::WheelAction& action);
		};
	}
}
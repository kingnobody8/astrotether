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
			sf::Texture m_texture;
			sf::Sprite m_sprite;

			sf::CircleShape m_shape;

		public:
			SetupState();
			virtual ~SetupState();

			virtual void Init();
			virtual void Exit();

			virtual const std::string DebugRender(sf::RenderWindow* pRenWin);

			void SetTransitionState(IBaseState* const pTransitionState) { m_pTransitionState = pTransitionState; }

			void OnMove( const mouse_events::MotionAction& action);
		};
	}
}
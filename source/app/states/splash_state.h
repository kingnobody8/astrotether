#pragma once
#include "state/base_state.h"

#include "utility/event/subscriber.h"
#include "input/input_event.h"

#include "spinecpp/spinecpp.h"

namespace app
{
	namespace state
	{
		class SplashState : public baka::state::IBaseState
		{
		private:
			util::Subscriber sub;
			sf::Texture m_texture;
			sf::Sprite m_sprite;

			sf::CircleShape m_shape;

		public:
			SplashState();
			virtual ~SplashState();

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

			virtual const std::string DebugRender(sf::RenderWindow* pRenWin);

			void OnMove( const baka::mouse_events::MotionAction& action);
		};
	}
}
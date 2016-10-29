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
		class SplashState : public baka::state::IBaseState
		{
		private:
			util::Subscriber sub;

			spine::Atlas* atlas;
			spine::Skeleton* skel;
			spine::AnimationStateData* stateData;
			spine::SkeletonDrawable* draw;
			spine::SkeletonData* data;

			bool m_bAnimOver;
			sf::Time m_counter;
			void AnimationListenerCallback(spine::AnimationState& state, int trackIndex, spine::EventType type, const spine::Event* event, int loopCount);
			void OnKeyUp(const baka::key_events::KeyAction& action);

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
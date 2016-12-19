#pragma once
#include "SFML/Graphics.hpp"
#include "engine/entity/entity.h"
#include "input/input_event.h"
#include "spinecpp/spinecpp.h"
#include "render/spine-sfml.h"

namespace app
{
	namespace entity
	{
		class LogoEnt : public baka::entity::IEntity
		{
		public:
			DECLARE_ENTITY_TYPE_INFO(LogoEnt);

		private:
			spine::Atlas* m_pAtlas;
			spine::SkeletonData* m_pSkelData;
			spine::SkeletonDrawable* m_pDrawable;

			sf::Time m_timer;
			bool m_bIsAnimOver;

			void OnKeyDown(const baka::key_events::KeyAction& action);
			void OnMouseDown(const baka::mouse_events::ButtonAction& action);
			void OnJoystickButtonDown(const baka::joypad_events::ButtonAction& action);
			void SnapToEnd();
			void AnimationListenerCallback(spine::AnimationState& state, int trackIndex, spine::EventType type, const spine::Event* event, int loopCount);
			void GotoNextState();

		public:
			LogoEnt();
			virtual ~LogoEnt();

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

		};
	}
}
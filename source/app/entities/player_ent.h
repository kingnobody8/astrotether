#pragma once
#include "SFML/Graphics.hpp"
#include "engine/entity/entity.h"
#include "input/input_event.h"
#include "spinecpp/spinecpp.h"
#include "render/spine-sfml.h"
#include "physics/callbacks/aabb_callback.h"
#include "utility/types/input.h"

namespace app
{
	namespace entity
	{
		class PlayerEnt : public baka::entity::IEntity
		{
			enum EButton { EB_INVALID = -1, EB_LEFT, EB_RIGHT, EB_JUMP, EB_COUNT };

		private:
			b2Body* m_pBody;
			b2Fixture* m_pGroundSensor;
			util::_Key m_vButtons[EButton::EB_COUNT];

			spine::Atlas* m_pAtlas;
			spine::SkeletonData* m_pSkelData;
			spine::SkeletonDrawable* m_pDrawable;

		private:
			void OnKeyDown(const baka::key_events::KeyAction& action);
			void OnKeyUp(const baka::key_events::KeyAction& action);
			void OnMouseDown(const baka::mouse_events::ButtonAction& action);
			void OnMouseUp(const baka::mouse_events::ButtonAction& action);
			void OnMouseMove(const baka::mouse_events::MotionAction& action);

			bool CheckGrounded();

		public:
			PlayerEnt(b2Body* pBody);
			virtual ~PlayerEnt();

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

			sf::Vector2f GetPosition() const { return sf::Vector2f(m_pBody->GetPosition().x, m_pBody->GetPosition().y); }

		};
	}
}
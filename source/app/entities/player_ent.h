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
		struct PlayerValue
		{
			PlayerValue()
				: m_fMaxSpeed(0)
				, m_fGroundAcceleration(0)
				, m_fGroundDeceleration(0)
				, m_fAirAcceleration(0)
				, m_fAirDeceleration(0)
				, m_fJumpImpulse(0)
			{
			}
			void LoadValues(const std::string& file_path);
			const std::string GetAsString() const;

			float m_fMaxSpeed;
			float m_fGroundAcceleration;
			float m_fAirAcceleration;
			float m_fGroundDeceleration;
			float m_fAirDeceleration;
			float m_fJumpImpulse;
		};

		class PlayerEnt : public baka::entity::IEntity
		{
			enum EButton { EB_INVALID = -1, EB_LEFT, EB_RIGHT, EB_JUMP, EB_COUNT };

		private:
			b2Body* m_pBody;
			b2Joint* m_pRopeJoint;
			b2Fixture* m_pGroundSensor;
			util::_Key m_vButtons[EButton::EB_COUNT];
			PlayerValue m_tValue;

			spine::Atlas* m_pAtlas;
			spine::SkeletonData* m_pSkelData;
			spine::SkeletonDrawable* m_pDrawable;

			std::vector<b2Contact*> m_vGroundContacts;
			bool m_bGrounded;

		private:
			void OnKeyDown(const baka::key_events::KeyAction& action);
			void OnKeyUp(const baka::key_events::KeyAction& action);
			void OnMouseDown(const baka::mouse_events::ButtonAction& action);
			void OnMouseUp(const baka::mouse_events::ButtonAction& action);
			void OnMouseMove(const baka::mouse_events::MotionAction& action);

			void OnRopeEvent(const sf::Vector2i& screenPos);

			void OnContactBegin(b2Contact*);
			void OnContactEnd(b2Contact*);

		public:
			PlayerEnt(b2Body* pBody);
			virtual ~PlayerEnt();

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

			sf::Vector2f GetPosition() const { return sf::Vector2f(m_pBody->GetPosition().x, m_pBody->GetPosition().y); }
			const PlayerValue& GetPlayerValue() const { return m_tValue; }
			b2Body* GetBody() const { return m_pBody; }
			const bool& IsGrounded() const { return m_bGrounded; }
		};
	}
}
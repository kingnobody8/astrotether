#pragma once
#include "SFML/Graphics.hpp"
#include "engine/entity/entity.h"
#include "input/input_event.h"
#include "spinecpp/spinecpp.h"
#include "render/spine-sfml.h"
#include "physics/callbacks/aabb_callback.h"
#include "utility/types/input.h"
#include "entity/physics_ent.h"

namespace app
{
	namespace entity
	{
		//forward declare
		class TetherEnt;

		struct PlayerValue
		{
			PlayerValue()
				: m_fMaxSpeed(0)
				, m_fGroundAcceleration(0)
				, m_fGroundDeceleration(0)
				, m_fAirAcceleration(0)
				, m_fAirDeceleration(0)
				, m_fJumpSpeed(0)
				, m_fJumpTime(0)
				, m_fFlipTime(0)
				, m_fDashImpulse(0)
				, m_fTetherLength(0)
				, m_fTetherAngle(0)
				, m_fTongueTipRadius(0)
				, m_fTongueTipDensity(0)
				, m_fTongueTipSpeed(0)
			{
			}
			void LoadValues(const std::string& file_path);
			const std::string GetAsString() const;

			float m_fMaxSpeed;
			float m_fGroundAcceleration;
			float m_fAirAcceleration;
			float m_fGroundDeceleration;
			float m_fAirDeceleration;
			float m_fJumpSpeed;
			float m_fJumpTime;
			float m_fFlipTime;
			float m_fDashImpulse;
			float m_fTetherLength;
			float m_fTetherAngle;
			float m_fTongueTipRadius;
			float m_fTongueTipDensity;
			float m_fTongueTipSpeed;
		};

		class PlayerEnt : public baka::entity::IPhysicsEnt
		{
		public:
			DECLARE_ENTITY_TYPE_INFO(PlayerEnt);

			enum EButton { EB_INVALID = -1, EB_LEFT, EB_RIGHT, EB_UP, EB_DOWN, EB_JUMP, EB_SHOOT, EB_LEFT_FLIP, EB_RIGHT_FLIP, EB_DASH, EB_COUNT };

		private:
			b2Body* m_pBody;
			b2Joint* m_pRopeJoint;
			TetherEnt* m_pTetherEnt;
			b2Fixture* m_pGroundSensor;
			util::_Key m_vButtons[EButton::EB_COUNT];
			PlayerValue m_tValue;
			b2Body* m_pTongueTip;
			int m_nPlayerId;

			spine::Atlas* m_pAtlas;
			spine::SkeletonData* m_pSkelData;
			spine::SkeletonDrawable* m_pDrawable;
			spine::AnimationStateData* m_pStateData;

			std::vector<b2Body*> m_vChain;
			std::vector<b2Contact*> m_vGroundContacts;
			std::vector<b2Contact*> m_vTongueContacts;
			b2Contact* m_pTongueContactUse;
			b2Vec2 m_TongueLocalCoords;
			bool m_bGrounded;
			float m_fJumpTime;
			float m_fFlipTime;

		private:
			void OnKeyDown(const baka::key_events::KeyAction& action);
			void OnKeyUp(const baka::key_events::KeyAction& action);
			void OnMouseDown(const baka::mouse_events::ButtonAction& action);
			void OnMouseUp(const baka::mouse_events::ButtonAction& action);
			void OnMouseMove(const baka::mouse_events::MotionAction& action);
			void OnJoypadButtonDown(const baka::joypad_events::ButtonAction& action);
			void OnJoypadButtonUp(const baka::joypad_events::ButtonAction& action);
			void OnJoypadMove(const baka::joypad_events::AxisAction& action);


			bool OnRopeEvent(const sf::Vector2f& woorldCoords);
			void Shoot();
			void Dash();
			void MakeRopeJoint(b2Body* pBody, b2Vec2 worldPoint);
			bool DestroyChain(void);

			void OnContactBegin(b2Contact*);
			void OnContactEnd(b2Contact*);

		public:
			PlayerEnt();
			virtual ~PlayerEnt();

			virtual void Setup(b2Body* pBody, b2dJson* pJson);

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

			sf::Vector2f GetPosition() const { return sf::Vector2f(m_pBody->GetPosition().x, m_pBody->GetPosition().y); }
			const PlayerValue& GetPlayerValue() const { return m_tValue; }
			b2Body* GetBody() const { return m_pBody; }
			const bool& IsGrounded() const { return m_bGrounded; }
			const b2Vec2 CalcShootDirection() const;
		};
	}
}
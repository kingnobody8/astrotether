#pragma once
#include "engine/entity/entity.h"
#include "entity/physics_ent.h"
#include "render/box2d-sfml.h"
#include "SFML/Audio.hpp"

namespace app
{
	namespace entity
	{
		class BlockEnt : public baka::entity::IPhysicsEnt
		{
		public:
			DECLARE_ENTITY_TYPE_INFO(BlockEnt);

		private:
			b2Body* m_pBody;
			baka::render::PhysicsDrawable* m_pDrawableR;
			baka::render::PhysicsDrawable* m_pDrawableG;
			int m_nPlayerId;
			bool m_bRespawn;
			bool m_bInGoal;
			bool m_bIsGreen;

			sf::SoundBuffer m_soundBufferSettled;
			sf::Sound m_sound;

			void SetToGreen(bool b);

		public:
			BlockEnt();

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

			virtual void Setup(b2Body* pBody, b2dJson* pJson);

			void Explode(const b2Vec2 v);
			void Respawn();
			bool IsSettled() { return m_pBody->GetLinearVelocity().Length() < 0.15f && m_pBody->GetAngularVelocity() < 0.1f; }
			void SetInGoal(bool b) { m_bInGoal = b; }

		};
	}
}
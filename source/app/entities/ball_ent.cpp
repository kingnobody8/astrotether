#pragma once
#include "ball_ent.h"
#include "utility/helper/func.h"
#include "physics/physics_plugin.h"

namespace app
{
	namespace entity
	{
		DEFINE_ENTITY_TYPE_INFO(BallEnt);

		BallEnt::BallEnt()
			: m_pBody(null)
		{
		}

		VIRTUAL void BallEnt::Setup(b2Body* pBody, b2dJson* pJson)
		{
			m_pBody = pBody;
		}

		VIRTUAL void BallEnt::Init()
		{
			baka::physics::contact_events::s_ContactBegin.Subscribe(this, BIND1(this, &BallEnt::OnContactBegin));
			baka::physics::contact_events::s_ContactEnd.Subscribe(this, BIND1(this, &BallEnt::OnContactEnd));
		}

		VIRTUAL void BallEnt::Exit()
		{
		}

		VIRTUAL void BallEnt::Update(const sf::Time& dt)
		{
		}

		void BallEnt::OnContactBegin(b2Contact* contact)
		{
		}

		void BallEnt::OnContactEnd(b2Contact* contact)
		{
		}

	}
}
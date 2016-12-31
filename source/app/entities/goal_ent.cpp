#pragma once
#include "goal_ent.h"
#include "utility/helper/func.h"
#include "physics/physics_plugin.h"
#include "ball_ent.h"

namespace app
{
	namespace entity
	{
		DEFINE_ENTITY_TYPE_INFO(GoalEnt);

		STATIC util::Publisher<int> GoalEnt::s_ScoreGoal;


		GoalEnt::GoalEnt()
			: m_pBody(null)
			, m_bScored(false)
			, m_nGoalId(-1)
		{
		}

		VIRTUAL void GoalEnt::Setup(b2Body* pBody, b2dJson* pJson)
		{
			m_pBody = pBody;

			b2Fixture* pFixture = pBody->GetFixtureList();
			while (pFixture != null)
			{
				if (pFixture->IsSensor())
				{
					m_pSensor = pFixture;
					break;
				}

				pFixture = pFixture->GetNext();
			}

			m_nGoalId = pJson->getCustomInt(pBody, "intData1", -1);
		}

		VIRTUAL void GoalEnt::Init()
		{
			m_bScored = false;
			baka::physics::contact_events::s_ContactBegin.Subscribe(this, BIND1(this, &GoalEnt::OnContactBegin));
			baka::physics::contact_events::s_ContactEnd.Subscribe(this, BIND1(this, &GoalEnt::OnContactEnd));
		}

		VIRTUAL void GoalEnt::Exit()
		{
		}

		VIRTUAL void GoalEnt::Update(const sf::Time& dt)
		{
			if (m_bScored && m_pBody->GetType() != b2BodyType::b2_staticBody)
			{
				m_pBody->SetType(b2BodyType::b2_staticBody);
			}
		}

		void GoalEnt::OnContactBegin(b2Contact* contact)
		{
			IPhysicsEnt* data = null;
			if (contact->GetFixtureA() == m_pSensor)
			{
				data = static_cast<IPhysicsEnt*>(contact->GetFixtureB()->GetBody()->GetUserData());
			}
			else if (contact->GetFixtureB() == m_pSensor)
			{
				data = static_cast<IPhysicsEnt*>(contact->GetFixtureA()->GetBody()->GetUserData());
			}
			if (data && data->GetType() == BallEnt::Type)
			{
				m_bScored = true;
				s_ScoreGoal.Publish(m_nGoalId);
			}
		}

		void GoalEnt::OnContactEnd(b2Contact* contact)
		{
		}

	}
}
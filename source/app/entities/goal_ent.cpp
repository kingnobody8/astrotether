#pragma once
#include "goal_ent.h"
#include "utility/helper/func.h"
#include "physics/physics_plugin.h"
#include "ball_ent.h"
#include "block_ent.h"

namespace app
{
	namespace entity
	{
		DEFINE_ENTITY_TYPE_INFO(GoalEnt);

		STATIC util::Publisher<int> GoalEnt::s_ScoreGoal;


		GoalEnt::GoalEnt()
			: m_pBody(null)
			, m_bScored(false)
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
		}

		VIRTUAL void GoalEnt::Init()
		{
			m_bScored = true;
			baka::physics::contact_events::s_ContactBegin.Subscribe(this, BIND1(this, &GoalEnt::OnContactBegin));
			baka::physics::contact_events::s_ContactEnd.Subscribe(this, BIND1(this, &GoalEnt::OnContactEnd));
		}

		VIRTUAL void GoalEnt::Exit()
		{
		}

		VIRTUAL void GoalEnt::Update(const sf::Time& dt)
		{
			if (!m_bScored)
				CheckBlocks();


		}

		void GoalEnt::CheckBlocks()
		{
			IPhysicsEnt* data = null;
			std::vector<BlockEnt*> m_vBlocks;
			for (int i = 0; i < m_vContacts.size(); ++i)
			{
				if (m_vContacts[i]->GetFixtureA() == m_pSensor)
				{
					data = static_cast<IPhysicsEnt*>(m_vContacts[i]->GetFixtureB()->GetBody()->GetUserData());
				}
				else
				{
					data = static_cast<IPhysicsEnt*>(m_vContacts[i]->GetFixtureA()->GetBody()->GetUserData());
				}
				if (data && data->GetTypeName() == "BlockEnt")
				{
					m_vBlocks.push_back(static_cast<BlockEnt*>(data));
					data == null;
				}
			}

			if (m_vBlocks.size() == 12)
			{
				bool b = true;
				for (int i = 0; i < m_vBlocks.size(); ++i)
				{
					if (!m_vBlocks[i]->IsSettled())
					{
						b = false;
						break;
					}
				}

				if (b)
				{
					m_bScored = true;
					s_ScoreGoal.Publish(0);
				}

			}
		}

		void GoalEnt::OnContactBegin(b2Contact* contact)
		{
			IPhysicsEnt* data = null;
			if (contact->GetFixtureA() == m_pSensor || contact->GetFixtureA() == m_pSensor)
			{
				if (contact->GetFixtureA() == m_pSensor)
				{
					data = static_cast<IPhysicsEnt*>(contact->GetFixtureB()->GetBody()->GetUserData());
				}
				else
				{
					data = static_cast<IPhysicsEnt*>(contact->GetFixtureA()->GetBody()->GetUserData());
				}
				if (data && data->GetTypeName() == "BlockEnt")
				{
					static_cast<BlockEnt*>(data)->SetInGoal(true);
					m_vContacts.push_back(contact);
				}

			}
			/*else if (contact->GetFixtureB() == m_pSensor)
			{
				data = static_cast<IPhysicsEnt*>(contact->GetFixtureA()->GetBody()->GetUserData());
			}
			if (data && data->GetType() == BallEnt::Type)
			{
				m_bScored = true;
				s_ScoreGoal.Publish(m_nGoalId);
			}*/
		}

		void GoalEnt::OnContactEnd(b2Contact* contact)
		{
			for (int i = 0; i < m_vContacts.size(); ++i)
			{
				if (m_vContacts[i] == contact)
				{
					IPhysicsEnt* data = null;
					if (contact->GetFixtureA() == m_pSensor)
					{
						data = static_cast<IPhysicsEnt*>(contact->GetFixtureB()->GetBody()->GetUserData());
					}
					else
					{
						data = static_cast<IPhysicsEnt*>(contact->GetFixtureA()->GetBody()->GetUserData());
					}
					if (data && data->GetTypeName() == "BlockEnt")
					{
						static_cast<BlockEnt*>(data)->SetInGoal(false);
					}

					m_vContacts.erase(m_vContacts.begin() + i);
					break;
				}
			}
		}

	}
}
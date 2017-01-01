#pragma once
#include "kill_zone_ent.h"
#include "utility/helper/func.h"
#include "physics/physics_plugin.h"
#include "player_ent.h"

namespace app
{
	namespace entity
	{
		DEFINE_ENTITY_TYPE_INFO(KillZoneEnt);

		STATIC util::Publisher<PlayerEnt*> KillZoneEnt::s_OutOfBounds;


		KillZoneEnt::KillZoneEnt()
			: m_pBody(null)
		{
		}

		VIRTUAL void KillZoneEnt::Setup(b2Body* pBody, b2dJson* pJson)
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

		VIRTUAL void KillZoneEnt::Init()
		{
			baka::physics::contact_events::s_ContactBegin.Subscribe(this, BIND1(this, &KillZoneEnt::OnContactBegin));
			baka::physics::contact_events::s_ContactEnd.Subscribe(this, BIND1(this, &KillZoneEnt::OnContactEnd));
		}

		VIRTUAL void KillZoneEnt::Exit()
		{
		}

		VIRTUAL void KillZoneEnt::Update(const sf::Time& dt)
		{
		}

		void KillZoneEnt::OnContactBegin(b2Contact* contact)
		{
			IPhysicsEnt* data = null;
			if ((m_pSensor != null && contact->GetFixtureA() == m_pSensor) || (contact->GetFixtureA()->GetBody() == m_pBody))
			{
				data = static_cast<IPhysicsEnt*>(contact->GetFixtureB()->GetBody()->GetUserData());
			}
			else if ((m_pSensor != null && contact->GetFixtureB() == m_pSensor) || (contact->GetFixtureB()->GetBody() == m_pBody))
			{
				data = static_cast<IPhysicsEnt*>(contact->GetFixtureA()->GetBody()->GetUserData());
			}
			if (data && data->GetType() == PlayerEnt::Type)
			{
				PlayerEnt* pPlayer = static_cast<PlayerEnt*>(data);
				s_OutOfBounds.Publish(pPlayer);
				pPlayer->Respawn();
			}
		}

		void KillZoneEnt::OnContactEnd(b2Contact* contact)
		{
		}

	}
}
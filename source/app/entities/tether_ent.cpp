#pragma once
#include "tether_ent.h"
#include "render/render_plugin.h"
#include "physics/physics_plugin.h"
#include "physics/callbacks/raycast_callback.h"

namespace app
{
	namespace entity
	{
		TetherEnt::TetherEnt(b2Body* pPlayerBody, b2Body* pObjectBody, b2Vec2 startPoint, b2Vec2 endPoint)
			: m_pPlayerBody(pPlayerBody)
			, m_pObjectBody(pObjectBody)
			, m_startLocalPoint(pPlayerBody->GetLocalPoint(startPoint))
			, m_endLocalPoint(m_pObjectBody->GetLocalPoint(endPoint))
		{

		}

		VIRTUAL TetherEnt::~TetherEnt()
		{
		}

		VIRTUAL void TetherEnt::Init()
		{
			b2World* pWorld = m_pPlayerBody->GetWorld();

			b2RopeJointDef md;
			md.bodyA = m_pPlayerBody;
			md.bodyB = m_pObjectBody;
			md.localAnchorA = m_startLocalPoint;
			md.localAnchorB = m_endLocalPoint;
			md.maxLength = (m_pPlayerBody->GetWorldPoint(m_startLocalPoint) - m_pObjectBody->GetWorldPoint(m_endLocalPoint)).Length();
			md.collideConnected = true;
			m_pRopeJoint = (b2RopeJoint*)pWorld->CreateJoint(&md);
			m_pObjectBody->SetAwake(true);
		}

		VIRTUAL void TetherEnt::Exit()
		{
			if (m_pRopeJoint && m_pPlayerBody)
			{
				m_pPlayerBody->GetWorld()->DestroyJoint(m_pRopeJoint);
				m_pRopeJoint = NULL;
			}
		}

		VIRTUAL void TetherEnt::Update(const sf::Time& dt)
		{
		}
	}
}
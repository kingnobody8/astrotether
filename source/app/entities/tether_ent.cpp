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

			b2Vec2 startWorldPoint = m_pPlayerBody->GetWorldPoint(m_startLocalPoint);
			b2Vec2 endWorldPoint = m_pObjectBody->GetWorldPoint(m_endLocalPoint);

			b2RopeJointDef md;
			md.bodyA = m_pPlayerBody;
			md.bodyB = m_pObjectBody;
			md.localAnchorA = m_startLocalPoint;
			md.localAnchorB = m_endLocalPoint;
			md.maxLength = (endWorldPoint - startWorldPoint).Length();
			md.collideConnected = true;
			m_pRopeJoint = (b2RopeJoint*)pWorld->CreateJoint(&md);
			m_pObjectBody->SetAwake(true);

			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			pRenderPlug->AddDrawable(&m_drawableRect, "physics");
		}

		VIRTUAL void TetherEnt::Exit()
		{
			if (m_pRopeJoint && m_pPlayerBody)
			{
				m_pPlayerBody->GetWorld()->DestroyJoint(m_pRopeJoint);
				m_pRopeJoint = NULL;
			}
			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			pRenderPlug->RemDrawable(&m_drawableRect);
		}

		VIRTUAL void TetherEnt::Update(const sf::Time& dt)
		{
			//m_vVertexArray[0].position = sf::Vector2f(m_pRopeJoint->GetAnchorA().x, -m_pRopeJoint->GetAnchorA().y);
			//m_vVertexArray[1].position = sf::Vector2f(m_pRopeJoint->GetAnchorB().x, -m_pRopeJoint->GetAnchorB().y);

			CreateRect();
		}

		void TetherEnt::CreateRect()
		{
			b2Vec2 swp = m_pPlayerBody->GetWorldPoint(m_startLocalPoint);
			b2Vec2 ewp = m_pObjectBody->GetWorldPoint(m_endLocalPoint);
			swp.y *= -1;
			ewp.y *= -1;

			float len = (ewp - swp).Length();
			b2Vec2 dir = (ewp - swp);
			dir.Normalize();
			sf::Vector2f pos = sf::Vector2f(swp.x, swp.y);
			float angle = atan2(dir.x, dir.y) * -RAD_DEG;

			m_drawableRect.setSize(sf::Vector2f(len, 0.05));
			m_drawableRect.setPosition(pos);
			m_drawableRect.setRotation(angle + 90); __todo() //why do i need to add 90 degrees to make it look right?
		}

	}
}
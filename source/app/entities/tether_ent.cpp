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

			sf::Vertex v;
			v.color = sf::Color::Red;

			v.position = sf::Vector2f(startWorldPoint.x, -startWorldPoint.y);
			m_vVertexArray.append(v);

			v.position = sf::Vector2f(endWorldPoint.x, -endWorldPoint.y);
			m_vVertexArray.append(v);

			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			m_vVertexArray.setPrimitiveType(sf::PrimitiveType::LinesStrip);
			pRenderPlug->AddDrawable(&m_vVertexArray, "physics");
		}

		VIRTUAL void TetherEnt::Exit()
		{
			if (m_pRopeJoint && m_pPlayerBody)
			{
				m_pPlayerBody->GetWorld()->DestroyJoint(m_pRopeJoint);
				m_pRopeJoint = NULL;
			}
			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			pRenderPlug->RemDrawable(&m_vVertexArray);
		}

		VIRTUAL void TetherEnt::Update(const sf::Time& dt)
		{
			m_vVertexArray[0].position = sf::Vector2f(m_pRopeJoint->GetAnchorA().x, -m_pRopeJoint->GetAnchorA().y);
			m_vVertexArray[1].position = sf::Vector2f(m_pRopeJoint->GetAnchorB().x, -m_pRopeJoint->GetAnchorB().y);
		}
	}
}
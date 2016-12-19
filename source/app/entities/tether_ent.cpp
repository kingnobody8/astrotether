#pragma once
#include "tether_ent.h"
#include "render/render_plugin.h"
#include "physics/physics_plugin.h"
#include "physics/callbacks/raycast_callback.h"
#include "physics/callbacks/aabb_callback.h"

const float lineSize = 0.05f;
const float sensorSize = 0.1f;

namespace app
{
	namespace entity
	{
		void RopeSegment::Destroy()
		{
			if (m_pRopeJoint)
			{
				m_pRopeJoint->GetBodyA()->GetWorld()->DestroyJoint(m_pRopeJoint);
				m_pRopeJoint = NULL;
			}
			if (m_pDrawable)
			{
				baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
				if (!pRenderPlug) return; __todo() //this is a hack because we don't use an object plugin system yet
					pRenderPlug->RemDrawable(m_pDrawable);
				delete m_pDrawable;
				m_pDrawable = null;
			}
		}

		void RopeSegment::Update(const sf::Time& dt)
		{
			if (!m_pRopeJoint)
				return;

			b2Vec2 swp = m_prevStart = m_pRopeJoint->GetAnchorA();
			b2Vec2 ewp = m_prevEnd = m_pRopeJoint->GetAnchorB();

			float len = (ewp - swp).Length();
			b2Vec2 dir = (ewp - swp);
			dir.Normalize();

			//stupid rendering system
			swp.y *= -1;
			ewp.y *= -1;
			dir.y *= -1;
			float angle = atan2(dir.x, dir.y) * -RAD_DEG + 90;

			sf::Vector2f pos = sf::Vector2f(swp.x, swp.y);

			m_pDrawable->setSize(sf::Vector2f(len, 0.05));
			m_pDrawable->setPosition(pos);
			m_pDrawable->setRotation(angle);
		}

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
			CreateTether(m_pPlayerBody, m_pObjectBody, m_startLocalPoint, m_endLocalPoint);
		}

		VIRTUAL void TetherEnt::Exit()
		{
			for (int i = 0; i < m_vRopeSegments.size(); ++i)
			{
				m_vRopeSegments[i]->Destroy();
				delete m_vRopeSegments[i];
			}
			m_vRopeSegments.clear();

			//if (m_pRopeJoint && m_pPlayerBody)
			//{
			//	m_pPlayerBody->GetWorld()->DestroyJoint(m_pRopeJoint);
			//	m_pRopeJoint = NULL;
			//}
			//baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			//pRenderPlug->RemDrawable(&m_drawableRect);
		}

		VIRTUAL void TetherEnt::Update(const sf::Time& dt)
		{
			//for (int i = 0; i < m_vRopeSegments.size(); ++i)
			//{
			//	CheckSegment(i, m_vRopeSegments[i]);
			//}

			for (int i = 0; i < m_vRopeSegments.size(); ++i)
			{
				m_vRopeSegments[i]->Update(dt);
			}
		}

		void TetherEnt::CreateTether(b2Body* pBodyA, b2Body* pBodyB, b2Vec2 localPointA, b2Vec2 localPointB)
		{
			assert(pBodyA && pBodyB);

			b2World* pWorld = pBodyA->GetWorld();
			b2Vec2 worldPointA = pBodyA->GetWorldPoint(m_startLocalPoint);
			b2Vec2 worldPointB = pBodyB->GetWorldPoint(m_endLocalPoint);

			RopeSegment* seg = new RopeSegment();

			b2RopeJointDef md;
			md.bodyA = pBodyA;
			md.bodyB = pBodyB;
			md.localAnchorA = localPointA;
			md.localAnchorB = localPointB;
			md.maxLength = (worldPointB - worldPointA).Length();
			md.collideConnected = true;
			seg->m_pRopeJoint = (b2RopeJoint*)pWorld->CreateJoint(&md);

			pBodyA->SetAwake(true);
			pBodyB->SetAwake(true);

			seg->m_prevStart = worldPointA;
			seg->m_prevEnd = worldPointB;

			seg->m_pDrawable = new sf::RectangleShape();

			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			pRenderPlug->AddDrawable(seg->m_pDrawable, "physics");

			seg->Update(sf::Time());

			m_vRopeSegments.push_back(seg);
		}

		void TetherEnt::CheckSegment(int& index, RopeSegment* alpha)
		{
			b2World* pWorld = m_pPlayerBody->GetWorld();

			//check for breaking up the first rope
			if (!alpha->m_pRopeJoint)
				return;
			b2Vec2 currStart = alpha->m_pRopeJoint->GetAnchorA();
			b2Vec2 currEnd = alpha->m_pRopeJoint->GetAnchorB();

			b2PolygonShape shape;
			shape.SetAsBox(1, 1);

			shape.m_vertices[0] = alpha->m_prevStart;
			shape.m_vertices[1] = alpha->m_prevEnd;
			shape.m_vertices[2] = currEnd;
			shape.m_vertices[3] = currStart;

			b2Transform transform;
			transform.SetIdentity();

			baka::physics::callbacks::AabbCallbackAll callback;
			pWorld->QueryShapeAABB(&callback, shape, transform);

			//do a ray test to find where the intersection is
			baka::physics::callbacks::RayCastClosestCallback rcallback;
			rcallback.m_vIgnore.push_back(m_pPlayerBody);
			rcallback.m_vIgnore.push_back(m_pObjectBody);
			pWorld->RayCast(&rcallback, alpha->m_pRopeJoint->GetAnchorA(), alpha->m_pRopeJoint->GetAnchorB());

			if (rcallback.m_pFixture != null)
			{
				static bool s = true;
				if (s)
				{
					s = false;
					{
						//body definition
						b2BodyDef def;
						def.type = b2_staticBody;

						//shape definition
						b2CircleShape circleShape;
						circleShape.m_radius = 0.1f;

						//fixture definition
						b2FixtureDef myFixtureDef;
						myFixtureDef.shape = &circleShape;
						myFixtureDef.density = 1;

						//create dynamic body
						b2Vec2 startPoint = rcallback.m_point;
						def.position.Set(startPoint.x, startPoint.y);
						b2Body* body = pWorld->CreateBody(&def);
						body->CreateFixture(&myFixtureDef);

						CreateTether(body, m_pPlayerBody, body->GetLocalPoint(startPoint), alpha->m_pRopeJoint->GetLocalAnchorA());

						pWorld->DestroyJoint(alpha->m_pRopeJoint);
						alpha->m_pRopeJoint = null;
					}
				}

				//pWorld->DestroyBody(callback.m_vFixtures[0]->GetBody());
			}
		}
	}
}
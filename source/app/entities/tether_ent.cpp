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
			b2Vec2 swp = m_prevStart = m_pRopeJoint->GetAnchorA();
			b2Vec2 ewp = m_prevEnd = m_pRopeJoint->GetAnchorB();

			float len = (ewp - swp).Length();
			b2Vec2 dir = (ewp - swp);
			dir.Normalize();

			//update the sensor position/rotation
			float angle = -atan2(dir.x, dir.y);
			angle += 90 * DEG_RAD; __todo() //why do i need to add 90 degrees to make it look right?
				m_pSensor->SetTransform(swp + dir * len * 0.5f, angle);

			//stupid rendering system
			swp.y *= -1;
			ewp.y *= -1;
			dir.y *= -1;
			angle = atan2(dir.x, dir.y) * -RAD_DEG + 90;

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


			baka::physics::contact_events::s_ContactBegin.Subscribe(this, BIND1(this, &TetherEnt::OnContactBegin));
			baka::physics::contact_events::s_ContactEnd.Subscribe(this, BIND1(this, &TetherEnt::OnContactEnd));

			//b2World* pWorld = m_pPlayerBody->GetWorld();
			//
			//b2Vec2 startWorldPoint = m_pPlayerBody->GetWorldPoint(m_startLocalPoint);
			//b2Vec2 endWorldPoint = m_pObjectBody->GetWorldPoint(m_endLocalPoint);
			//
			//b2RopeJointDef md;
			//md.bodyA = m_pPlayerBody;
			//md.bodyB = m_pObjectBody;
			//md.localAnchorA = m_startLocalPoint;
			//md.localAnchorB = m_endLocalPoint;
			//md.maxLength = (endWorldPoint - startWorldPoint).Length();
			//md.collideConnected = true;
			//m_pRopeJoint = (b2RopeJoint*)pWorld->CreateJoint(&md);
			//m_pObjectBody->SetAwake(true);
			//
			//baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			//pRenderPlug->AddDrawable(&m_drawableRect, "physics");
		}

		VIRTUAL void TetherEnt::Exit()
		{
			for (int i = 0; i < m_vRopeSegments.size(); ++i)
			{
				m_vRopeSegments[i].Destroy();
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
			//m_vVertexArray[0].position = sf::Vector2f(m_pRopeJoint->GetAnchorA().x, -m_pRopeJoint->GetAnchorA().y);
			//m_vVertexArray[1].position = sf::Vector2f(m_pRopeJoint->GetAnchorB().x, -m_pRopeJoint->GetAnchorB().y);

			for (int i = 0; i < m_vRopeSegments.size(); ++i)
			{
				CheckSegment(i, m_vRopeSegments[i]);
			}


			for (int i = 0; i < m_vRopeSegments.size(); ++i)
			{
				m_vRopeSegments[i].Update(dt);
			}
		}

		void TetherEnt::CreateTether(b2Body* pBodyA, b2Body* pBodyB, b2Vec2 localPointA, b2Vec2 localPointB)
		{
			assert(pBodyA && pBodyB);

			b2World* pWorld = pBodyA->GetWorld();
			b2Vec2 worldPointA = pBodyA->GetWorldPoint(m_startLocalPoint);
			b2Vec2 worldPointB = pBodyB->GetWorldPoint(m_endLocalPoint);

			RopeSegment seg;

			b2RopeJointDef md;
			md.bodyA = pBodyA;
			md.bodyB = pBodyB;
			md.localAnchorA = localPointA;
			md.localAnchorB = localPointB;
			md.maxLength = (worldPointB - worldPointA).Length();
			md.collideConnected = true;
			seg.m_pRopeJoint = (b2RopeJoint*)pWorld->CreateJoint(&md);

			pBodyA->SetAwake(true);
			pBodyB->SetAwake(true);

			seg.m_prevStart = worldPointA;
			seg.m_prevEnd = worldPointB;

			seg.m_pDrawable = new sf::RectangleShape();

			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			pRenderPlug->AddDrawable(seg.m_pDrawable, "physics");


			//create sensor

			//body definition
			b2BodyDef def;
			def.type = b2_dynamicBody;

			//shape definition
			b2PolygonShape shape;
			shape.SetAsBox((worldPointB - worldPointA).Length() / 2, sensorSize);

			//fixture definition
			b2FixtureDef myFixtureDef;
			myFixtureDef.shape = &shape;
			myFixtureDef.isSensor = true;

			seg.m_pSensor = pWorld->CreateBody(&def);
			seg.m_pSensor->CreateFixture(&myFixtureDef);

			m_vRopeSegments.push_back(seg);
		}

		void TetherEnt::CheckSegment(int& index, RopeSegment& alpha)
		{
			b2World* pWorld = m_pPlayerBody->GetWorld();

			//check for breaking up the first rope
			b2Vec2 currStart = alpha.m_pRopeJoint->GetAnchorA();
			b2Vec2 currEnd = alpha.m_pRopeJoint->GetAnchorB();

			/*b2PolygonShape shape;
			shape.SetAsBox(1, 1);

			shape.m_vertices[0] = alpha.m_prevStart;
			shape.m_vertices[1] = currStart;
			shape.m_vertices[2] = alpha.m_prevEnd;
			shape.m_vertices[3] = currEnd;

			b2Transform transform;
			transform.SetIdentity();

			baka::physics::callbacks::AabbCallbackAll callback;
			pWorld->QueryShapeAABB(&callback, shape, transform);*/

			for (int i = 0; i < alpha.m_vContacts.size(); ++i)
			{
				b2Contact* contact = alpha.m_vContacts[i];

				b2Fixture* otherFixture = null;
				if (contact->GetFixtureA()->GetBody() == alpha.m_pSensor)
					otherFixture = contact->GetFixtureB();
				else
					otherFixture = contact->GetFixtureA();

				if (contact->IsTouching())
				{
					int x = 0;
					x++;
				}
			}

			/*for (int i = 0; i < callback.m_vFixtures.size(); ++i)
			{
			if (callback.m_vFixtures[i]->GetBody() == m_pPlayerBody ||
			callback.m_vFixtures[i]->GetBody() == m_pObjectBody ||
			callback.m_vFixtures[i]->GetBody()->GetType() != b2BodyType::b2_staticBody)
			{
			callback.m_vFixtures.erase(callback.m_vFixtures.begin() + i);
			--i;
			continue;
			}
			}

			if (!callback.m_vFixtures.empty())
			{
			b2Shape* pShape = callback.m_vFixtures[0]->GetShape();
			b2Transform tran = callback.m_vFixtures[0]->GetBody()->GetTransform();
			b2Manifold mani;

			b2CollidePolygons(&mani, (b2PolygonShape*)pShape, tran, &shape, transform);

			pWorld->DestroyBody(callback.m_vFixtures[0]->GetBody());
			}*/
		}

		void TetherEnt::OnContactBegin(b2Contact* contact)
		{
			int x = 0;
			x++;

			if (contact->GetFixtureA()->GetBody() != m_pPlayerBody &&
				contact->GetFixtureA()->GetBody() != m_pObjectBody &&
				contact->GetFixtureB()->GetBody() != m_pPlayerBody &&
				contact->GetFixtureB()->GetBody() != m_pObjectBody)
			{
				for (int i = 0; i < m_vRopeSegments.size(); ++i)
				{
					if (m_vRopeSegments[i].m_pSensor == contact->GetFixtureA()->GetBody() ||
						m_vRopeSegments[i].m_pSensor == contact->GetFixtureB()->GetBody())
					{
						m_vRopeSegments[i].m_vContacts.push_back(contact);
						break;
					}
				}
			}
		}

		void TetherEnt::OnContactEnd(b2Contact* contact)
		{
			for (int i = 0; i < m_vRopeSegments.size(); ++i)
			{
				for (int j = 0; j < m_vRopeSegments[i].m_vContacts.size(); ++j)
				{
					if (m_vRopeSegments[i].m_vContacts[j] == contact)
					{
						m_vRopeSegments[i].m_vContacts.erase(m_vRopeSegments[i].m_vContacts.begin() + j);
						return;
					}
				}
			}
		}
	}
}
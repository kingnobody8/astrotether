#pragma once
#include "block_ent.h"
#include "utility/helper/func.h"
#include "physics/physics_plugin.h"
#include "render/render_plugin.h"

namespace app
{
	namespace entity
	{
		DEFINE_ENTITY_TYPE_INFO(BlockEnt);

		BlockEnt::BlockEnt()
			: m_pBody(null)
			, m_pDrawableR(null)
			, m_pDrawableG(null)
			, m_nPlayerId(-1)
			, m_bRespawn(false)
			, m_bInGoal(false)
		{
		}

		VIRTUAL void BlockEnt::Setup(b2Body* pBody, b2dJson* pJson)
		{
			m_pBody = pBody;
			baka::physics::PhysicsPlugin* pPlug = FIND_PLUGIN(baka::physics::PhysicsPlugin);
			auto vec = pPlug->FindImagesByBody(pBody);

			assert(vec.size() == 2);

			baka::render::PhysicsDrawable* pTmp1, *pTmp2;
			pTmp1 = static_cast<baka::render::PhysicsDrawable*>(vec[0]);
			pTmp2 = static_cast<baka::render::PhysicsDrawable*>(vec[1]);

			std::string path = pTmp1->GetPath();
			int val = path.find("box_green");
			if (val != -1)
			{
				m_pDrawableR = pTmp2;
				m_pDrawableG = pTmp1;
			}
			else
			{
				m_pDrawableR = pTmp1;
				m_pDrawableG = pTmp2;
			}

			m_nPlayerId = pJson->getCustomInt(pBody, "intData1", -1);
		}

		VIRTUAL void BlockEnt::Init()
		{
			baka::render::RenderPlugin* pPlug = FIND_PLUGIN(baka::render::RenderPlugin);
			pPlug->RemDrawable(m_pDrawableR);
			pPlug->RemDrawable(m_pDrawableG);
			m_soundBufferSettled.loadFromFile("assets/sfx/settled.wav");
		}

		VIRTUAL void BlockEnt::Exit()
		{
		}

		VIRTUAL void BlockEnt::Update(const sf::Time& dt)
		{
			if (m_bRespawn)
			{
				m_bRespawn = false;
				if (m_nPlayerId == 0)
				{
					m_pBody->SetTransform(b2Vec2(-16, 0), m_pBody->GetAngle());
				}
				else
				{
					m_pBody->SetTransform(b2Vec2(16, 0), m_pBody->GetAngle());
				}
			}
			if (m_pBody->IsActive())
			{
				if (IsSettled() && m_bInGoal)
				{
					SetToGreen(true);
				}
				else
				{
					SetToGreen(false);
				}
			}
		}

		void BlockEnt::SetToGreen(bool b)
		{
			if (m_bIsGreen == b)
				return;
			m_bIsGreen = b;

			m_sound.setBuffer(m_soundBufferSettled);
			m_sound.play();

			baka::render::RenderPlugin* pPlug = FIND_PLUGIN(baka::render::RenderPlugin);
			if (b)
			{
				pPlug->RemDrawable(m_pDrawableR);
				pPlug->AddDrawable(m_pDrawableG, "physics");

				//baka::render::PhysicsDrawable* pDraw = static_cast<baka::render::PhysicsDrawable*>(m_pDrawable);
				//pDraw->SetColor(sf::Color::Green);
			}
			else
			{
				pPlug->AddDrawable(m_pDrawableR, "physics");
				pPlug->RemDrawable(m_pDrawableG);
				//baka::render::PhysicsDrawable* pDraw = static_cast<baka::render::PhysicsDrawable*>(m_pDrawable);
				//pDraw->SetColor(sf::Color::White);
			}
		}

		void BlockEnt::Explode(const b2Vec2 v)
		{
			m_pBody->SetActive(true);
			baka::render::RenderPlugin* pPlug = FIND_PLUGIN(baka::render::RenderPlugin);
			pPlug->AddDrawable(m_pDrawableR, "physics");

			b2Vec2 impulse = m_pBody->GetPosition();
			impulse.x *= 1.5;
			impulse.y *= 2.5;
			m_pBody->ApplyLinearImpulse(impulse, m_pBody->GetLocalCenter(), true);
		}

		void BlockEnt::Respawn()
		{
			m_bRespawn = true;
			
		}

	}
}
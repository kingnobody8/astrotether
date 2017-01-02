#pragma once
#include "rain_ent.h"
#include "utility/helper/func.h"
#include "physics/physics_plugin.h"
#include "ball_ent.h"

namespace app
{
	namespace entity
	{
		DEFINE_ENTITY_TYPE_INFO(RainEnt);

		RainEnt::RainEnt()
			: m_pParticleSystem(null)
		{
		}

		VIRTUAL void RainEnt::Setup(b2Body* pBody, b2dJson* pJson)
		{
			const b2AABB box = pBody->GetFixtureList()->GetAABB(0);
			minX = box.lowerBound.x;
			maxX = box.upperBound.x;
			altY = box.lowerBound.y;


			const b2ParticleSystemDef particleSystemDef;
			m_pParticleSystem = pBody->GetWorld()->CreateParticleSystem(&particleSystemDef);
			m_pParticleSystem->SetRadius(0.1f);
			m_pParticleSystem->SetDamping(0.2f);
			m_pParticleSystem->SetDensity(0.01f);
			//m_pParticleSystem->SetGravityScale(2);


			pd.flags = b2_solidParticleGroup;
			pd.color = b2Color(0, 0.25, 0.45, 0);
			pd.color.r = 24;
			pd.color.g = 59;
			pd.color.b = 85;
			pd.color.a = 100;
			//pd.color = b2Color(sf::Color::Yellow.r / 255.0f, sf::Color::Yellow.g / 255.0f, sf::Color::Yellow.b / 255.0f);

			pd.lifetime = 5.0f;
			//pd.shape = pBody->GetFixtureList()[0].GetShape();
			//pd.position = pBody->GetPosition();

			pBody->GetWorld()->DestroyBody(pBody);
		}

		VIRTUAL void RainEnt::Init()
		{
			m_timer = sf::seconds(util::RandomFloat(0.1f, 0.3f));
		}

		VIRTUAL void RainEnt::Exit()
		{
		}

		VIRTUAL void RainEnt::Update(const sf::Time& dt)
		{
#define MIN_DROPS 10
#define MAX_DROPS 40

#define MIN_TIME 0.1f
#define MAX_TIME 0.3f
			b2Vec2 asdf[MAX_DROPS];
			m_timer -= dt;
			if (m_timer.asSeconds() < 0)
			{
				m_timer = sf::seconds(util::RandomFloat(MIN_TIME, MAX_TIME));

				int drops = util::RandomSlong(MIN_DROPS, MAX_DROPS);
				float dist = (maxX - minX) / drops;

				//b2Vec2* posData = new b2Vec2[pd.particleCount];
				for (int i = 0; i < drops; ++i)
				{
					asdf[i] = b2Vec2(minX + dist * i, altY + util::RandomFloat(-1.0f, 4.0f));
				}
				pd.positionData = asdf;
				//pd.linearVelocity = b2Vec2(util::RandomFloat(-10.0f, 10.0f), util::RandomFloat(-1.0f, 1.0f));

				pd.particleCount = drops;
				if (pd.particleCount > 0)
				{
					b2ParticleGroup * const group = m_pParticleSystem->CreateParticleGroup(pd);
				}
				//delete[] posData;
			}
		}

	}
}
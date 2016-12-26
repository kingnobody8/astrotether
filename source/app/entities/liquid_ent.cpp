#pragma once
#include "liquid_ent.h"
#include "utility/helper/func.h"

namespace app
{
	namespace entity
	{
		DEFINE_ENTITY_TYPE_INFO(LiquidEnt);

		LiquidEnt::LiquidEnt()
		{
		}


		VIRTUAL void LiquidEnt::Setup(b2Body* pBody, b2dJson* pJson)
		{
			const b2ParticleSystemDef particleSystemDef;
			m_pParticleSystem = pBody->GetWorld()->CreateParticleSystem(&particleSystemDef);
			m_pParticleSystem->SetRadius(0.1f);
			m_pParticleSystem->SetDamping(0.2f);
			m_pParticleSystem->SetDensity(10.0f);



			b2ParticleGroupDef pd;
			pd.flags = b2_solidParticleGroup;
			pd.color = b2Color(0,0.5,0.5,1.0);
			pd.shape = pBody->GetFixtureList()[0].GetShape();
			pd.position = pBody->GetPosition();
			pd.particleCount = 20;
			pd.flags |= b2_fixtureContactListenerParticle;

			if (pd.particleCount > 0)
			{
				b2Vec2* posData = new b2Vec2[pd.particleCount];
				for (int i = 0; i < pd.particleCount; ++i)
				{
					posData[i] = b2Vec2(0, 0);
				}
				pd.positionData = posData;
				b2ParticleGroup * const group = m_pParticleSystem->CreateParticleGroup(pd);
				group->ApplyLinearImpulse(b2Vec2(0, 10));
			}

			pBody->GetWorld()->DestroyBody(pBody);


			//for (int i = 0; i < vBodies.size(); ++i)
			//{
			//	/*b2ParticleGroupDef pd;
			//	pd.flags = b2_solidParticleGroup;
			//	pd.lifetime = 3.0f;
			//	pd.linearVelocity = b2Vec2(1.0f, 0.5f);
			//	util::Color clr = util::Color::CYAN;
			//	pd.color = b2Color(clr.r, clr.g, clr.b, clr.a);*/
			//	m_pParticleEmitterShape = *(b2PolygonShape*)vBodies[i]->GetFixtureList()[0].GetShape();
			//	m_pParticleEmitterPosition = vBodies[i]->GetPosition();
			//	angle = vBodies[i]->GetAngle();

			//	//b2PolygonShape* shapep = (b2PolygonShape*)bodyShape;
			//	//pd.shape = shapep;
			//	//pd.position = vBodies[i]->GetPosition();
			//	////pd.shape = vBodies[i]->GetFixtureList()[0].GetShape();

			//	MakeParticles(1, -1, b2Vec2(0, 0));

			//	//b2ParticleGroup * const group = m_pParticleSystem->CreateParticleGroup(pd);
			//	m_pWorld->DestroyBody(vBodies[i]);
			//}
		}


		VIRTUAL void LiquidEnt::Init()
		{
		}

		VIRTUAL void LiquidEnt::Exit()
		{
		}

		VIRTUAL void LiquidEnt::Update(const sf::Time& dt)
		{
		}

	}
}
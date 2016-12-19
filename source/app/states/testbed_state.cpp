#include "testbed_state.h"

#include "input/input_plugin.h"
#include "render/render_plugin.h"
#include "state/state_plugin.h"
#include "entity/entity_plugin.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "rapidjson/FileReadStream.h"
#include <cstdio>
#include "resource/json.h"

#include "input/input_event.h"
#include "engine/b2djson/b2dJsonImage.h"
#include "render/box2d-sfml.h"

namespace app
{
	namespace state
	{
		TestbedState::TestbedState()
			: m_pPlayer(null)
		{
			this->m_name = "Testbed";
		}

		VIRTUAL TestbedState::~TestbedState()
		{
		}

		VIRTUAL void TestbedState::Init()
		{
			//Setup physics
			m_pPhysicsPlugin = new baka::physics::PhysicsPlugin();
			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			m_pPhysicsPlugin->SetRenderWinow(pRenderPlug->GetRenderWindow());
			baka::IPlugin::AddPlugin(m_pPhysicsPlugin);

			std::string path = "assets/worlds/";
			std::string file = "testbed.json";

			b2dJson* json = m_pPhysicsPlugin->LoadWorld(path, file);

			baka::entity::EntityPlugin* pEntPlug = static_cast<baka::entity::EntityPlugin*>(baka::IPlugin::FindPlugin(baka::entity::EntityPlugin::Type));
			//b2Body* pPlayerBody = json->getBodyByName("player");
		//	m_pPlayer = CreateEntityMacro(pEntPlug, entity::PlayerEnt);
			//m_pPlayer->Setup(pPlayerBody);
		}

		VIRTUAL void TestbedState::Exit()
		{
			baka::entity::EntityPlugin* pEntPlug = static_cast<baka::entity::EntityPlugin*>(baka::IPlugin::FindPlugin(baka::entity::EntityPlugin::Type));
			if (pEntPlug)
				pEntPlug->DestroyAllEntities();
		}

		VIRTUAL const std::string TestbedState::DebugRender(sf::RenderWindow* pRenWin)
		{
			std::string ret;

			/*ret = m_pPlayer->GetPlayerValue().GetAsString();
			ret += std::string("x vel:\t") + std::to_string(m_pPlayer->GetBody()->GetLinearVelocity().x) + std::string("\n");
			ret += std::string("grounded:\t") + std::to_string(m_pPlayer->IsGrounded()) + std::string("\n");


			b2Vec2 pos = m_pPlayer->GetBody()->GetPosition();
			shooter[0].position = m_pPlayer->GetPosition();
			shooter[0].position.y *= -1;
			b2Vec2 dir = m_pPlayer->CalcShootDirection();
			pos += dir * m_pPlayer->GetPlayerValue().m_fTetherLength;
			pos.y += 0.5f;
			shooter[0].position.y -= 0.5f;

			shooter[1].position = sf::Vector2f(pos.x, -pos.y);

			shooter[0].color = sf::Color::Red;

			pRenWin->setView(m_pPhysicsPlugin->GetView());
			pRenWin->draw(shooter, 2, sf::Lines);

			pos = m_pPlayer->GetBody()->GetPosition();
			dir = m_pPlayer->CalcShootDirection();
			float angle = atan2(dir.y, dir.x);
			dir.x = cos(angle + m_pPlayer->GetPlayerValue().m_fTetherAngle * DEG_RAD);
			dir.y = sin(angle + m_pPlayer->GetPlayerValue().m_fTetherAngle * DEG_RAD);
			dir.Normalize();
			pos += dir * m_pPlayer->GetPlayerValue().m_fTetherLength;
			pos.y += 0.5f;
			shooter[1].position = sf::Vector2f(pos.x, -pos.y);
			pRenWin->setView(m_pPhysicsPlugin->GetView());
			pRenWin->draw(shooter, 2, sf::Lines);

			pos = m_pPlayer->GetBody()->GetPosition();
			dir = m_pPlayer->CalcShootDirection();
			angle = atan2(dir.y, dir.x);
			dir.x = cos(angle - m_pPlayer->GetPlayerValue().m_fTetherAngle * DEG_RAD);
			dir.y = sin(angle - m_pPlayer->GetPlayerValue().m_fTetherAngle * DEG_RAD);
			dir.Normalize();
			pos += dir * m_pPlayer->GetPlayerValue().m_fTetherLength;
			pos.y += 0.5f;
			shooter[1].position = sf::Vector2f(pos.x, -pos.y);
			pRenWin->setView(m_pPhysicsPlugin->GetView());
			pRenWin->draw(shooter, 2, sf::Lines);*/

			return ret;
		}

		VIRTUAL void TestbedState::Update(const sf::Time& dt)
		{
			sf::View& view = m_pPhysicsPlugin->GetView();
			//sf::Vector2f pos = m_pPlayer->GetPosition();
			//pos.y *= -1;
			//view.setCenter(pos);
		}
	}
}
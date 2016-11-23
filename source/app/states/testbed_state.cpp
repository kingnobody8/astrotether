#include "testbed_state.h"

#include "input/input_plugin.h"
#include "render/render_plugin.h"
#include "state/state_plugin.h"

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

			b2Body* pPlayerBody = json->getBodyByName("player");
			m_pPlayer = new entity::PlayerEnt(pPlayerBody);
			m_pPlayer->Init();
		}

		VIRTUAL void TestbedState::Exit()
		{
			m_pPlayer->Exit();
			delete m_pPlayer;
		}

		VIRTUAL const std::string TestbedState::DebugRender(sf::RenderWindow* pRenWin)
		{
			std::string ret;

			ret = m_pPlayer->GetPlayerValue().GetAsString();
			ret += std::string("x vel:\t") + std::to_string(m_pPlayer->GetBody()->GetLinearVelocity().x) + std::string("\n");
			ret += std::string("grounded:\t") + std::to_string(m_pPlayer->CheckGrounded()) + std::string("\n");

			return ret;
		}

		VIRTUAL void TestbedState::Update(const sf::Time& dt)
		{
			m_pPlayer->Update(dt);

			sf::View& view = m_pPhysicsPlugin->GetView();
			sf::Vector2f pos = m_pPlayer->GetPosition();
			pos.y *= -1;
			view.setCenter(pos);
		}
	}
}
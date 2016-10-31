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
#include "physics/physics_plugin.h"

namespace app
{
	namespace state
	{
		TestbedState::TestbedState()
		{
			this->m_name = "Testbed";
		}

		VIRTUAL TestbedState::~TestbedState()
		{
		}

		VIRTUAL void TestbedState::Init()
		{
			//Setup physics
			baka::physics::PhysicsPlugin* pPhysicsPlugin = new baka::physics::PhysicsPlugin();
			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			pPhysicsPlugin->SetRenderWinow(pRenderPlug->GetRenderWindow());
			baka::IPlugin::AddPlugin(pPhysicsPlugin);

			pPhysicsPlugin->LoadWorld("assets/worlds/testbed.json");
		}

		VIRTUAL void TestbedState::Exit()
		{
		}

		VIRTUAL const std::string TestbedState::DebugRender(sf::RenderWindow* pRenWin)
		{
			std::string ret;

			return ret;
		}

		VIRTUAL void TestbedState::Update(const sf::Time& dt)
		{
		}
	}
}
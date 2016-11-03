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
			m_pPhysicsPlugin = new baka::physics::PhysicsPlugin();
			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			m_pPhysicsPlugin->SetRenderWinow(pRenderPlug->GetRenderWindow());
			baka::IPlugin::AddPlugin(m_pPhysicsPlugin);

			std::string path = "assets/worlds/";
			std::string file = "testbed.json";

			b2dJson& json = m_pPhysicsPlugin->LoadWorld(path + file);
			std::vector<b2dJsonImage*> m_vImages;
			int result = json.getAllImages(m_vImages);

			for (int i = 0; i < m_vImages.size(); ++i)
			{
				b2dJsonImage* pImage = m_vImages[i];

				std::string imageFile = path + pImage->file;

				pPEnt = new entity::PhysicsImageEnt(imageFile, pImage);
				pPEnt->Init();
			}

		}

		VIRTUAL void TestbedState::Exit()
		{
		}

		VIRTUAL const std::string TestbedState::DebugRender(sf::RenderWindow* pRenWin)
		{
			pRenWin->setView(m_pPhysicsPlugin->GetView());

			//for (int i = 0; i < m_vSprites.size(); ++i)
			//{
			//	pRenWin->draw(*m_vSprites[i]);
			//}
			//

			pRenWin->draw(*pPEnt->GetSprite());

			std::string ret;

			return ret;
		}

		VIRTUAL void TestbedState::Update(const sf::Time& dt)
		{
			pPEnt->Update(dt);
		}
	}
}
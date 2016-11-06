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

baka::render::PhysicsDrawable* pPhysDraw;

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

				pPhysDraw = new baka::render::PhysicsDrawable(pImage, path);
				//pRenderPlug->AddDrawable(m_pPhysDraw);
			}

		}

		VIRTUAL void TestbedState::Exit()
		{
			delete pPhysDraw;
		}

		VIRTUAL const std::string TestbedState::DebugRender(sf::RenderWindow* pRenWin)
		{
			pRenWin->setView(pRenWin->getDefaultView());
			//pRenWin->draw(*m_pSprite);


			pRenWin->setView(m_pPhysicsPlugin->GetView());

			//for (int i = 0; i < m_vSprites.size(); ++i)
			//{
			//	pRenWin->draw(*m_vSprites[i]);
			//}
			//

			pRenWin->draw(*pPhysDraw);

			std::string ret;

			return ret;
		}

		VIRTUAL void TestbedState::Update(const sf::Time& dt)
		{
		}
	}
}
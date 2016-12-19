#include "splash_state.h"

#include "input/input_plugin.h"
#include "render/render_plugin.h"
#include "entity/entity_plugin.h"
#include "state/state_plugin.h"
#include "entities/registration.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "rapidjson/FileReadStream.h"
#include <cstdio>
#include "resource/json.h"

#include "input/input_event.h"
#include "testbed_state.h"

namespace app
{
	namespace state
	{
		SplashState::SplashState()
		{
			this->m_name = "SplashState";
		}

		VIRTUAL SplashState::~SplashState()
		{
		}

		VIRTUAL void SplashState::Init()
		{

			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			sf::View* view = new sf::View(pRenderPlug->GetRenderWindow()->getDefaultView());
			view->setCenter(sf::Vector2f());
			view->setSize(view->getSize().x, -view->getSize().y);
			pRenderPlug->AddLayer("main", view, true);


			baka::entity::EntityPlugin* pEntPlug = static_cast<baka::entity::EntityPlugin*>(baka::IPlugin::FindPlugin(baka::entity::EntityPlugin::Type));
			entity::RegisterAppEntities(pEntPlug);
			entity::LogoEnt* pLogoEnt = m_logoEnt =  static_cast<entity::LogoEnt*>(pEntPlug->CreateEntity("LogoEnt"));


			__todo() //change how we load the logo entity, it should be set from some sort of world file
			m_logoEnt->Init();
		}

		VIRTUAL void SplashState::Exit()
		{
			m_logoEnt->Exit();
		}

		VIRTUAL const std::string SplashState::DebugRender(sf::RenderWindow* pRenWin)
		{
			std::string ret;
			return ret;
		}

		VIRTUAL void SplashState::Update(const sf::Time& dt)
		{
			m_logoEnt->Update(dt);
		}

	}
}
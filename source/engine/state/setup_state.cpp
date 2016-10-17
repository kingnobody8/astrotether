#include "setup_state.h"

#include "input/input_plugin.h"
#include "render/render_plugin.h"
#include "state/state_plugin.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "rapidjson/FileReadStream.h"
#include <cstdio>
#include "resource/json.h"

#include "input/input_event.h"

namespace baka
{
	namespace state
	{
		SetupState::SetupState()
			: m_pTransitionState(null)
		{
			this->m_name = "SetupState";
		}

		VIRTUAL SetupState::~SetupState()
		{
		}

		VIRTUAL void SetupState::Init()
		{
			__todo() //use the resource loader when it is finished
			//Initial loading

			srand(unsigned int(util::Time::GetTimeSinceEpoch().Milli()));
			m_texture.loadFromFile("assets/textures/logo_128.png");
			m_sprite.setTexture(m_texture);
			m_sprite.setPosition(512, 512);
			m_shape.setTexture(&m_texture);
			m_shape.setPosition(500, 200);
			m_shape.setRadius(64);

			m_sprite.setColor(sf::Color::Green);
			m_sprite.setRotation(34);

			

			//Setup the window
			render::RenderPlugin* pRenderPlugin = new render::RenderPlugin();
			IPlugin::AddPlugin(pRenderPlugin);

			//Setup input
			InputPlugin* pInputPlugin = new InputPlugin();
			pInputPlugin->SetSFMLWin(pRenderPlugin->GetRenderWindow());
			IPlugin::AddPlugin(pInputPlugin);

			//Setup first state
			//state::StatePlugin* pStatePlugin = static_cast<state::StatePlugin*>(IPlugin::FindPlugin(state::StatePlugin::Type));
			//pStatePlugin->TransitionState(m_pTransitionState);

			mouse_events::s_InputMouseMotion.Subscribe(&sub, BIND1(this, &SetupState::OnMove));
		}

		VIRTUAL const std::string SetupState::DebugRender(sf::RenderWindow* pRenWin)
		{
			pRenWin->draw(m_sprite);
			pRenWin->draw(m_shape);


			return "Hello There from the setup state";
		}

		void SetupState::OnMove(const mouse_events::MotionAction& action)
		{
			//m_sprite.setPosition(action.m_pixel.x, action.m_pixel.y);
			m_sprite.setScale( 1.0f, m_sprite.getScale().y + 0.01f);
		}

		VIRTUAL void SetupState::Exit()
		{
		}
	}
}
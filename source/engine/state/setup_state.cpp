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
			srand(unsigned int(util::Time::GetTimeSinceEpoch().Milli()));

			//Setup the window
			render::RenderPlugin* pRenderPlugin = new render::RenderPlugin();
			IPlugin::AddPlugin(pRenderPlugin);

			//Setup input
			InputPlugin* pInputPlugin = new InputPlugin();
			pInputPlugin->SetSFMLWin(pRenderPlugin->GetRenderWindow());
			IPlugin::AddPlugin(pInputPlugin);

			//Setup first state
			state::StatePlugin* pStatePlugin = static_cast<state::StatePlugin*>(IPlugin::FindPlugin(state::StatePlugin::Type));
			pStatePlugin->TransitionState(m_pTransitionState);
		}

		VIRTUAL void SetupState::Exit()
		{
		}
	}
}
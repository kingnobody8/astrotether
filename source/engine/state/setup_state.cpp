#include "setup_state.h"

#include "input/input_plugin.h"
#include "render/render_plugin.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "rapidjson/FileReadStream.h"
#include <cstdio>
#include "resource/json.h"

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


			//Setup the window
			render::RenderPlugin* pRenderPlugin = new render::RenderPlugin();
			IPlugin::AddPlugin(pRenderPlugin);

			InputPlugin* pInputPlugin = new InputPlugin();
			pInputPlugin->SetSFMLWin(pRenderPlugin->GetRenderWindow());
			IPlugin::AddPlugin(pInputPlugin);
		}

		VIRTUAL void SetupState::Exit()
		{
		}
	}
}
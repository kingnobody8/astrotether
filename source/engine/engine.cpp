#include "engine.h"
#include "state/base_state.h"
#include "state/state_plugin.h"
#include "helper/func.h"
#include <assert.h>

namespace baka
{
	Engine* Engine::s_pInstance = nullptr;
	Engine* Engine::Get(void)
	{
		if (!s_pInstance)
			s_pInstance = new Engine();
		return s_pInstance;
	}
	void Engine::DeleteInstance(void)
	{
		SafeDelete(s_pInstance);
	}

	Engine::Engine()
		: m_quit(true)
	{
	}
	Engine::~Engine()
	{
	}

	void Engine::Init(state::IBaseState* const pFirstState)
	{
		//util::TypeCheck();

		assert(pFirstState);

		//Init state plugin
		state::StatePlugin* pStatePlugin = new state::StatePlugin(pFirstState);
		pStatePlugin->TransitionState(pFirstState);
		IPlugin::AddPlugin(pStatePlugin);

		//clear the timer
		this->m_timer.restart();

		//Don't Give Up Yet!
		m_quit = false;
	}

	void Engine::Exit(void)
	{
		IPlugin::DestroyPlugins();
		DeleteInstance();
	}

	void Engine::RunFrame(void* params)
	{
		if (m_quit)
			return;

		//Update the timer
		sf::Time delta = this->m_timer.restart();

		//Update the plugins
		if (!IPlugin::UpdatePlugins(delta))
		{
			m_quit = true;
		}
	}

}
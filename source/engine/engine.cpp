#include "engine.h"
#include "state/base_state.h"
#include "state/state_plugin.h"
#include "helper/func.h"
#include <assert.h>

const float timeScale = 1.0f;

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

		__todo() // add timescale for debuging
		/*static sf::Time t;
		t += delta;
		if (t.asSeconds() < 0.01f)
		{
			return;
		}
		t = sf::seconds(0);*/

		if (delta.asSeconds() > 0.125f)
		{
			__todo() //the very first frame is getting a very long dt
			delta = sf::seconds(0.125f); 
		}

		//Update the plugins
		if (!IPlugin::UpdatePlugins(delta))
		{
			m_quit = true;
		}
	}

}
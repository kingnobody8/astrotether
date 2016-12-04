#include "engine.h"
#include "state/base_state.h"
#include "state/state_plugin.h"
#include "helper/func.h"
#include <assert.h>

const float fps = 1 / 60.0f;
const float maxTime = 1 / 8.0f;

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
		delta = sf::seconds(Clamp(delta.asSeconds(), 0, maxTime));
		m_accumulate += delta;
		if (m_accumulate.asSeconds() < fps)
			return;

		//Update the plugins
		if (!IPlugin::UpdatePlugins(m_accumulate))
		{
			m_quit = true;
		}
		m_accumulate -= sf::seconds(fps);
	}

}
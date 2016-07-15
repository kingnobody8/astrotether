#include "engine.h"
#include "state/base_state.h"

#include "plugin.h"
#include "state/state_plugin.h"
#include "input/input_plugin.h"
#include "physics/physics_plugin.h"

namespace engine
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

		//Init the rendering system
		//m_pRenderPlugin = new render::RenderPlugin();
		//IPlugin::AddPlugin(m_pRenderPlugin);

		//Init the input system
		//InputPlugin* pInputPlugin = new InputPlugin();
		//pInputPlugin->SetSdlWin(m_pRenderPlugin->GetSdlWindow());
		//IPlugin::AddPlugin(pInputPlugin);

		//Init state plugin
		state::StatePlugin* pStatePlugin = new state::StatePlugin();
		pStatePlugin->TransitionState(pFirstState);
		IPlugin::AddPlugin(pStatePlugin);

		//Init the physics plugin
		physics::PhysicsPlugin* pPhysicsPlugin = new physics::PhysicsPlugin();
		IPlugin::AddPlugin(pPhysicsPlugin);

		//Start timer
		this->m_timer.Restart();

		//Don't quit
		m_quit = false;
	}

	void Engine::Exit(void)
	{
		IPlugin::DestroyPlugins();
		DeleteInstance();
	}

	void Engine::RunFrame(void* params)
	{
		Update();
		//m_pRenderPlugin->DoRender();
	}

	void Engine::Update(void)
	{
		if (m_quit)
			return;

		//Update the timer
		this->m_timer.Signal();
		util::Time delta = Min(this->m_timer.Delta(), 16);

		//Update the plugins
		if (!IPlugin::UpdatePlugins(delta))
		{
			m_quit = true;
		}

	}

}
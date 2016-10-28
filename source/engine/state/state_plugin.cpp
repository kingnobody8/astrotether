#include "state_plugin.h"
#include "base_state.h"
#include "utility/helper/func.h"

namespace baka
{
	namespace state
	{
		DEFINE_PLUGIN_TYPE_INFO(StatePlugin);

		StatePlugin::StatePlugin()
			: m_pCurrState(null)
			, m_pNextState(null)
		{
		}

		StatePlugin::StatePlugin(IBaseState* const pFirstState)
			: m_pCurrState(null)
			, m_pNextState(pFirstState)
		{
			m_bDebugDraw = true;
		}

		VIRTUAL StatePlugin::~StatePlugin()
		{
		}

		VIRTUAL void StatePlugin::Init()
		{
		}

		VIRTUAL void StatePlugin::Exit()
		{
			if (m_pCurrState)
				m_pCurrState->Exit();

			SafeDelete(m_pCurrState);
			SafeDelete(m_pNextState);
		}

		VIRTUAL bool StatePlugin::Update(const sf::Time& dt)
		{
			if (m_pNextState)
			{
				FlushState();
			}
			m_pCurrState->Update(dt);
			return true;
		}

		VIRTUAL const std::string StatePlugin::DebugRender(sf::RenderWindow* pRenWin)
		{
			std::string temp = "----State----\n";
			if (m_pCurrState)
			{
				temp += m_pCurrState->GetName();
				temp += std::string("\n");
				temp += m_pCurrState->DebugRender(pRenWin);
			}
			return temp;
		}

		void StatePlugin::FlushState()
		{
			assert(m_pNextState);

			if (this->m_pCurrState)
			{
				this->m_pCurrState->Transition(m_pNextState);
				this->m_pCurrState->Exit();
				delete this->m_pCurrState;
				this->m_pCurrState = null;
			}
			this->m_pCurrState = m_pNextState;
			this->m_pNextState = null;
			this->m_pCurrState->Init();

			if (this->m_pNextState != null) //it is possible that the init of curr state immediately pushes a new state
			{
				FlushState();
			}
		}

		void StatePlugin::TransitionState(IBaseState* const pNextState)
		{
			assert(pNextState);
			this->m_pNextState = pNextState;
		}
	}
}
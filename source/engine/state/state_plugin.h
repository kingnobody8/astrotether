#pragma once
#include "plugin.h"

namespace baka
{
	namespace state
	{
		//forward declare
		class IBaseState;

		class StatePlugin : public IPlugin
		{
		public:
			DECLARE_PLUGIN_TYPE_INFO(StatePlugin);

		private:
			IBaseState* m_pCurrState;
			IBaseState* m_pNextState;

		private:
			void FlushState();

		public:
			StatePlugin();
			StatePlugin(IBaseState* const pFirstState);
			virtual ~StatePlugin();
			virtual const EPluginOrder GetPriority() const { return EPO_STATE; }

			virtual void Init();
			virtual void Exit();
			virtual bool Update(const sf::Time& dt);

			void TransitionState(IBaseState* const pNextState);
		};
	}
}
#pragma once
#include "SFML/system/clock.hpp"

namespace baka
{
	//forward declare
	namespace state { class IBaseState; }

	class Engine
	{
	private:
		sf::Clock				m_timer;
		bool					m_quit;

		static Engine* s_pInstance;
		Engine();
		~Engine();

	public:
		static Engine* Get(void);
		static void DeleteInstance(void);
		const bool GetQuit() const { return m_quit; }

		void Init(state::IBaseState* const pFirstState);
		void Exit(void);
		void RunFrame(void* params);
	};
}

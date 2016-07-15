#pragma once
#include "utility/time/tick.h"

namespace engine
{
	//forward declare
	namespace state { class IBaseState; }

	class Engine
	{
	private:
		util::Tick				m_timer;
		bool					m_quit;

		static Engine* s_pInstance;
		Engine();
		~Engine();

		void Update();

	public:
		static Engine* Get(void);
		static void DeleteInstance(void);
		const bool GetQuit() const { return m_quit; }

		void Init(state::IBaseState* const pFirstState);
		void Exit(void);
		void RunFrame(void* params);
	};

	/*static Engine* Get()
	{
		return Engine::Get();
	}*/
}


/*HABIG BEGIN*/
//#include <string>
//#include <assert.h>
//static void CheckSdlError(const char* ignore_error = nullptr)
//{
//	const char* err = SDL_GetError();
//	if (*err != 0)
//		SDL_Log("SDL Error: %s", err);
//	assert(*err == 0 || std::strcmp(err, ignore_error) == 0);
//	SDL_ClearError();
//}
/*HABIG END*/
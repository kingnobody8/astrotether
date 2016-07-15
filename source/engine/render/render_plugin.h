#pragma once
#include "plugin.h"
#include "sdl.h"

namespace engine
{
	namespace render
	{
		class RenderPlugin : public IPlugin
		{
		public:
			DECLARE_PLUGIN_TYPE_INFO(RenderPlugin);

		private:
			SDL_GLContext m_pGLContext;
			SDL_Window* m_pSdlWin;

		public:
			RenderPlugin();
			virtual ~RenderPlugin();

			virtual void Init();
			virtual void Exit();

			void DoRender() const;

			inline SDL_GLContext GetGLContext() { return m_pGLContext; }
			inline SDL_Window* GetSdlWindow() { return m_pSdlWin; }

		};
	}
}
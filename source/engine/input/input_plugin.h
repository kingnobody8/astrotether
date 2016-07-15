#pragma once
#include "plugin.h"
#include "math/math.h"
#include "SDL.h"


namespace engine
{
	class InputPlugin : public IPlugin
	{
	public:
		DECLARE_PLUGIN_TYPE_INFO(InputPlugin);

	private:
		SDL_Window* m_pSdlWin;

	private:
		const vec2 ConvertPixelToCartesian(int x, int y);
		const vec2 ConvertFloatPixelToCartesian(float x, float y);

	public:
		InputPlugin();
		virtual ~InputPlugin();

		virtual void Init();
		virtual void Exit();
		virtual bool Update(const util::Time& dt);

		void SetSdlWin(SDL_Window* pSdlWin) { m_pSdlWin = pSdlWin; }
	};
}
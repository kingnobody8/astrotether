#pragma once
#include "plugin.h"
#include "math/math.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


namespace engine
{
	class InputPlugin : public IPlugin
	{
	public:
		DECLARE_PLUGIN_TYPE_INFO(InputPlugin);

	private:
		sf::RenderWindow* m_pRenWin;

	private:
		const vec2 ConvertPixelToCartesian(int x, int y);
		const vec2 ConvertFloatPixelToCartesian(float x, float y);

	public:
		InputPlugin();
		virtual ~InputPlugin();

		virtual void Init();
		virtual void Exit();
		virtual bool Update(const util::Time& dt);

		void SetSFMLWin(sf::RenderWindow* pRenWin) { m_pRenWin = pRenWin; }
	};
}
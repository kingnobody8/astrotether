#pragma once
#include "plugin.h"
#include "math/math.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


namespace baka
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
		virtual const EPluginOrder GetPriority() const { return EPO_INPUT; }

		virtual void Init();
		virtual void Exit();
		virtual bool Update(const sf::Time& dt);
		virtual const std::string DebugRender(sf::RenderWindow* pRenWin);


		void SetSFMLWin(sf::RenderWindow* pRenWin) { m_pRenWin = pRenWin; }
	};
}
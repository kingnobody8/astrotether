#pragma once
#include "plugin.h"
#include <SFML/Graphics.hpp>

namespace baka
{
	namespace render
	{
		class RenderPlugin : public IPlugin
		{
		public:
			DECLARE_PLUGIN_TYPE_INFO(RenderPlugin);

		private:
			sf::RenderWindow* m_pRenWin;
			sf::View m_view;
			sf::Color m_backgroundColor;
			sf::Font m_debugFont;
			sf::Text m_debugText;
			bool m_bDebugDraw;

			std::vector<sf::Drawable*> m_vDrawables;

		public:
			RenderPlugin();
			virtual ~RenderPlugin();
			virtual const EPluginOrder GetPriority() const { return EPO_RENDER; }

			virtual void Init();
			virtual void Exit();

			virtual bool Update(const sf::Time& dt);
			void DoRender();

			inline sf::RenderWindow* GetRenderWindow() { return m_pRenWin; }

			void AddDrawable(sf::Drawable* pDrawable) { m_vDrawables.push_back(pDrawable); }
			void RemDrawable(sf::Drawable* pDrawable);
		};
	}
}
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

		public:
			RenderPlugin();
			virtual ~RenderPlugin();
			virtual const EPluginOrder GetPriority() const { return EPO_RENDER; }

			virtual void Init();
			virtual void Exit();

			virtual bool Update(const sf::Time& dt);
			void DoRender() const;

			inline sf::RenderWindow* GetRenderWindow() { return m_pRenWin; }
		};
	}
}
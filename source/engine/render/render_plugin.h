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
			sf::RenderWindow m_renWin;

		public:
			RenderPlugin();
			virtual ~RenderPlugin();

			virtual void Init();
			virtual void Exit();

			void DoRender() const;

			inline sf::RenderWindow* GetRenderWindow() { return &m_renWin; }
		};
	}
}
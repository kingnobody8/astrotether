#pragma once
#include "plugin.h"
#include <SFML/Graphics.hpp>

namespace baka
{
	namespace render
	{
		struct RenderLayer
		{
			std::string m_name;
			sf::View* m_pView;
			std::vector<sf::Drawable*> m_vDrawables;
			bool m_ownsView = false;

			~RenderLayer()
			{
				if (m_ownsView)
				{
					delete m_pView;
				}

				//for (int i = 0; i < m_vDrawables.size(); ++i)
				//{
				//	delete m_vDrawables[i];
				//}
				m_vDrawables.clear();
			}

			void draw(sf::RenderWindow* pRenWin)
			{
				pRenWin->setView(*m_pView);
				for (int i = 0; i < m_vDrawables.size(); ++i)
				{
					pRenWin->draw(*m_vDrawables[i]);
				}
			}

		};

		class RenderPlugin : public IPlugin
		{
		public:
			DECLARE_PLUGIN_TYPE_INFO(RenderPlugin);

		private:
			sf::RenderWindow* m_pRenWin;
			sf::Color m_backgroundColor;
			sf::Font m_debugFont;
			sf::Text m_debugText;
			bool m_bDebugDraw;

			std::list<RenderLayer> m_vLayers;

		public:
			RenderPlugin();
			virtual ~RenderPlugin();
			virtual const EPluginOrder GetPriority() const { return EPO_RENDER; }

			virtual void Init();
			virtual void Exit();

			virtual bool Update(const sf::Time& dt);
			void DoRender(const sf::Time& dt);

			inline sf::RenderWindow* GetRenderWindow() { return m_pRenWin; }

			void AddLayer(const std::string& szLayerName, sf::View* pView, const bool ownsView = false);
			void AddDrawable(sf::Drawable* pDrawable, std::string szLayerName);
			void RemDrawable(sf::Drawable* pDrawable);
		};
	}
}
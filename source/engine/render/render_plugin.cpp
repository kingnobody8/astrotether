#include "render_plugin.h"
#include "utility/math/util_math.h"
#include "utility/resource/json.h"
#include "utility/helper/func.h"
#include "rapidjson/FileReadStream.h"

namespace baka
{
	namespace render
	{
		DEFINE_PLUGIN_TYPE_INFO(RenderPlugin);

		RenderPlugin::RenderPlugin()
			: m_pRenWin(null)
			, m_bDebugDraw(false)
		{
		}

		VIRTUAL RenderPlugin::~RenderPlugin()
		{
		}

		VIRTUAL void RenderPlugin::Init()
		{
			FILE* fp = fopen("assets/render_init.json", "rb"); // non-Windows use "r"
			char readBuffer[65536];
			rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
			rapidjson::Document d;
			d.ParseStream(is);
			fclose(fp);

			util::JSON initJson(d);
			std::string asdf = initJson.Read();

			const std::string appName = initJson["title"].GetString();
			const util::vec2 screenSize(initJson["size"]["x"].GetDouble(), initJson["size"]["y"].GetDouble());
			const double viewRatio = initJson["view_ratio"].GetDouble();
			const std::string debugFontPath = initJson["debug_font_path"].GetString();
			m_backgroundColor.r = initJson["background_color"]["r"].GetInt();
			m_backgroundColor.g = initJson["background_color"]["g"].GetInt();
			m_backgroundColor.b = initJson["background_color"]["b"].GetInt();
			m_bDebugDraw = initJson["debug_draw"].GetBool();

			//load the font
			if (!m_debugFont.loadFromFile(debugFontPath))
			{
				assert(false);
			}
			m_debugText.setFont(m_debugFont);
			m_debugText.setString("hello \n woot");

			m_debugText.setCharacterSize(32); // in pixels, not points!
			m_debugText.setColor(sf::Color::White);

			// Create the main window
			sf::ContextSettings settings;
			m_pRenWin = new sf::RenderWindow(sf::VideoMode(screenSize.x, screenSize.y), appName.c_str(), sf::Style::Default, settings);


			// Create the camera, origin at center
			const float vw = screenSize.x * viewRatio;
			const float vh = screenSize.y * viewRatio;
			m_view = sf::View(sf::FloatRect(-vw / 2.0f, -vh / 2.0f, vw, vh));
			m_view.setSize(m_view.getSize().x, -m_view.getSize().y);
			m_pRenWin->setView(m_view);

			// Make it the active window for OpenGL calls
			m_pRenWin->setActive();

			// do an initial render, so we don't get a white screen
			DoRender(); __todo() //although now we just a a screen with the background color and nothing else for a frame. need a better solution
		}

		VIRTUAL void RenderPlugin::Exit()
		{
			m_pRenWin->close();
			SafeDelete(m_pRenWin);
		}

		VIRTUAL bool RenderPlugin::Update(const sf::Time& dt)
		{
			//m_view.setCenter(m_view.getCenter().x - 100.0f * dt.asSeconds(), m_view.getCenter().y);

			if (m_pRenWin->isOpen())
			{
				DoRender();
				return true;
			}
			return false;
		}

		void RenderPlugin::DoRender()
		{
			m_pRenWin->setView(m_view);
			m_pRenWin->clear(m_backgroundColor);

			//draw each drawable
			for (size_t i = 0; i < m_vDrawables.size(); ++i)
			{
				m_pRenWin->draw(*m_vDrawables[i]);
			}


			//draw debug information
			if (m_bDebugDraw)
			{
				
				m_pRenWin->setView(m_pRenWin->getDefaultView());
				const std::list<std::string> listDebugText = IPlugin::DebugDrawPlugins(m_pRenWin);
				std::string temp;
				for (auto iter = listDebugText.begin(); iter != listDebugText.end(); ++iter)
				{
					temp += (*iter) + std::string("\n");
				}
				m_debugText.setString(temp);
				m_pRenWin->draw(m_debugText);

				//Draw axis
				m_pRenWin->setView(m_view);
				const sf::Vertex x_axis[] =
				{
					sf::Vertex(sf::Vector2f(0, 0)),
					sf::Vertex(sf::Vector2f(100, 0), sf::Color::Red)
				};
				const sf::Vertex y_axis[] =
				{
					sf::Vertex(sf::Vector2f(0, 0)),
					sf::Vertex(sf::Vector2f(0, 100), sf::Color::Green)
				};
				m_pRenWin->draw(x_axis, 2, sf::Lines);
				m_pRenWin->draw(y_axis, 2, sf::Lines);
			}

			
			// Finally, display the rendered frame on screen
			m_pRenWin->display();
		}

		void RenderPlugin::RemDrawable(sf::Drawable* pDrawable)
		{
			for (size_t i = 0; i < m_vDrawables.size(); ++i)
			{
				if (m_vDrawables[i] == pDrawable)
				{
					m_vDrawables.erase(m_vDrawables.begin() + i);
					break;
				}
			}
		}

	}
}
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

			std::string appName = initJson["title"].GetString();
			util::vec2 screenSize(initJson["size"]["x"].GetDouble(), initJson["size"]["y"].GetDouble());
			double viewRatio = initJson["view_ratio"].GetDouble();
			m_backgroundColor.r = initJson["background_color"]["r"].GetInt();
			m_backgroundColor.g = initJson["background_color"]["g"].GetInt();
			m_backgroundColor.b = initJson["background_color"]["b"].GetInt();

			// Create the main window
			m_pRenWin = new sf::RenderWindow(sf::VideoMode(screenSize.x, screenSize.y), appName.c_str(), sf::Style::Default);

			// Create the camera, origin at center
			const float vw = screenSize.x * viewRatio;
			const float vh = screenSize.y * viewRatio;
			m_view = sf::View(sf::FloatRect(-vw / 2.0f, -vh / 2.0f, vw, vh));
			m_pRenWin->setView(m_view);

			// Make it the active window for OpenGL calls
			m_pRenWin->setActive();

			// Make it the active window for OpenGL calls
			m_pRenWin->setActive();
		}

		VIRTUAL void RenderPlugin::Exit()
		{
			m_pRenWin->close();
			SafeDelete(m_pRenWin);
		}

		VIRTUAL bool RenderPlugin::Update(const sf::Time& dt)
		{
			if (m_pRenWin->isOpen())
			{
				DoRender();
				return true;
			}
			return false;
		}


		void RenderPlugin::DoRender() const
		{
			m_pRenWin->setView(m_view);
			m_pRenWin->clear(m_backgroundColor);

			//for (size_t i = 0; i < vActors.size(); ++i)
			//{
			//	vActors[i]->Draw(window);
			//}


			//m_pWorld->DrawDebugData();
			//m_debugDraw.Flush();

			// Finally, display the rendered frame on screen
			m_pRenWin->display();
		}

	}
}
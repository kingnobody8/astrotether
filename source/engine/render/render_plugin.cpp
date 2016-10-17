#include "render_plugin.h"
#include "utility/helper/func.h"
#include "glm/glm.hpp"

namespace baka
{
	namespace render
	{
		__todo() //these should be passed in from the application layer
		const glm::u32vec2 default_dims = glm::u32vec2(1280, 720);
		const std::string app_name = "The Lark Ascending"; __todo() //move this to app project same for screen size
			const int GRID_CELL_SIZE = 64;
		const int style = sf::Style::Default;

		DEFINE_PLUGIN_TYPE_INFO(RenderPlugin);

		RenderPlugin::RenderPlugin()
		{
		}

		VIRTUAL RenderPlugin::~RenderPlugin()
		{
		}

		VIRTUAL void RenderPlugin::Init()
		{
			// Create the main window
			//m_renWin = sf::RenderWindow(sf::VideoMode(default_dims.x, default_dims.y), app_name.c_str(), style);

			// Make it the active window for OpenGL calls
			m_renWin.setActive();
		}

		VIRTUAL void RenderPlugin::Exit()
		{
		}

		void RenderPlugin::DoRender() const
		{
		}

	}
}
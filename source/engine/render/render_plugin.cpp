#include "render_plugin.h"
#include "utility/helper/func.h"
#include "render_core.h"
#include "camera.h"
#include "render_layer.h"

#include "node/polygon_node.h"
#include "shader/shader_color_polygon.h"
#include "shader/shader_default.h"
#include "node/rect_node.h"
#include "node/circle_node.h"
#include "node/line_node.h"

namespace engine
{
	namespace render
	{
		//----------------------------------------
		DefaultShader shader;
		PolygonNode node;
		//b2PolygonShape.cpp
		//CircleNode node;
		//LineNode node;
		//-----------------------------------------

		bool initGL()
		{
			
			shader.LoadProgram();

			//Initialize clear color
			glClearColor(0.13f, 0.09f, 0.1f, 1.0f);

			util::Color fade = util::Color::YELLOW;
			fade.a = 0.0f;

			std::vector<Vertex> verts;
			verts.push_back(Vertex(vec2(-1.0f, -1.0f), vec2(), util::Color::RED));
			verts.push_back(Vertex(vec2(0.5f, -0.5f), vec2(), util::Color::GREEN));
			verts.push_back(Vertex(vec2(1.0f, 1.0f), vec2(), util::Color::BLUE));
			verts.push_back(Vertex(vec2(-0.5f, 0.5f), vec2(), fade));

			std::vector<int> indicies;
			indicies.push_back(0);
			indicies.push_back(1);
			indicies.push_back(2);
			indicies.push_back(3);

			node.InitVBO(verts, indicies);
			//node.InitVBO(vec2(1.0, 0.5), util::Color::AZURE);
			//node.InitVBO(1.0f, 16, util::Color::SPRING_GREEN);
			//CircleNode circ;
			//circ.InitVBO(1.0f, 16, util::Color::SPRING_GREEN);
			//node.InitVBO(circ.GetVerts());

			return true;
		}

		void RenderTestFunc()
		{
			node.SetShader(&shader);
			node(util::Matrix());
		}


		__todo() //these should be passed in from the application layer
		const glm::u32vec2 default_dims = glm::u32vec2(1280, 720);
		const std::string app_name = "The Lark Ascending"; __todo() //move this to app project same for screen size
			const int GRID_CELL_SIZE = 64;

		DEFINE_PLUGIN_TYPE_INFO(RenderPlugin);

		RenderPlugin::RenderPlugin()
		{
		}

		VIRTUAL RenderPlugin::~RenderPlugin()
		{
		}

		VIRTUAL void RenderPlugin::Init()
		{
			//Initialization
			int result = SDL_Init(SDL_INIT_VIDEO);
			//util::CheckSdlError();
			assert(result == 0);

			//Create Window
			
#ifdef IS_MOBILE
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_EGL, 1);
			SDL_DisplayMode mode;
			SDL_GetCurrentDisplayMode(0, &mode);
			
			//this->m_tPhysicalScreenSize = Type2<slong>(mode.w, mode.h);
			Uint32 win_flag = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;
			m_pSdlWin = SDL_CreateWindow(app_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mode.w, mode.h, win_flag);
#else
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
			Uint32 win_flag = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
			m_pSdlWin = SDL_CreateWindow(app_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, default_dims.x, default_dims.y, win_flag);
#endif
			//Error check
			//util::CheckSdlError();
			assert(m_pSdlWin);
			SDL_Log("SDL Window Initialized");
			
			glm::ivec2 size;
			SDL_GetWindowSize(m_pSdlWin, &size.x, &size.y);
			SDL_Log("Window Size: %d x %d", size.x, size.y);

			//Create context
			m_pGLContext = SDL_GL_CreateContext(m_pSdlWin);
			//util::CheckSdlError();
			assert(m_pGLContext);
			SDL_Log("Open GL Context Initialized");
			
			glViewport(0, 0, size.x, size.y);
			
			
#ifndef IS_MOBILE
			//Use Vsync
			if (SDL_GL_SetSwapInterval(1) < 0)
			{
				//util::CheckSdlError();
				//assert(false);
			}
			SDL_Log("VSYNC Initialized");
#endif

			SDL_Log("Renderer Initialized");
			//--------------------------------------

			initGL();
		}

		VIRTUAL void RenderPlugin::Exit()
		{
			CRenderLayer::DestroyLayers();
			CCamera::DestroyCameras();
			SDL_GL_DeleteContext(m_pGLContext);
			m_pGLContext = null;
			SDL_DestroyWindow(m_pSdlWin);
			m_pSdlWin = null;
		}

		void RenderPlugin::DoRender() const
		{
			if (m_pGLContext == null || m_pSdlWin == null)
				return;
			
			static util::Color clr = util::Color::CYAN;
			
			glClearColor(clr.r, clr.g, clr.b, 1.0);
			auto hsv = clr.GetHSV();
			hsv.x += 0.001f;
			hsv.z = 0.5f;
			if(hsv.x > 1.0f)
				hsv.x = 0.0f;
			clr.SetHSV(hsv);

			//Clear color buffer
			glClear(GL_COLOR_BUFFER_BIT);

			//RenderGrid();
			//RenderCrossSection();
			RenderTestFunc();

			//CRenderLayer::RenderAllLayers();

			//Update screen
			SDL_GL_SwapWindow(m_pSdlWin);
		}

	}
}
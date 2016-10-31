#include "physics_plugin.h"
#include "utility/helper/func.h"
#include "render/render_plugin.h"
#include "input/input_event.h"

namespace baka
{
	namespace physics
	{
		const int VELOCITY_ITERATIONS = 8;
		const int POSITION_ITERATIONS = 3;
		const b2Vec2 GRAVITY = b2Vec2(0.0f, 0.0f);
		const int PARTICLES_PER_SEC = 24;
		const float SEXY_FPS = 1 / 60.0f;

		DEFINE_PLUGIN_TYPE_INFO(PhysicsPlugin);

		PhysicsPlugin::PhysicsPlugin()
			: m_pWorld(null)
			, m_pRenWin(null)
			, steps(0)
			, m_bIsLeftMouseDown(false)
			, m_bIsRightMouseDown(false)
		{
			m_bDebugDraw = true;
		}

		VIRTUAL PhysicsPlugin::~PhysicsPlugin()
		{
		}

		VIRTUAL void PhysicsPlugin::Init()
		{
			vec2 screenSize(1280, -720);
			float viewRatio = 0.05f;
			const float vw = screenSize.x * viewRatio;
			const float vh = screenSize.y * viewRatio;
			m_view = sf::View(sf::FloatRect(-vw / 2.0f, -vh / 2.0f, vw, vh));
			m_view.setSize(m_view.getSize().x, -m_view.getSize().y);

			m_pWorld = new b2World(b2Vec2());

			//m_pWorld = new b2World(GRAVITY);
			//m_pWorld->SetAllowSleeping(true);
			//m_pWorld->SetWarmStarting(true);
			//m_pWorld->SetContinuousPhysics(true);
			//m_pWorld->SetSubStepping(false);

			m_debugDraw.Init();
			m_pWorld->SetDebugDraw(&m_debugDraw);
			uint32 flags = 0;
			flags += true * b2Draw::e_shapeBit;
			flags += true * b2Draw::e_jointBit;
			flags += false * b2Draw::e_aabbBit;
			flags += false * b2Draw::e_centerOfMassBit;
			flags += true * b2Draw::e_particleBit;
			m_debugDraw.SetFlags(flags);

			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			auto pRenWin = pRenderPlug->GetRenderWindow();
			m_debugDraw.SetRenderWindow(pRenWin);

			baka::mouse_events::s_InputMouseButtonDown.Subscribe(&m_sub, BIND1(this, &PhysicsPlugin::OnMouseDown));
			baka::mouse_events::s_InputMouseButtonUp.Subscribe(&m_sub, BIND1(this, &PhysicsPlugin::OnMouseUp));
			baka::mouse_events::s_InputMouseMotion.Subscribe(&m_sub, BIND1(this, &PhysicsPlugin::OnMouseMove));
			baka::mouse_events::s_InputMouseScrollWheel.Subscribe(&m_sub, BIND1(this, &PhysicsPlugin::OnMouseWheel));


		}

		VIRTUAL void PhysicsPlugin::Exit()
		{
			m_debugDraw.Exit();
			SafeDelete(m_pWorld);
			m_json = b2dJson();
		}

		VIRTUAL bool PhysicsPlugin::Update(const sf::Time& dt)
		{
			static sf::Time timer;
			timer += dt;
			if (timer.asSeconds() > SEXY_FPS)
			{
				timer -= sf::seconds(SEXY_FPS);
				m_pWorld->Step(SEXY_FPS, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
				steps++;
			}

			return true;
		}

		VIRTUAL const std::string PhysicsPlugin::DebugRender(sf::RenderWindow* pRenWin)
		{
			pRenWin->setView(m_view);
			m_pWorld->DrawDebugData();
			m_debugDraw.Flush();

			const sf::Vertex x_axis[] =
			{
				sf::Vertex(sf::Vector2f(0, 0)),
				sf::Vertex(sf::Vector2f(1, 0), sf::Color::Red)
			};
			const sf::Vertex y_axis[] =
			{
				sf::Vertex(sf::Vector2f(0, 0)),
				sf::Vertex(sf::Vector2f(0, -1), sf::Color::Green)
			};
			m_pRenWin->draw(x_axis, 2, sf::Lines);
			m_pRenWin->draw(y_axis, 2, sf::Lines);

			std::string ret = "----Physics----\n";

			ret += std::string("Bodies: ") + std::to_string(m_pWorld->GetBodyCount()) + std::string("\n");
			ret += std::string("Steps: ") + std::to_string(steps) + std::string("\n");
			ret += std::string("View: \nX:") + std::to_string(m_view.getCenter().x) +
				std::string(" Y: ") + std::to_string(m_view.getCenter().y) + std::string("\n");
			ret += std::string("W: ") + std::to_string(m_view.getSize().x) + std::string("\n") +
				std::string("H: ") + std::to_string(m_view.getSize().y) + std::string("\n");



			return ret;
		}


		const b2dJson& PhysicsPlugin::LoadWorld(const std::string filepath)
		{
			std::string errorMsg;
			m_json.readFromFile(filepath.c_str(), errorMsg, m_pWorld);

			__todo() //figure out how to parse the world settings from the json and set them here
				//although it seems odd to me that the read from file doesn't just set them already
				m_pWorld->SetGravity(b2Vec2(0, -10));

			return this->m_json;
		}

		void PhysicsPlugin::OnMouseDown(const baka::mouse_events::ButtonAction& action)
		{
			sf::Vector2f worldCoords = m_pRenWin->mapPixelToCoords(action.m_pixel, this->m_view);
			if (action.m_button == sf::Mouse::Left)
			{
				m_bIsLeftMouseDown = true;
			}
			else if (action.m_button == sf::Mouse::Right)
			{
				m_bIsRightMouseDown = true;
			}
		}

		void PhysicsPlugin::OnMouseUp(const baka::mouse_events::ButtonAction& action)
		{
			sf::Vector2f worldCoords = m_pRenWin->mapPixelToCoords(action.m_pixel, this->m_view);
			if (action.m_button == sf::Mouse::Left)
			{
				m_bIsLeftMouseDown = false;
			}
			else if (action.m_button == sf::Mouse::Right)
			{
				m_bIsRightMouseDown = false;
			}
		}
	
		void PhysicsPlugin::OnMouseMove(const baka::mouse_events::MotionAction& action)
		{
			sf::Vector2f worldCoords = m_pRenWin->mapPixelToCoords(action.m_pixel, this->m_view);
			static bool b = true;
			if (b)
			{
				m_prevMouseCoords = worldCoords;
				b = false;
			}

			if (m_bIsRightMouseDown)
			{
				sf::Vector2f delta = worldCoords - m_prevMouseCoords;
				m_view.move(-delta);
				//m_view.setCenter(worldCoords);
			}


			m_prevMouseCoords = worldCoords;
		}

		void PhysicsPlugin::OnMouseWheel(const baka::mouse_events::WheelAction action)
		{
			if (action.m_delta > 0)
			{
				m_view.zoom(0.9f);
			}
			else
			{
				m_view.zoom(1.1f);
			}
		}

	}
}
#include "physics_plugin.h"
#include "utility/helper/func.h"
#include "render/render_plugin.h"
#include "input/input_event.h"
#include "callbacks/aabb_callback.h"
#include "entity/entity_plugin.h"

namespace baka
{
	namespace physics
	{
		const int VELOCITY_ITERATIONS = 24;
		const int POSITION_ITERATIONS = 24;
		const int PARTICLE_ITERATIONS = 1;
		const b2Vec2 GRAVITY = b2Vec2(0.0f, 0.0f);
		const int PARTICLES_PER_SEC = 24;
		const float SEXY_FPS = 1 / 60.0f;

		float dx;
		float dy;

		STATIC util::Publisher<b2Contact*> contact_events::s_ContactBegin;
		STATIC util::Publisher<b2Contact*> contact_events::s_ContactEnd;
		STATIC util::Publisher<ParticleBeginContactData*> contact_events::s_ParticleContactBegin;
		STATIC util::Publisher<ParticleEndContactData*> contact_events::s_ParticleContactEnd;

		DEFINE_PLUGIN_TYPE_INFO(PhysicsPlugin);

		PhysicsPlugin::PhysicsPlugin()
			: m_pWorld(null)
			, m_pRenWin(null)
			, m_pMouseJoint(null)
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
			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			
			float viewRatio = 0.05f;
			m_view = pRenderPlug->GetRenderWindow()->getDefaultView();
			m_view.setCenter(sf::Vector2f());
			m_view.setSize(m_view.getSize().x * viewRatio, m_view.getSize().y * viewRatio);
			pRenderPlug->AddLayer("physics-1", &m_view);
			pRenderPlug->AddLayer("physics", &m_view);

			m_pWorld = new b2World(b2Vec2());
			m_pWorld->SetContactListener(this);

			//m_pWorld = new b2World(GRAVITY);
			//m_pWorld->SetAllowSleeping(true);
			//m_pWorld->SetWarmStarting(true);
			//m_pWorld->SetContinuousPhysics(true);
			//m_pWorld->SetSubStepping(false);

			m_debugDraw.Init();
			m_pWorld->SetDebugDraw(&m_debugDraw);
			uint32 flags = 0;
			flags += false * b2Draw::e_shapeBit;
			flags += false * b2Draw::e_jointBit;
			flags += false * b2Draw::e_aabbBit;
			flags += false * b2Draw::e_centerOfMassBit;
			flags += true * b2Draw::e_particleBit;
			m_debugDraw.SetFlags(flags);

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
			m_pMouseJoint = null;
		}

		VIRTUAL bool PhysicsPlugin::Update(const sf::Time& dt)
		{
			m_pWorld->Step(dt.asSeconds(), VELOCITY_ITERATIONS, POSITION_ITERATIONS, PARTICLE_ITERATIONS);
			steps++;

			return true;
		}

		VIRTUAL const std::string PhysicsPlugin::DebugRender(sf::RenderWindow* pRenWin)
		{
			pRenWin->setView(m_view);
			m_pWorld->DrawDebugData();

			if (m_pMouseJoint)
			{
				b2Vec2 p1 = m_pMouseJoint->GetAnchorB();
				b2Vec2 p2 = m_pMouseJoint->GetTarget();

				b2Color c;
				c.Set(0.0f, 1.0f, 0.0f);
				m_debugDraw.DrawPoint(p1, 4.0f, c);
				m_debugDraw.DrawPoint(p2, 4.0f, c);

				c.Set(0.8f, 0.8f, 0.8f);
				m_debugDraw.DrawSegment(p1, p2, c);

				m_debugDraw.DrawSolidCircle(p1, 0.1f, b2Vec2(1, 0), b2Color(0, 1, 0));
				m_debugDraw.DrawSolidCircle(p2, 0.1f, b2Vec2(1, 0), b2Color(0, 1, 0));
			}
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

			std::string ret; //= "----Physics----\n";

			////ret += std::string("Bodies: ") + std::to_string(m_pWorld->GetBodyCount()) + std::string("\n");
			//ret += std::string("Steps: ") + std::to_string(steps);
			//ret += std::string("View: \nX:") + std::to_string(m_view.getCenter().x) +
			//	std::string(" Y: ") + std::to_string(m_view.getCenter().y) + std::string("\n");
			//ret += std::string("W: ") + std::to_string(m_view.getSize().x) + std::string("\n") +
			//	std::string("H: ") + std::to_string(m_view.getSize().y) + std::string("\n");
			//ret += std::string("dx: ") + std::to_string(dx) + std::string("\n") +
			//	std::string("dy: ") + std::to_string(dy) + std::string("\n");


			return ret;
		}


		b2dJson* PhysicsPlugin::LoadWorld(const std::string path, const std::string file)
		{
			DeleteDrawables();

			std::string errorMsg;
			std::string filepath = path + file;
			m_json.readFromFile(filepath.c_str(), errorMsg, m_pWorld);

			__todo() //figure out how to parse the world settings from the json and set them here
				//although it seems odd to me that the read from file doesn't just set them already
				m_pWorld->SetGravity(b2Vec2(0, -20));

			std::vector<b2dJsonImage*> m_vImages;
			int result = m_json.getAllImages(m_vImages);
			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			for (int i = 0; i < m_vImages.size(); ++i)
			{
				b2dJsonImage* pImage = m_vImages[i];
				render::PhysicsDrawable* pDrawable = new render::PhysicsDrawable(pImage, path);
				m_vDrawables.push_back(pDrawable);
				pRenderPlug->AddDrawable(pDrawable, "physics");
			}

			baka::entity::EntityPlugin* pEntPlug = static_cast<baka::entity::EntityPlugin*>(baka::IPlugin::FindPlugin(baka::entity::EntityPlugin::Type));
			pEntPlug->CreatePhysicsEntities(&m_json);

			return &this->m_json;
		}

		void PhysicsPlugin::DeleteDrawables()
		{
			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			for (int i = 0; i < m_vDrawables.size(); ++i)
			{
				pRenderPlug->RemDrawable(m_vDrawables[i]);
				delete m_vDrawables[i];
			}
			m_vDrawables.clear();
		}

		void PhysicsPlugin::OnMouseDown(const baka::mouse_events::ButtonAction& action)
		{
			sf::Vector2f worldCoords = m_pRenWin->mapPixelToCoords(action.m_pixel, this->m_view);
			worldCoords.y *= -1;
			if (action.m_button == sf::Mouse::Left)
			{
				m_bIsLeftMouseDown = true;
				if (m_pMouseJoint != NULL)
				{
					return;
				}

				b2Vec2 pw(worldCoords.x, worldCoords.y);

				// Make a small box.
				b2AABB aabb;
				b2Vec2 d;
				d.Set(0.001f, 0.001f);
				aabb.lowerBound = pw - d;
				aabb.upperBound = pw + d;

				// Query the world for overlapping shapes.
				callbacks::AabbCallback callback(pw);
				m_pWorld->QueryAABB(&callback, aabb);

				if (callback.m_pFixture)
				{
					b2Body* body = callback.m_pFixture->GetBody();
					b2MouseJointDef md;
					md.bodyA = body;//m_pGroundBody;
					md.bodyB = body;
					md.target = pw;
					md.maxForce = 1000.0f * body->GetMass();
					m_pMouseJoint = (b2MouseJoint*)m_pWorld->CreateJoint(&md);
					body->SetAwake(true);
				}

			}
			else if (action.m_button == sf::Mouse::Right)
			{
				m_bIsRightMouseDown = true;
				m_prevMouseCoords = worldCoords;
			}
		}

		void PhysicsPlugin::OnMouseUp(const baka::mouse_events::ButtonAction& action)
		{
			sf::Vector2f worldCoords = m_pRenWin->mapPixelToCoords(action.m_pixel, this->m_view);
			worldCoords.y *= -1;
			if (action.m_button == sf::Mouse::Left)
			{
				m_bIsLeftMouseDown = false;

				if (m_pMouseJoint)
				{
					m_pWorld->DestroyJoint(m_pMouseJoint);
					m_pMouseJoint = NULL;
				}
			}
			else if (action.m_button == sf::Mouse::Right)
			{
				m_bIsRightMouseDown = false;
			}
		}
	
		void PhysicsPlugin::OnMouseMove(const baka::mouse_events::MotionAction& action)
		{
			sf::Vector2f worldCoords = m_pRenWin->mapPixelToCoords(action.m_pixel, this->m_view);
			worldCoords.y *= -1;

			if (m_bIsLeftMouseDown)
			{
				if (m_pMouseJoint)
				{
					b2Vec2 pw(worldCoords.x, worldCoords.y);
					m_pMouseJoint->SetTarget(pw);
				}
			}
			else if (m_bIsRightMouseDown)
			{
				sf::Vector2f delta = worldCoords - m_prevMouseCoords;
				sf::Vector2f center = m_view.getCenter();
				dx = delta.x;
				dy = delta.y;
				delta.y *= -1; __todo()//i need to adjust how sfml does the map pixel to coords, i suspect it isn't using the transform (since i invert the y axis to make it cartesian i need to account for that)

				center -= delta;
				//m_view.move(sf::Vector2f(-delta));
				m_view.setCenter(center);
				m_prevMouseCoords = m_pRenWin->mapPixelToCoords(action.m_pixel, this->m_view);
				m_prevMouseCoords.y *= -1;
			}
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

		std::vector<render::PhysicsDrawable*> PhysicsPlugin::FindImagesByBody(b2Body* pBody)
		{
			std::vector<render::PhysicsDrawable*> ret;
			for (int i = 0; i < m_vDrawables.size(); ++i)
			{
				if (m_vDrawables[i]->GetBody() == pBody)
				{
					ret.push_back(m_vDrawables[i]);
				}
			}
			return ret;
		}

		VIRTUAL void PhysicsPlugin::BeginContact(b2Contact* contact)
		{
			contact_events::s_ContactBegin.Publish(contact);
		}
		VIRTUAL void PhysicsPlugin::EndContact(b2Contact* contact)
		{
			contact_events::s_ContactEnd.Publish(contact);
		}
		VIRTUAL void PhysicsPlugin::BeginContact(b2ParticleSystem* particleSystem, b2ParticleBodyContact* particleBodyContact)
		{
			ParticleBeginContactData data(particleSystem, particleBodyContact);
			contact_events::s_ParticleContactBegin.Publish(&data);
		}
		VIRTUAL void PhysicsPlugin::EndContact(b2Fixture* fixture, b2ParticleSystem* particleSystem, int32 index)
		{
			ParticleEndContactData data(particleSystem, fixture, index);
			contact_events::s_ParticleContactEnd.Publish(&data);
		}
		VIRTUAL void PhysicsPlugin::BeginContact(b2ParticleSystem* particleSystem, b2ParticleContact* particleContact)
		{
		}
		VIRTUAL void PhysicsPlugin::EndContact(b2ParticleSystem* particleSystem, int32 indexA, int32 indexB)
		{
		}
		VIRTUAL void PhysicsPlugin::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
		{
		}
		VIRTUAL void PhysicsPlugin::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
		{
		}
	}
}
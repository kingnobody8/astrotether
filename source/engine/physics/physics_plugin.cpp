#include "physics_plugin.h"
#include "utility/helper/func.h"
#include "../render/render_core.h"
#include "../render/render_plugin.h"
#include "../asset/resource_path.h"
#include "../asset/loader.h"
#include "b2djson/b2dJsonImage.h"
#include "callbacks/raycast_callback.h"
#include "callbacks/aabb_callback.h"
#include "../render/node/rect_node.h"
#include "../render/shader/shader_default.h"

namespace engine
{
	namespace physics
	{
		const int VELOCITY_ITERATIONS = 8;
		const int POSITION_ITERATIONS = 3;
		const b2Vec2 GRAVITY = b2Vec2(0.0f, 0.0f);
		const int PARTICLES_PER_SEC = 24;

		render::RectNode node;
		render::DefaultShader shader;


		DEFINE_PLUGIN_TYPE_INFO(PhysicsPlugin);

		PhysicsPlugin::PhysicsPlugin()
			: m_pWorld(null)
			, m_pMouseJoint(null)
			, m_pGroundBody(null)
			, m_bRightMouseBtn(false)
			, m_bLeftMouseBtn(false)
			, m_pParticleSystem(null)
			, rel(false)
			, m_bIsTestbed(false)
		{
		}

		VIRTUAL PhysicsPlugin::~PhysicsPlugin()
		{
		}

		VIRTUAL void PhysicsPlugin::Init()
		{
			util::Time t = util::Time::GetTimeSinceEpoch();
			SDL_Log("Physics Init Begun.");

			node.InitVBO(vec2(0.5f, 0.5f), util::Color::SPRING_GREEN);
			shader.LoadProgram();

			engine::render::RenderPlugin* plug = static_cast<engine::render::RenderPlugin*>(engine::IPlugin::FindPlugin(engine::render::RenderPlugin::Type));
			glm::ivec2 size;
			SDL_GetWindowSize(plug->GetSdlWindow(), &size.x, &size.y);
			g_camera.m_width = size.x;
			g_camera.m_height = size.y;

			mouse_events::s_InputMouseButtonDown.Subscribe(&m_sub, BIND1(this, &PhysicsPlugin::OnMouseDown));
			mouse_events::s_InputMouseButtonUp.Subscribe(&m_sub, BIND1(this, &PhysicsPlugin::OnMouseUp));
			mouse_events::s_InputMouseMotion.Subscribe(&m_sub, BIND1(this, &PhysicsPlugin::OnMouseMotion));
			mouse_events::s_InputMouseScrollWheel.Subscribe(&m_sub, BIND1(this, &PhysicsPlugin::OnMouseWheel));
			key_events::s_InputKeyDown.Subscribe(&m_sub, BIND1(this, &PhysicsPlugin::OnKeyDown));

			m_pWorld = new b2World(GRAVITY);
			m_pWorld->SetAllowSleeping(true);
			m_pWorld->SetWarmStarting(true);
			m_pWorld->SetContinuousPhysics(true);
			m_pWorld->SetSubStepping(false);

			m_debugDraw.Create();
			m_pWorld->SetDebugDraw(&m_debugDraw);
			uint32 flags = 0;
			flags += true * b2Draw::e_shapeBit;
			flags += true * b2Draw::e_jointBit;
			flags += false * b2Draw::e_aabbBit;
			flags += false * b2Draw::e_centerOfMassBit;
			flags += true * b2Draw::e_particleBit;
			m_debugDraw.SetFlags(flags);

			std::string errorMsg;
			m_json.readFromFile((getResourcePath() + std::string("assets/test_scene.json")).c_str(), errorMsg, m_pWorld);
			SDL_Log(errorMsg.c_str());

			std::vector<b2Body*> vBodies;
			m_json.getBodiesByCustomBool("particle_shape", true, vBodies);


			const b2ParticleSystemDef particleSystemDef;
			m_pParticleSystem = m_pWorld->CreateParticleSystem(&particleSystemDef);
			m_pParticleSystem->SetRadius(0.1f);
			m_pParticleSystem->SetDamping(0.2f);
			m_pParticleSystem->SetDensity(10.0f);

			for (int i = 0; i < vBodies.size(); ++i)
			{
				/*b2ParticleGroupDef pd;
				pd.flags = b2_solidParticleGroup;
				pd.lifetime = 3.0f;
				pd.linearVelocity = b2Vec2(1.0f, 0.5f);
				util::Color clr = util::Color::CYAN;
				pd.color = b2Color(clr.r, clr.g, clr.b, clr.a);*/
				m_pParticleEmitterShape = *(b2PolygonShape*)vBodies[i]->GetFixtureList()[0].GetShape();
				m_pParticleEmitterPosition = vBodies[i]->GetPosition();
				angle = vBodies[i]->GetAngle();

				//b2PolygonShape* shapep = (b2PolygonShape*)bodyShape;
				//pd.shape = shapep;
				//pd.position = vBodies[i]->GetPosition();
				////pd.shape = vBodies[i]->GetFixtureList()[0].GetShape();

				MakeParticles(1, -1, b2Vec2(0, 0));

				//b2ParticleGroup * const group = m_pParticleSystem->CreateParticleGroup(pd);
				m_pWorld->DestroyBody(vBodies[i]);
			}




			//create particles
			/*m_pParticleSystem->SetRadius(0.1f);
			m_pParticleSystem->SetDamping(0.2f);
			m_pParticleSystem->SetDensity(5.0f);
			b2ParticleGroupDef pd;
			pd.flags = b2_solidParticleGroup;
			util::Color clr = util::Color::CYAN;
			pd.color = b2Color(clr.r, clr.g, clr.b, clr.a);
			b2PolygonShape shapep;
			shapep.SetAsBox(5.0f, 10.0f, b2Vec2(0.0f, 20.0f), 0.0);
			pd.shape = &shapep;
			b2ParticleGroup * const group = m_pParticleSystem->CreateParticleGroup(pd);*/


			//create shape (test)
			b2BodyDef bd;
			b2Body* ground = m_pWorld->CreateBody(&bd);
			b2EdgeShape shapeG;
			shapeG.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
			ground->CreateFixture(&shapeG, 0.0f);


			b2BodyDef bodyDef;
			m_pGroundBody = m_pWorld->CreateBody(&bodyDef);

			//lark
			b2CircleShape shapec;
			shapec.m_radius = 1.0f;
			b2BodyDef bdc;
			bdc.fixedRotation = true;
			bdc.type = b2_dynamicBody;
			bdc.position.Set(0.0, 20.0f);
			b2Body* pLark = m_pWorld->CreateBody(&bdc);
			b2FixtureDef fdef;
			fdef.density = 10.0f;
			fdef.restitution = 0.5f;
			fdef.shape = &shapec;
			pLark->CreateFixture(&fdef);
			m_LarkController.Init(m_pWorld, pLark);


			/*float32 a = 0.5f;
			b2PolygonShape shape;
			shape.SetAsBox(a, a);

			b2Vec2 x(-7.0f, 0.75f);
			b2Vec2 y;
			b2Vec2 deltaX(0.5625f, 1.25f);
			b2Vec2 deltaY(1.125f, 0.0f);
			int e_count = 20;

			for (int32 i = 0; i < e_count; ++i)
			{
			y = x;

			for (int32 j = i; j < e_count; ++j)
			{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position = y;
			b2Body* body = m_pWorld->CreateBody(&bd);
			body->CreateFixture(&shape, 5.0f);

			y += deltaY;
			}

			x += deltaX;
			}

			for (int32 i = 0; i < e_count; ++i)
			{
			y = x;

			for (int32 j = i; j < e_count; ++j)
			{
			if (i % 5 == 0)
			{
			b2BodyDef bd;
			bd.type = b2_staticBody;
			bd.position = y;
			b2Body* body = m_pWorld->CreateBody(&bd);
			body->CreateFixture(&shape, 5.0f);
			}

			y += deltaY;
			}

			x += deltaX;
			}*/

			util::Time diff = util::Time::GetTimeSinceEpoch() - t;
			SDL_Log("Physics Loaded - Time: %d milli-secs", diff.Milli());
		}

		VIRTUAL void PhysicsPlugin::Exit()
		{
			m_sub.UnsubscribeAll();
			m_debugDraw.Destroy();
			SafeDelete(m_pWorld);
			m_pParticleSystem = null;
			m_pMouseJoint = null;
			m_pGroundBody = null;
			m_json = b2dJson();
		}

		VIRTUAL bool PhysicsPlugin::Update(const util::Time& dt)
		{
			static util::Color clr = util::Color::CYAN;

			//glClearColor(clr.r, clr.g, clr.b, 1.0);
			auto hsv = clr.GetHSV();
			hsv.x += 0.001f;
			hsv.z = 0.5f;
			if (hsv.x > 1.0f)
				hsv.x = 0.0f;
			clr.SetHSV(hsv);

			if (rel)
			{
				Reload();
				rel = false;
				return true;
			}

			//update particles
			/*if(m_pParticleSystem != null)
			{
				static util::Time t = 0;
				t += dt;
				vec2 dir;
				dir.x = sin(t.Milli() / 1000.0f) * 2.0f - 1.0f;
				dir.y = cos(t.Milli() / 1000.0f) * 2.0f - 1.0f;
				dir *= 1000.0f;

				static float particles = 0.0f;
				particles += dt.Milli() / 1000.0f * PARTICLES_PER_SEC;

				b2ParticleGroupDef pd;
				pd.flags = b2_solidParticleGroup;
				pd.lifetime = 10.0f;
				pd.linearVelocity = b2Vec2(dir.x, dir.y);
				util::Color clr = util::Color::CYAN;
				pd.color = b2Color(clr.r, clr.g, clr.b, clr.a);

				pd.shape = &m_pParticleEmitterShape;
				pd.position = m_pParticleEmitterPosition;
				pd.particleCount = particles;
				

				if (pd.particleCount > 0)
				{
					b2Vec2* posData = new b2Vec2[pd.particleCount];
					for (int i = 0; i < pd.particleCount; ++i)
					{
						posData[i] = b2Vec2(0, 0);
					}
					pd.positionData = posData;

					b2ParticleGroup * const group = m_pParticleSystem->CreateParticleGroup(pd);
					particles = 0.0f;
				}
			}
*/

			m_LarkController.Update(dt);

			if (!m_bIsTestbed)
			{
				g_camera.m_center = m_LarkController.GetLarkBody()->GetPosition();
			}

			//if (m_state == LARK && m_pLark != null)
			//{
			//	g_camera.m_center = m_pLark->GetPosition();

			//	float goal = Lerp(0.5f, 1.5f, Clamp(m_pLark->GetLinearVelocity().Length() / 10.0f, 0.0, 1.0f));
			//	float diff = goal - g_camera.m_zoom;
			//	if (diff > 0)
			//		g_camera.m_zoom += Min(diff, 0.005f);
			//	else
			//		g_camera.m_zoom += Max(diff, -0.005f);

			//	//apply forces
			//	if (m_bLeftMouseBtn)
			//	{
			//		b2Vec2 diff = m_pLark->GetPosition() - lastp;
			//		diff.Normalize();
			//		diff *= 1000.0f;

			//		m_pLark->ApplyForceToCenter(diff, true);
			//	}
			//}

			double delta = dt.ToDouble() / 1000;
			m_pWorld->Step(delta, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

			//Clear color buffer
			glClear(GL_COLOR_BUFFER_BIT);

			engine::render::RenderPlugin* plug = static_cast<engine::render::RenderPlugin*>(engine::IPlugin::FindPlugin(engine::render::RenderPlugin::Type));

			glm::ivec2 size;
			SDL_GetWindowSize(plug->GetSdlWindow(), &size.x, &size.y);
			glViewport(0, 0, size.x, size.y);

			int count = m_pWorld->GetBodyCount();
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
			}

			std::vector<b2dJsonImage*> images;
			m_json.getAllImages(images);
			for (int i = 0; i < images.size(); ++i)
			{
				images[i]->render();
			}

			m_debugDraw.Flush();

			//node.SetShader(&shader);
			//node(Matrix());


			SDL_GL_SwapWindow(plug->GetSdlWindow());

			return true;
		}

		void PhysicsPlugin::MakeParticles(int count, int lifetime, b2Vec2 dir)
		{
			b2ParticleGroupDef pd;
			pd.flags = b2_solidParticleGroup;
			if (lifetime > 0)
				pd.lifetime = lifetime;
			pd.linearVelocity = dir;
			util::Color clr = util::Color::CYAN;
			pd.color = b2Color(clr.r, clr.g, clr.b, clr.a);

			pd.shape = &m_pParticleEmitterShape;
			pd.position = m_pParticleEmitterPosition;
			pd.particleCount = count;
			pd.angle = angle;


			if (pd.particleCount > 0)
			{
				b2Vec2* posData = new b2Vec2[pd.particleCount];
				for (int i = 0; i < pd.particleCount; ++i)
				{
					posData[i] = b2Vec2(0, 0);
				}
				pd.positionData = posData;
				b2ParticleGroup * const group = m_pParticleSystem->CreateParticleGroup(pd);
			}
		}


		void PhysicsPlugin::Reload()
		{
			Exit();
			Init();
		}

		void PhysicsPlugin::OnKeyDown(const key_events::KeyAction& action)
		{
			switch (action.m_code)
			{
			case SDLK_t: m_bIsTestbed = !m_bIsTestbed; m_LarkController.SetTestbed(m_bIsTestbed); break;
				/*case SDLK_SPACE:
				{
				if (m_state == LARK)
				m_state = TESTBED;
				else
				m_state = LARK;
				break;
				}*/
			case SDLK_r:
				rel = true; //todo can't unsubscribe then subscribe to same event again while in the middle of callback
				break;
			case SDLK_UP:
				g_camera.m_extent += 1.0f;
				break;
			case SDLK_DOWN:
				g_camera.m_extent -= 1.0f;
				break;
			}
		}

		void PhysicsPlugin::OnMouseDown(const mouse_events::ButtonAction& action)
		{
			OnMouseDownTestbed(action);
		}

		void PhysicsPlugin::OnMouseUp(const mouse_events::ButtonAction& action)
		{
			OnMouseUpTestbed(action);
		}

		void PhysicsPlugin::OnMouseMotion(const mouse_events::MotionAction& action)
		{
			OnMouseMotionTestbed(action);
		}

		void PhysicsPlugin::OnMouseWheel(const mouse_events::WheelAction& action)
		{
			if (action.m_scroll.y > 0)
			{
				g_camera.m_zoom /= 1.1f;
			}
			else
			{
				g_camera.m_zoom *= 1.1f;
			}
		}


		void PhysicsPlugin::OnMouseDownTestbed(const mouse_events::ButtonAction& action)
		{
			if (!m_bIsTestbed)
				return;

			b2Vec2 pw = g_camera.ConvertScreenToWorld(b2Vec2(action.m_pixel.x, action.m_pixel.y));

			if (action.m_button == SDL_BUTTON_RIGHT && !m_bRightMouseBtn)
			{
				m_bRightMouseBtn = true;
				lastp = pw;
				return;
			}
			else if (action.m_button == SDL_BUTTON_LEFT)
			{

				if (m_pMouseJoint != NULL)
				{
					return;
				}

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
					md.bodyA = m_pGroundBody;
					md.bodyB = body;
					md.target = pw;
					md.maxForce = 1000.0f * body->GetMass();
					m_pMouseJoint = (b2MouseJoint*)m_pWorld->CreateJoint(&md);
					body->SetAwake(true);
				}
			}
		}
		void PhysicsPlugin::OnMouseUpTestbed(const mouse_events::ButtonAction& action)
		{
			if (!m_bIsTestbed)
				return;

			if (action.m_button == SDL_BUTTON_RIGHT)
			{
				m_bRightMouseBtn = false;
				return;
			}

			if (m_pMouseJoint)
			{
				m_pWorld->DestroyJoint(m_pMouseJoint);
				m_pMouseJoint = NULL;
			}
		}
		void PhysicsPlugin::OnMouseMotionTestbed(const mouse_events::MotionAction& action)
		{
			if (!m_bIsTestbed)
				return;

			b2Vec2 pw = g_camera.ConvertScreenToWorld(b2Vec2(action.m_pixel.x, action.m_pixel.y));

			if (m_bRightMouseBtn)
			{
				b2Vec2 diff = pw - lastp;
				g_camera.m_center.x -= diff.x;
				g_camera.m_center.y -= diff.y;
				lastp = g_camera.ConvertScreenToWorld(b2Vec2(action.m_pixel.x, action.m_pixel.y));
				return;
			}

			if (m_pMouseJoint)
			{
				m_pMouseJoint->SetTarget(pw);
			}
		}


		//void PhysicsPlugin::OnMouseDownLark(const mouse_events::ButtonAction& action)
		//{
		//	b2Vec2 pw = g_camera.ConvertScreenToWorld(b2Vec2(action.m_pixel.x, action.m_pixel.y));

		//	if (action.m_button == SDL_BUTTON_LEFT && !m_bLeftMouseBtn && m_clickType == ELarkClick::PUSH)
		//	{
		//		m_bLeftMouseBtn = true;
		//		lastp = pw;
		//		return;
		//	}
		//	
		//}
		//void PhysicsPlugin::OnMouseUpLark(const mouse_events::ButtonAction& action)
		//{

		//	if (action.m_pixel.x > 0.85 * g_camera.m_width && action.m_pixel.y < 0.15 * g_camera.m_height)
		//	{
		//		m_bLeftMouseBtn = false;
		//		/*if (m_pRopeJoint)
		//		{
		//			m_pWorld->DestroyJoint(m_pRopeJoint);
		//			m_pRopeJoint = NULL;
		//		}*/
		//		m_clickType++;
		//		if (m_clickType >= ELarkClick::COUNT)
		//		{
		//			m_clickType = ELarkClick::PUSH;
		//		}

		//		switch (m_clickType)
		//		{
		//		case ELarkClick::PUSH:
		//			affectClr = util::Color::CYAN;
		//			break;
		//		case ELarkClick::LINE:
		//			affectClr = util::Color::MAGENTA;
		//			break;
		//		case ELarkClick::BLAST:
		//			affectClr = util::Color::YELLOW;
		//			break;
		//		}
		//		return;
		//	}


		//	b2Vec2 pw = g_camera.ConvertScreenToWorld(b2Vec2(action.m_pixel.x, action.m_pixel.y));

		//	if (action.m_button == SDL_BUTTON_LEFT && m_clickType == ELarkClick::PUSH)
		//	{
		//		m_bLeftMouseBtn = false;
		//	}
		//	else if (action.m_button == SDL_BUTTON_LEFT && m_clickType == ELarkClick::BLAST)
		//	{
		//		b2Vec2 direction = pw - m_pLark->GetPosition();
		//		direction.Normalize();
		//		direction *= -100000;
		//		m_pLark->ApplyForceToCenter(direction, true);
		//	}
		//	else if ((action.m_button == SDL_BUTTON_LEFT && m_clickType == ELarkClick::LINE) && m_pRopeJoint == null)
		//	{
		//		bool connect_anywhere = false;
		//		if (!connect_anywhere)
		//		{

		//			callbacks::RayCastClosestCallback callback;
		//			b2Vec2 direction = pw - m_pLark->GetPosition();
		//			direction.Normalize();
		//			direction *= 100;
		//			b2Vec2 point = m_pLark->GetPosition() + direction;
		//			m_pWorld->RayCast(&callback, m_pLark->GetPosition(), point);

		//			if (callback.m_bHit)
		//			{
		//				b2Body* body = callback.m_pFixture->GetBody();
		//				b2RopeJointDef md;
		//				md.bodyA = m_pLark;
		//				md.bodyB = body;
		//				md.localAnchorA.SetZero();
		//				md.localAnchorB = body->GetLocalPoint(callback.m_point);
		//				md.maxLength = (callback.m_point - m_pLark->GetPosition()).Length() * 1.1f;
		//				md.collideConnected = true;
		//				m_pRopeJoint = (b2RopeJoint*)m_pWorld->CreateJoint(&md);
		//				body->SetAwake(true);
		//			}
		//		}
		//		else
		//		{
		//			//conect anywhere
		//			b2Body* body = m_pLark;
		//			b2RopeJointDef md;
		//			md.bodyA = m_pGroundBody;
		//			md.bodyB = body;
		//			md.localAnchorA = pw;
		//			md.localAnchorB.SetZero();
		//			md.maxLength = (pw - m_pLark->GetPosition()).Length() * 1.1f;
		//			m_pRopeJoint = (b2RopeJoint*)m_pWorld->CreateJoint(&md);
		//			body->SetAwake(true);
		//		}
		//	}
		//	else if (action.m_button == SDL_BUTTON_LEFT && m_clickType == ELarkClick::LINE)
		//	{
		//		m_pWorld->DestroyJoint(m_pRopeJoint);
		//		m_pRopeJoint = NULL;
		//	}

		//}
		//void PhysicsPlugin::OnMouseMotionLark(const mouse_events::MotionAction& action)
		//{
		//	b2Vec2 pw = g_camera.ConvertScreenToWorld(b2Vec2(action.m_pixel.x, action.m_pixel.y));

		//	if (m_bLeftMouseBtn)
		//	{
		//		lastp = pw;
		//	}
		//}

	}
}
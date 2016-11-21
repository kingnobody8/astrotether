#pragma once
#include "player_ent.h"
#include "render/render_plugin.h"
#include "utility/helper/func.h"
#include <string>
#include <regex>
#include "state/state_plugin.h"
#include "states/testbed_state.h"
#include "physics/physics_plugin.h"
#include "physics/callbacks/raycast_callback.h"
#include "utility/resource/json.h"
#include "rapidjson/FileReadStream.h"

const std::string path = "../spine_examples/REP/export/REP";
//const std::string path = "assets/animations/REP/export/REP";
const std::string file = "spineboy";
spine::AnimationStateData* pData = null;

namespace app
{
	namespace entity
	{
		void PlayerValue::LoadValues(const std::string& file_path)
		{
			FILE* fp = fopen(file_path.c_str(), "rb"); // non-Windows use "r"
			char readBuffer[65536];
			rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
			rapidjson::Document d;
			d.ParseStream(is);
			fclose(fp);

			util::JSON json(d);

			m_fMaxSpeed = json["max_speed"].GetDouble();
			m_fGroundAcceleration = json["ground_acceleration"].GetDouble();
			m_fGroundDeceleration = json["ground_deceleration"].GetDouble();
			m_fAirAcceleration = json["air_acceleration"].GetDouble();
			m_fAirDeceleration = json["air_deceleration"].GetDouble();
			m_fJumpImpulse = json["jump_impulse"].GetDouble();
		}

		const std::string PlayerValue::GetAsString() const
		{
			std::string ret;

			ret += std::string("Max Speed:\t") + std::to_string(m_fMaxSpeed) + std::string("\n");
			ret += std::string("Grnd Accel:\t") + std::to_string(m_fGroundAcceleration) + std::string("\n");
			ret += std::string("Grnd Decel:\t") + std::to_string(m_fGroundDeceleration) + std::string("\n");
			ret += std::string("Air Accel:\t") + std::to_string(m_fAirAcceleration) + std::string("\n");
			ret += std::string("Air Decel:\t") + std::to_string(m_fAirDeceleration) + std::string("\n");
			ret += std::string("Jump Impulse:\t") + std::to_string(m_fJumpImpulse) + std::string("\n");
			return ret;
		}


		PlayerEnt::PlayerEnt(b2Body* pBody)
			: m_pBody(pBody)
			, m_pRopeJoint(null)
			, m_pAtlas(null)
			, m_pSkelData(null)
			, m_pDrawable(null)
			, m_pGroundSensor(null)
		{
			b2Fixture* pFixture = pBody->GetFixtureList();
			while (pFixture != null)
			{
				if (pFixture->IsSensor())
				{
					m_pGroundSensor = pFixture;
					break;
				}

				pFixture = pFixture->GetNext();
			}

		}

		VIRTUAL PlayerEnt::~PlayerEnt()
		{
		}

		VIRTUAL void PlayerEnt::Init()
		{
			m_tValue.LoadValues("assets/player_values.json");

			std::string file_path = std::regex_replace(path, std::regex("REP"), file);
			m_pAtlas = spine::Atlas::createFromFile((file_path + std::string(".atlas")).c_str(), null);
			spine::SkeletonJson json(*m_pAtlas);
			m_pSkelData = json.readSkeletonDataFile((file_path + std::string(".json")).c_str());
			pData = new spine::AnimationStateData(*m_pSkelData);
			m_pDrawable = new spine::SkeletonDrawable(m_pSkelData, pData);

			//m_pDrawable->state->listener = BIND5(this, &LogoEnt::AnimationListenerCallback);

			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			pRenderPlug->AddDrawable(m_pDrawable, "physics");

			m_pDrawable->skeleton->setBonesToSetupPose();
			m_pDrawable->scale(0.004f, -0.004f);
			m_pDrawable->state->setAnimationByName(0, "idle", true);
			//pData = new spine::AnimationStateData();
			//	pData->setMixByName("walk", "idle", 5.0f);
			//m_pDrawable->state->data.defaultMix = 2.0f;

			baka::key_events::s_InputKeyDown.Subscribe(this, BIND1(this, &PlayerEnt::OnKeyDown));
			baka::key_events::s_InputKeyUp.Subscribe(this, BIND1(this, &PlayerEnt::OnKeyUp));

			baka::mouse_events::s_InputMouseButtonDown.Subscribe(this, BIND1(this, &PlayerEnt::OnMouseDown));
			baka::mouse_events::s_InputMouseButtonUp.Subscribe(this, BIND1(this, &PlayerEnt::OnMouseUp));
			baka::mouse_events::s_InputMouseMotion.Subscribe(this, BIND1(this, &PlayerEnt::OnMouseMove));

		}

		VIRTUAL void PlayerEnt::Exit()
		{

			baka::render::RenderPlugin* renderer = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			if (renderer != null)
			{
				renderer->RemDrawable(m_pDrawable);
				m_pDrawable->state->clearTracks();
				delete m_pDrawable;
			}

			delete m_pAtlas;
			delete m_pSkelData;
			delete pData;

			m_pAtlas = null;
			m_pSkelData = null;
			m_pDrawable = null;
		}

		VIRTUAL void PlayerEnt::Update(const sf::Time& dt)
		{
			for (int i = 0; i < EButton::EB_COUNT; ++i)
			{
				m_vButtons[i].Flush(dt.asSeconds());
			}

			bool grounded = CheckGrounded();
			if (m_vButtons[EB_JUMP].Push() && grounded)
			{
				m_pBody->ApplyLinearImpulse(b2Vec2(0, m_tValue.m_fJumpImpulse), m_pBody->GetLocalCenter(), true);
			}


			m_pDrawable->update(dt.asSeconds());
			m_pDrawable->setPosition(m_pBody->GetPosition().x, -m_pBody->GetPosition().y);
		//	m_pDrawable->setRotation(m_pBody->GetAngle() * -RAD_DEG);

			b2Vec2 vel = m_pBody->GetLinearVelocity();
			float desiredVel = 0;
			if (m_vButtons[EB_RIGHT].Held())
			{
				if (grounded)
					desiredVel = b2Min(vel.x + m_tValue.m_fGroundAcceleration, m_tValue.m_fMaxSpeed);
				else
					desiredVel = b2Min(vel.x + m_tValue.m_fAirAcceleration, m_tValue.m_fMaxSpeed);
			}
			if (m_vButtons[EB_LEFT].Held())
			{
				if (grounded)
					desiredVel = b2Max(vel.x - m_tValue.m_fGroundAcceleration, -m_tValue.m_fMaxSpeed);
				else
					desiredVel = b2Max(vel.x - m_tValue.m_fAirAcceleration, -m_tValue.m_fMaxSpeed);
			}
			if (desiredVel == 0) //apply deceleration
			{
				if (grounded)
					desiredVel = vel.x * m_tValue.m_fGroundDeceleration;
				else
					desiredVel = vel.x * m_tValue.m_fAirDeceleration;
			}
			float velChange = desiredVel - vel.x;
			float impulse = m_pBody->GetMass() * velChange; //disregard time factor
			m_pBody->ApplyLinearImpulse(b2Vec2(impulse, 0), m_pBody->GetWorldCenter(), true);


			if (desiredVel > 0 )
				m_pDrawable->skeleton->flipX = false;
			else if (desiredVel < 0)
				m_pDrawable->skeleton->flipX = true;


			//b2Vec2 vel = m_pBody->GetLinearVelocity();
			//float desiredVel = 0;
			//bool applyForce = false;
			//if (m_vButtons[EB_RIGHT].Held())
			//{
			//	applyForce = !applyForce;
			//	desiredVel = 5;
			//}
			//if (m_vButtons[EB_LEFT].Held())
			//{
			//	applyForce = !applyForce; //instead of setting it to true, i use (not) applyForce because if both left and right are being held they should cancel eachother out
			//	desiredVel = -5;
			//}
			//float velChange = desiredVel - vel.x;
			//float force = m_pBody->GetMass() * velChange / dt.asSeconds()/*(1 / 60.0)*/; //f = mv/t
			//if (force != 0)
			//	m_pBody->ApplyForceToCenter(b2Vec2(force, 0), true);

			


			//b2Vec2 linVel = m_pBody->GetLinearVelocity();
			//float xForce = TOPSPEED - abs(linVel.x);

			//bool applyForce = false;
			//if (m_vButtons[EB_RIGHT].Held())
			//{
			//	applyForce = !applyForce;
			//}
			//if (m_vButtons[EB_LEFT].Held())
			//{
			//	applyForce = !applyForce; //instead of setting it to true, i use (not) applyForce because if both left and right are being held they should cancel eachother out
			//	xForce *= -1;
			//}

			//if (applyForce)
			//{
			//	if (grounded)
			//		xForce *= GRND_ACC;
			//	else
			//		xForce *= AIR_ACC;

			//	m_pBody->ApplyForceToCenter(b2Vec2(xForce, 0), true);
			//}
			//if (xForce > 0)
			//	m_pDrawable->skeleton->flipX = false;
			//else if (xForce < 0)
			//	m_pDrawable->skeleton->flipX = true;

			//if (mMoveRight) {
			//	inputXForce = (mMaxXVelocity - xVel);
			//}
			//if (onGround)
			//	inputXForce *= mGroundXAcceleration;
			//else
			//	inputXForce *= mAirXAcceleration;


			/*const float Xvel = 100.0f * dt.asSeconds();
			const float Yvel = 100.0f * dt.asSeconds();
			b2Vec2 dir;
			dir.x = dir.y = 0;*/

			/*if (m_vButtons[EB_RIGHT].Held())
				dir.x += Xvel;
				if (m_vButtons[EB_LEFT].Held())
				dir.x -= Xvel;*/

			/*
		if (m_vDirections[ED_UP])
		dir.y += Yvel;
		if (m_vDirections[ED_DOWN])
		dir.y -= Yvel;*/


			/*if (abs(linVel.x) >= TOPSPEED && ((linVel.x > 0 && dir.x > 0) || (linVel.x < 0 && dir.x < 0)))
				dir.x = 0;
				if (abs(linVel.y) >= TOPSPEED && ((linVel.y > 0 && dir.y > 0) || (linVel.y < 0 && dir.y < 0)))
				dir.y = 0;
				vel.x += dir.x;*/

			//m_pBody->SetLinearVelocity(vel);


		}


		void PlayerEnt::OnKeyDown(const baka::key_events::KeyAction& action)
		{
			switch (action.m_code)
			{
			case sf::Keyboard::Left:
			case sf::Keyboard::A:
			{
				m_vButtons[EButton::EB_LEFT].Next(true);
			}break;
			case sf::Keyboard::Right:
			case sf::Keyboard::D:
			{
				m_vButtons[EButton::EB_RIGHT].Next(true);
			}break;
			case sf::Keyboard::Up:
			case sf::Keyboard::W:
			case sf::Keyboard::Space:
			{
				m_vButtons[EButton::EB_JUMP].Next(true);
			}break;
			}


			//bool gotoWalk = false;
			//if (action.m_code == sf::Keyboard::Right || action.m_code == sf::Keyboard::D)
			//{
			//	m_vDirections[ED_RIGHT] = true;
			//	gotoWalk = true;
			//}
			//if (action.m_code == sf::Keyboard::Left || action.m_code == sf::Keyboard::A)
			//{
			//	m_vDirections[ED_LEFT] = true;
			//	gotoWalk = true;
			//}
			//if (action.m_code == sf::Keyboard::Up || action.m_code == sf::Keyboard::W)
			//	m_vDirections[ED_UP] = true;
			//if (action.m_code == sf::Keyboard::Down || action.m_code == sf::Keyboard::S)
			//	m_vDirections[ED_DOWN] = true;

			//if (action.m_code == sf::Keyboard::Space && !m_vDirections[ED_SPACE])
			//{
			//	m_vDirections[ED_SPACE] = true;

			//	//baka::physics::callbacks::AabbCallback callback(b2Vec2());
			//	callback = baka::physics::callbacks::AabbCallback(b2Vec2());
			//	m_pBody->GetWorld()->QueryAABB(&callback, m_pGroundSensor->GetAABB(0));

			//	if (callback.m_pFixture)
			//	{
			//		b2Body* body = callback.m_pFixture->GetBody();
			//		m_pBody->SetLinearVelocity(b2Vec2(m_pBody->GetLinearVelocity().x, 10));
			//	}

			//	//m_pDrawable->state->setAnimationByName(0, "jump", false);
			//}

			//if (gotoWalk && m_pDrawable->state->getCurrent(0)->animation.name != "run")
			//{
			//	m_pDrawable->state->setAnimationByName(0, "run", true);
			//}
		}

		void PlayerEnt::OnKeyUp(const baka::key_events::KeyAction& action)
		{
			switch (action.m_code)
			{
			case sf::Keyboard::Left:
			case sf::Keyboard::A:
			{
				m_vButtons[EButton::EB_LEFT].Next(false);
			}break;
			case sf::Keyboard::Right:
			case sf::Keyboard::D:
			{
				m_vButtons[EButton::EB_RIGHT].Next(false);
			}break;
			case sf::Keyboard::Up:
			case sf::Keyboard::W:
			case sf::Keyboard::Space:
			{
				m_vButtons[EButton::EB_JUMP].Next(false);
			}break;
			case sf::Keyboard::R:
			{
				m_tValue.LoadValues("assets/player_values.json");
			}break;
			}


			//		bool gotoIdle = false;
			//		if (action.m_code == sf::Keyboard::Right || action.m_code == sf::Keyboard::D)
			//		{
			//			m_vDirections[ED_RIGHT] = false;
			//			gotoIdle = true;
			//			m_pBody->SetLinearVelocity(b2Vec2(0, m_pBody->GetLinearVelocity().y));
			//		}
			//		if (action.m_code == sf::Keyboard::Left || action.m_code == sf::Keyboard::A)
			//		{
			//			m_vDirections[ED_LEFT] = false;
			//			gotoIdle = true;
			//			m_pBody->SetLinearVelocity(b2Vec2(0, m_pBody->GetLinearVelocity().y));
			//		}
			//		if (action.m_code == sf::Keyboard::Up || action.m_code == sf::Keyboard::W)
			//			m_vDirections[ED_UP] = false;
			//		if (action.m_code == sf::Keyboard::Down || action.m_code == sf::Keyboard::S)
			//			m_vDirections[ED_DOWN] = false;

			//		if (action.m_code == sf::Keyboard::Space)
			//		{
			//			m_vDirections[ED_SPACE] = false;
			//		}

			//		if (gotoIdle&& m_pDrawable->state->getCurrent(0)->animation.name != "idle")
			//		{
			//			pData->setMixByName("run", "idle", 1.0f);
			//			//m_pDrawable->state->data = pData;
			//			//m_pDrawable->state->data.setMixByName("idle", "walk", 5.0f);
			//			auto track = m_pDrawable->state->addAnimationByName(0, "idle", true, 0.0f);
			//			//track->mixTime = 5.0f;
			////			track->mixDuration = 5.0f;
			//		}
		}

		void PlayerEnt::OnMouseDown(const baka::mouse_events::ButtonAction& action)
		{
		}

		void PlayerEnt::OnMouseUp(const baka::mouse_events::ButtonAction& action)
		{
			if (action.m_button == sf::Mouse::Right)
				OnRopeEvent(action.m_pixel);
		}

		void PlayerEnt::OnMouseMove(const baka::mouse_events::MotionAction& action)
		{
		}


		void PlayerEnt::OnRopeEvent(const sf::Vector2i& screenPos)
		{
			if (m_pRopeJoint == NULL)
			{
				baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
				baka::physics::PhysicsPlugin* pPhysicsPlug = static_cast<baka::physics::PhysicsPlugin*>(baka::IPlugin::FindPlugin(baka::physics::PhysicsPlugin::Type));
				sf::Vector2f worldCoords = pRenderPlug->GetRenderWindow()->mapPixelToCoords(screenPos, pPhysicsPlug->GetView());
				worldCoords.y *= -1;

				b2Vec2 pw(worldCoords.x, worldCoords.y);
				baka::physics::callbacks::RayCastClosestCallback callback;
				callback.m_pIgnore = m_pBody;
				b2Vec2 direction = pw - m_pBody->GetPosition();
				direction.Normalize();
				direction *= 100;
				b2Vec2 point = m_pBody->GetPosition() + direction;
				m_pBody->GetWorld()->RayCast(&callback, m_pBody->GetPosition() + b2Vec2(0,0.5), point);

				if (callback.m_bHit)
				{
					b2Body* body = callback.m_pFixture->GetBody();
					b2RopeJointDef md;
					md.bodyA = m_pBody;
					md.bodyB = body;
					md.localAnchorA = b2Vec2(0, 0.5);// .SetZero();
					md.localAnchorB = body->GetLocalPoint(callback.m_point);
					md.maxLength = (callback.m_point - m_pBody->GetPosition()).Length() * 1.1f;
					md.collideConnected = true;
					m_pRopeJoint = (b2RopeJoint*)m_pBody->GetWorld()->CreateJoint(&md);
					body->SetAwake(true);
				}
			}
			else
			{
				m_pBody->GetWorld()->DestroyJoint(m_pRopeJoint);
				m_pRopeJoint = NULL;
			}
		}

		bool PlayerEnt::CheckGrounded()
		{
			return true;

			baka::physics::PhysicsPlugin* pPhysicsPlug = static_cast<baka::physics::PhysicsPlugin*>(baka::IPlugin::FindPlugin(baka::physics::PhysicsPlugin::Type));
			b2dJson* json = pPhysicsPlug->GetJson();
			//std::vector<b2Body*> vGroundBodies;
			//json.getBodiesByCustomBool("is_ground", true, vGroundBodies);
			//json.custombool

			baka::physics::callbacks::AabbCallbackAll callback;
			m_pBody->GetWorld()->QueryAABB(&callback, m_pGroundSensor->GetAABB(0));


			for (int i = 0; i < callback.m_vFixtures.size(); ++i)
			{
				b2Body* body = callback.m_vFixtures[i]->GetBody();

				bool is_ground = json->getCustomBool(body, "is_ground", false);
				if (is_ground)
				{
					return true;
				}
			}

			return false;
		}
	}
}
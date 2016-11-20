#pragma once
#include "player_ent.h"
#include "render/render_plugin.h"
#include "utility/helper/func.h"
#include <string>
#include <regex>
#include "state/state_plugin.h"
#include "states/testbed_state.h"
#include "physics/physics_plugin.h"

const float TOPSPEED = 10.0f;
const std::string path = "../spine_examples/REP/export/REP";
//const std::string path = "assets/animations/REP/export/REP";
const std::string file = "spineboy";
spine::AnimationStateData* pData = null;

namespace app
{
	namespace entity
	{
		PlayerEnt::PlayerEnt(b2Body* pBody)
			: m_pBody(pBody)
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
			baka::mouse_events::s_InputMouseButtonUp.Subscribe(this, BIND1(this, &PlayerEnt::OnMouseDown));
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

			if (m_vButtons[EB_JUMP].Push())
			{
				bool grounded = CheckGrounded();

				if (grounded)
				{
					m_pBody->ApplyLinearImpulse(b2Vec2(0, 30), m_pBody->GetLocalCenter(), true);
				}
			}


			m_pDrawable->update(dt.asSeconds());
			m_pDrawable->setPosition(m_pBody->GetPosition().x, -m_pBody->GetPosition().y);

			const float Xvel = 10.0f;
			const float Yvel = 10.0f;
			b2Vec2 dir;
			dir.x = dir.y = 0;

			if (m_vButtons[EB_RIGHT].Held())
				dir.x += Xvel;
			if (m_vButtons[EB_LEFT].Held())
				dir.x -= Xvel;

				/*
			if (m_vDirections[ED_UP])
				dir.y += Yvel;
			if (m_vDirections[ED_DOWN])
				dir.y -= Yvel;*/

			b2Vec2 vel = m_pBody->GetLinearVelocity();

			if (abs(vel.x) >= TOPSPEED && ((vel.x > 0 && dir.x > 0) || (vel.x < 0 && dir.x < 0)))
				dir.x = 0;
			if (abs(vel.y) >= TOPSPEED && ((vel.y > 0 && dir.y > 0) || (vel.y < 0 && dir.y < 0)))
				dir.y = 0;
		
			m_pBody->ApplyForceToCenter(dir, true);
			if (dir.x > 0)
				m_pDrawable->skeleton->flipX = false;
			else if (dir.x < 0)
				m_pDrawable->skeleton->flipX = true;
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
		}

		void PlayerEnt::OnMouseMove(const baka::mouse_events::MotionAction& action)
		{
		}


		bool PlayerEnt::CheckGrounded()
		{
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
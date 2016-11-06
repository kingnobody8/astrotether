#pragma once
#include "player_ent.h"
#include "render/render_plugin.h"
#include "utility/helper/func.h"
#include <string>
#include <regex>
#include "state/state_plugin.h"
#include "states/testbed_state.h"

const float TOPSPEED = 1.0f;
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
		{
			for (int i = 0; i < EDirection::ED_COUNT; ++i)
				m_vDirections[i] = false;
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
			m_pDrawable->scale(0.005f, -0.005f);
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
			m_pDrawable->update(dt.asSeconds());
			m_pDrawable->setPosition(m_pBody->GetPosition().x, -m_pBody->GetPosition().y);

			const float Xvel = 10.0f;
			const float Yvel = 10.0f;
			b2Vec2 dir;
			dir.x = dir.y = 0;

			if (m_vDirections[ED_RIGHT])
				dir.x += Xvel;
			if (m_vDirections[ED_LEFT])
				dir.x -= Xvel;
			if (m_vDirections[ED_UP])
				dir.y += Yvel;
			if (m_vDirections[ED_DOWN])
				dir.y -= Yvel;

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
			bool gotoWalk = false;
			if (action.m_code == sf::Keyboard::Right || action.m_code == sf::Keyboard::D)
			{
				m_vDirections[ED_RIGHT] = true;
				gotoWalk = true;
			}
			if (action.m_code == sf::Keyboard::Left || action.m_code == sf::Keyboard::A)
			{
				m_vDirections[ED_LEFT] = true;
				gotoWalk = true;
			}
			if (action.m_code == sf::Keyboard::Up || action.m_code == sf::Keyboard::W)
				m_vDirections[ED_UP] = true;
			if (action.m_code == sf::Keyboard::Down || action.m_code == sf::Keyboard::S)
				m_vDirections[ED_DOWN] = true;

			if (gotoWalk && m_pDrawable->state->getCurrent(0)->animation.name != "walk")
			{
				m_pDrawable->state->setAnimationByName(0, "walk", true);
			}
		}

		void PlayerEnt::OnKeyUp(const baka::key_events::KeyAction& action)
		{
			bool gotoIdle = false;
			if (action.m_code == sf::Keyboard::Right || action.m_code == sf::Keyboard::D)
			{
				m_vDirections[ED_RIGHT] = false;
				gotoIdle = true;
			}
			if (action.m_code == sf::Keyboard::Left || action.m_code == sf::Keyboard::A)
			{
				m_vDirections[ED_LEFT] = false;
				gotoIdle = true;
			}
			if (action.m_code == sf::Keyboard::Up || action.m_code == sf::Keyboard::W)
				m_vDirections[ED_UP] = false;
			if (action.m_code == sf::Keyboard::Down || action.m_code == sf::Keyboard::S)
				m_vDirections[ED_DOWN] = false;

			if (gotoIdle&& m_pDrawable->state->getCurrent(0)->animation.name != "idle")
			{
				pData->setMixByName("walk", "idle", 1.0f);
				//m_pDrawable->state->data = pData;
				//m_pDrawable->state->data.setMixByName("idle", "walk", 5.0f);
				auto track = m_pDrawable->state->addAnimationByName(0, "idle", true, 0.0f);
				//track->mixTime = 5.0f;
	//			track->mixDuration = 5.0f;
			}
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

	}
}
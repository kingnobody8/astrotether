#include "knock_off_state.h"

#include "input/input_plugin.h"
#include "render/render_plugin.h"
#include "state/state_plugin.h"
#include "entity/entity_plugin.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "rapidjson/FileReadStream.h"
#include <cstdio>
#include "resource/json.h"

#include "input/input_event.h"
#include "engine/b2djson/b2dJsonImage.h"
#include "render/box2d-sfml.h"
#include "entities/goal_ent.h"

const float startPosY = -32;
const float posRain = 48;
const float transTime = 9.0f;

namespace app
{
	namespace state
	{
		KnockOffState::KnockOffState()
		{
			m_vPlayer[0] = m_vPlayer[1] = null;
			this->m_name = "KnockOff";
		}

		VIRTUAL void KnockOffState::Init()
		{
			m_soundBufferWin.loadFromFile("assets/sfx/win.wav");
			m_rain.openFromFile("assets/sfx/rain.wav");
			m_rain.play();
			m_rain.setLoop(true);

			m_pPhysicsPlugin = FIND_PLUGIN(baka::physics::PhysicsPlugin);
			baka::render::RenderPlugin* pRendPlug = FIND_PLUGIN(baka::render::RenderPlugin);

			sf::View& view = m_pPhysicsPlugin->GetView();
			float scale = 30.0f;
			view.setSize(1280 / scale, 720 / scale);
			view.setCenter(0, startPosY);
			//view.zoom(1.25);

			std::string path = "assets/worlds/";
			std::string file = "testbed.json";

			b2dJson* json = m_pPhysicsPlugin->LoadWorld(path, file);

			b2dJsonImage* pImage = json->getImageByCustomString("handle", "lights_bg");
			auto imgs = m_pPhysicsPlugin->FindImagesByBody(pImage->body);
			m_pBGLights = imgs[0];
			pImage = json->getImageByCustomString("handle", "nm_lights");
			imgs = m_pPhysicsPlugin->FindImagesByBody(pImage->body);
			m_pNMLights = imgs[0];
			pRendPlug->RemDrawable(m_pNMLights);

			baka::entity::EntityPlugin* pEntPlug = FIND_PLUGIN(baka::entity::EntityPlugin)
				m_pLogo = static_cast<entity::LogoEnt*>(pEntPlug->FindEntities("LogoEnt")[0]);
			m_pRain = static_cast<entity::RainEnt*>(pEntPlug->FindEntities("RainEnt")[0]);
			m_pGoal = static_cast<entity::GoalEnt*>(pEntPlug->FindEntities("GoalEnt")[0]);

			auto blocks = pEntPlug->FindEntities("BlockEnt");
			for (int i = 0; i < blocks.size(); ++i)
			{
				m_vBlocks.push_back(static_cast<entity::BlockEnt*>(blocks[i]));
			}

			std::vector<baka::entity::IEntity*> vEnts = pEntPlug->FindEntities("PlayerEnt");
			entity::PlayerEnt *pTmp1, *pTmp2;
			pTmp1 = static_cast<entity::PlayerEnt*>(vEnts[0]);
			pTmp2 = static_cast<entity::PlayerEnt*>(vEnts[1]);

			if (pTmp1->GetPlayerId() == 0)
			{
				m_vPlayer[0] = pTmp1;
				m_vPlayer[1] = pTmp2;
			}
			else
			{
				m_vPlayer[0] = pTmp2;
				m_vPlayer[1] = pTmp1;
			}

			FindControllers();

			BeginPan();
			baka::key_events::s_InputKeyUp.Subscribe(&sub, BIND1(this, &KnockOffState::OnKeyUp));

			baka::joypad_events::s_InputJoypadConnected.Subscribe(&sub, BIND1(this, &KnockOffState::OnJoystickConnected));
			baka::joypad_events::s_InputJoypadDisconnected.Subscribe(&sub, BIND1(this, &KnockOffState::OnJoystickDisconnected));

			entity::GoalEnt::s_ScoreGoal.Subscribe(&sub, BIND1(this, &KnockOffState::OnGoal));

			//b2Body* pPlayerBody = json->getBodyByName("player");
			//	m_pPlayer = CreateEntityMacro(pEntPlug, entity::PlayerEnt);
			//m_pPlayer->Setup(pPlayerBody);
		}

		VIRTUAL void KnockOffState::Exit()
		{
			baka::entity::EntityPlugin* pEntPlug = static_cast<baka::entity::EntityPlugin*>(baka::IPlugin::FindPlugin(baka::entity::EntityPlugin::Type));
			if (pEntPlug)
				pEntPlug->DestroyAllEntities();
		}

		VIRTUAL const std::string KnockOffState::DebugRender(sf::RenderWindow* pRenWin)
		{
			std::string ret;
			return ret;
		}

		VIRTUAL void KnockOffState::Update(const sf::Time& dt)
		{
			static bool once = false;
			if (m_timer.asSeconds() > 0)
			{
				m_timer -= dt;

				m_pPhysicsPlugin = FIND_PLUGIN(baka::physics::PhysicsPlugin)

					sf::View& view = m_pPhysicsPlugin->GetView();
				float amt = m_timer.asSeconds() / transTime;
				float posY = Lerp(startPosY, 0, 1.0f - amt);
				view.setCenter(0, posY);

				m_pRain->SetPosY(Lerp(posRain, 16, 1.0f - amt));
			}
			else if (m_pLogo->GetFinishedAnim() && !once)
			{
				once = true;
				for (int i = 0; i < m_vBlocks.size(); ++i)
				{
					m_vBlocks[i]->Explode(b2Vec2());
				}
				baka::render::RenderPlugin* pRendPlug = FIND_PLUGIN(baka::render::RenderPlugin);
				pRendPlug->AddDrawable(m_pNMLights, "physics-1");
				pRendPlug->RemDrawable(m_pBGLights);
				m_startDelay = sf::seconds(3);
			}

			if (m_startDelay.asSeconds() > 0)
			{
				m_startDelay -= dt;
				if (m_startDelay.asSeconds() <= 0)
				{
					m_pGoal->SetScored(false);
				}
			}
		}

		void KnockOffState::BeginPan()
		{
			m_timer = sf::seconds(transTime);
		}

		void KnockOffState::FindControllers()
		{
			int num = 0;
			for (int i = 0; i < 4; ++i)
			{
				if (sf::Joystick::isConnected(i))
				{
					num++;
				}
			}

			m_vPlayer[0]->SetJoystickId(-1);
			m_vPlayer[1]->SetJoystickId(-1);

			for (int i = 0; i < 4; ++i)
			{
				if (sf::Joystick::isConnected(i))
				{
					if (num <= 1)
					{
						for (int j = 1; j >= 0; --j)
						{
							if (m_vPlayer[j]->GetJoystickId() == -1)
							{
								m_vPlayer[j]->SetJoystickId(i);
								break;
							}
						}
					}
					else
					{
						for (int j = 0; j < 2; ++j)
						{
							if (m_vPlayer[j]->GetJoystickId() == -1)
							{
								m_vPlayer[j]->SetJoystickId(i);
								break;
							}
						}
					}
				}
			}
		}

		void KnockOffState::OnJoystickConnected(const int id)
		{
			FindControllers();
		}

		void KnockOffState::OnJoystickDisconnected(const int id)
		{
			FindControllers();
		}

		void KnockOffState::OnKeyUp(const baka::key_events::KeyAction& action)
		{
			switch (action.m_code)
			{
			case sf::Keyboard::C:
			{
				FindControllers();
			} break;
			}
		}

		void KnockOffState::OnGoal(int i)
		{
			baka::render::RenderPlugin* pRendPlug = FIND_PLUGIN(baka::render::RenderPlugin);
			pRendPlug->AddDrawable(m_pBGLights, "physics-1");
			pRendPlug->RemDrawable(m_pNMLights);

			m_sound.setBuffer(m_soundBufferWin);
			m_sound.play();
		}
	}
}
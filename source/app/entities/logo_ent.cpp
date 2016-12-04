#pragma once
#include "logo_ent.h"
#include "render/render_plugin.h"
#include "utility/helper/func.h"
#include <string>
#include <regex>
#include "state/state_plugin.h"
#include "states/testbed_state.h"

namespace app
{
	namespace entity
	{
		//const std::string path = "../spine_examples/REP/export/REP";
		const std::string path = "assets/animations/REP/export/REP";
		const std::string file = "logo";
		const float time_delay = 2.0f;


		LogoEnt::LogoEnt()
			: m_pAtlas(null)
			, m_pSkelData(null)
			, m_pDrawable(null)
			, m_bIsAnimOver(false)
		{
		}

		VIRTUAL LogoEnt::~LogoEnt()
		{
		}

		VIRTUAL void LogoEnt::Init()
		{
			std::string file_path = std::regex_replace(path, std::regex("REP"), file);
			m_pAtlas = spine::Atlas::createFromFile((file_path + std::string(".atlas")).c_str(), null);
			spine::SkeletonJson json(*m_pAtlas);
			m_pSkelData = json.readSkeletonDataFile((file_path + std::string(".json")).c_str());
			m_pDrawable = new spine::SkeletonDrawable(m_pSkelData);

			m_pDrawable->state->listener = BIND5(this, &LogoEnt::AnimationListenerCallback);

			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			pRenderPlug->AddDrawable(m_pDrawable, "main");

			baka::key_events::s_InputKeyDown.Subscribe(this, BIND1(this, &LogoEnt::OnKeyDown));
			baka::mouse_events::s_InputMouseButtonDown.Subscribe(this, BIND1(this, &LogoEnt::OnMouseDown));
			baka::joypad_events::s_InputJoypadButtonDown.Subscribe(this, BIND1(this, &LogoEnt::OnJoystickButtonDown));

			m_pDrawable->skeleton->setBonesToSetupPose();
			m_pDrawable->state->setAnimationByName(0, "animation", false);
		}

		VIRTUAL void LogoEnt::Exit()
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

			m_pAtlas = null;
			m_pSkelData = null;
			m_pDrawable = null;
		}

		VIRTUAL void LogoEnt::Update(const sf::Time& dt)
		{
			m_pDrawable->update(dt.asSeconds());
			if (m_bIsAnimOver)
			{
				m_timer += dt;
				if (m_timer.asSeconds() > time_delay)
				{
					GotoNextState();
				}
			}
		}


		void LogoEnt::OnKeyDown(const baka::key_events::KeyAction& action)
		{
			SnapToEnd();
		}

		void LogoEnt::OnMouseDown(const baka::mouse_events::ButtonAction& action)
		{
			SnapToEnd();
		}

		void LogoEnt::OnJoystickButtonDown(const baka::joypad_events::ButtonAction& action)
		{
			SnapToEnd();
		}


		void LogoEnt::SnapToEnd()
		{
			GotoNextState();
		}

		void LogoEnt::AnimationListenerCallback(spine::AnimationState& state, int trackIndex, spine::EventType type, const spine::Event* event, int loopCount)
		{
			if (type == spine::EventType::Anim_End)
				m_bIsAnimOver = true;
		}

		void LogoEnt::GotoNextState()
		{
			baka::state::StatePlugin* pStatePlug = static_cast<baka::state::StatePlugin*>(baka::IPlugin::FindPlugin(baka::state::StatePlugin::Type));
			pStatePlug->TransitionState(new state::TestbedState());
		}

	}
}
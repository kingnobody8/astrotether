#include "splash_state.h"

#include "input/input_plugin.h"
#include "render/render_plugin.h"
#include "state/state_plugin.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "rapidjson/FileReadStream.h"
#include <cstdio>
#include "resource/json.h"

#include "input/input_event.h"
#include "testbed_state.h"

namespace app
{
	namespace state
	{
		SplashState::SplashState()
			: m_bAnimOver(false)
		{
			this->m_name = "SplashState";
		}

		VIRTUAL SplashState::~SplashState()
		{
		}

		VIRTUAL void SplashState::Init()
		{
			//std::string path = "../spine_examples/spineboy/export/spineboy";
			std::string path = "assets/animations/logo/export/logo";
			atlas = spine::Atlas::createFromFile((path + std::string(".atlas")).c_str(), null);
			spine::SkeletonJson json(*atlas);
			data = json.readSkeletonDataFile((path + std::string(".json")).c_str());
			draw = new spine::SkeletonDrawable(data);


			draw->state->listener = BIND5(this, &SplashState::AnimationListenerCallback);

			baka::render::RenderPlugin* renderer = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			renderer->AddDrawable(draw);

			baka::key_events::s_InputKeyUp.Subscribe(&sub, BIND1(this, &SplashState::OnKeyUp));
			baka::mouse_events::s_InputMouseMotion.Subscribe(&sub, BIND1(this, &SplashState::OnMouseMove));
		}

		VIRTUAL void SplashState::Exit()
		{
			baka::render::RenderPlugin* renderer = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			if (renderer != null)
			{
				renderer->RemDrawable(draw);
				draw->state->clearTracks();
				delete draw;
			}

			delete atlas;
			delete data;

		}

		VIRTUAL const std::string SplashState::DebugRender(sf::RenderWindow* pRenWin)
		{

			std::string ret;
			auto curr = draw->state->getCurrent(0);
			if (curr)
			{
				ret += std::to_string(curr->time);
			}

			return ret;
		}

		VIRTUAL void SplashState::Update(const sf::Time& dt)
		{
			draw->update(dt.asSeconds());

			if (m_bAnimOver)
			{
				m_counter += dt;
				if (m_counter.asSeconds() > 2.0f)
				{
					baka::state::StatePlugin* statePlug = static_cast<baka::state::StatePlugin*>(baka::IPlugin::FindPlugin(baka::state::StatePlugin::Type));
					statePlug->TransitionState(new TestbedState());
				}
			}
		}

		void SplashState::OnKeyUp(const baka::key_events::KeyAction& action)
		{

			if (action.m_code == sf::Keyboard::Space)
			{
				draw->skeleton->setBonesToSetupPose();
				draw->state->setAnimationByName(0, "animation", false);
			}

		}

		void SplashState::OnMouseMove(const baka::mouse_events::MotionAction& action)
		{
			sf::Vector2i pixel(action.m_pixel.x, action.m_pixel.y);
			baka::render::RenderPlugin* renderer = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			sf::Vector2f pos = renderer->GetRenderWindow()->mapPixelToCoords(pixel);
			draw->setPosition(pos);
		}

		
		void SplashState::AnimationListenerCallback(spine::AnimationState& state, int trackIndex, spine::EventType type, const spine::Event* event, int loopCount)
		{
			if (type == spine::EventType::Anim_End)
			{
				m_bAnimOver = true;

				
			}
		}

	}
}
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
			//atlas = new spine::Atlas();
			//spine::AttachmentLoader* loader = new spine::AtlasAttachmentLoader(*atlas);
			spine::SkeletonJson json(*atlas);
			data = json.readSkeletonDataFile((path + std::string(".json")).c_str());
			skel = new spine::Skeleton(*data);
			stateData = new spine::AnimationStateData(*data);
			draw = new spine::SkeletonDrawable(data, stateData);

			draw->state->listener = BIND5(this, &SplashState::AnimationListenerCallback);
			draw->setPosition(-256, 0);

			baka::key_events::s_InputKeyUp.Subscribe(&sub, BIND1(this, &SplashState::OnKeyUp));
		}

		VIRTUAL void SplashState::Exit()
		{
			draw->state->clearTracks();
			delete atlas;
			delete skel;
			delete stateData;
			delete draw;
			delete data;
		}

		VIRTUAL const std::string SplashState::DebugRender(sf::RenderWindow* pRenWin)
		{

			pRenWin->draw(*draw);

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
		}

		void SplashState::OnKeyUp(const baka::key_events::KeyAction& action)
		{

			if (action.m_code == sf::Keyboard::Space)
			{
				draw->skeleton->setBonesToSetupPose();
				draw->state->setAnimationByName(0, "animation", false);
			}

		}

		
		void SplashState::AnimationListenerCallback(spine::AnimationState& state, int trackIndex, spine::EventType type, const spine::Event* event, int loopCount)
		{
		}

	}
}
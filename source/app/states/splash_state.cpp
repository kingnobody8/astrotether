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
		{
			this->m_name = "SplashState";
		}

		VIRTUAL SplashState::~SplashState()
		{
		}

		VIRTUAL void SplashState::Init()
		{
			spine::Atlas* atlas = spine::Atlas::createFromFile("../spine_examples/spineboy/export/spineboy.atlas", this);
			spine::SkeletonJson json(*atlas);
			spine::SkeletonData* data = json.readSkeletonDataFile("../spine_examples/spineboy/export/spineboy.json");
			skel = new spine::Skeleton(*data);
			stateData = new spine::AnimationStateData(*data);
			draw = new spine::SkeletonDrawable(data, stateData);
			draw->state->setAnimationByName(0, "run", true);
//			draw->state->timeScale = 0.25f;
			draw->timeScale = 0.25f;
			__todo() //use the resource loader when it is finished
			//Initial loading

			m_texture.loadFromFile("assets/textures/logo_128.png");
			m_sprite.setTexture(m_texture);
			m_sprite.setScale(2.0f, 2.0f);
			//m_sprite.setPosition(m_sprite.getScale().x * m_sprite.getLocalBounds().width * -0.5f, m_sprite.getScale().y * m_sprite.getLocalBounds().height * -0.5f);

			baka::mouse_events::s_InputMouseMotion.Subscribe(&sub, BIND1(this, &SplashState::OnMove));
		}

		VIRTUAL void SplashState::Exit()
		{
		}

		VIRTUAL const std::string SplashState::DebugRender(sf::RenderWindow* pRenWin)
		{

			//m_sprite.setPosition(pRenWin->getSize().x * 0.5f - m_sprite.getGlobalBounds().width * 0.5f,
			//	pRenWin->getSize().y * 0.5f - m_sprite.getGlobalBounds().height * 0.5f);

			//pRenWin->draw(m_sprite);
			pRenWin->draw(*draw);

		//	pRenWin->draw(m_shape);

			std::string ret;
			ret += std::string("Left: ") + std::to_string(m_sprite.getGlobalBounds().left) + std::string("\n")
				+ std::string("top: ") + std::to_string(m_sprite.getGlobalBounds().top) + std::string("\n")
				+ std::string("Width: ") + std::to_string(m_sprite.getGlobalBounds().width) + std::string("\n")
				+ std::string("Height: ") + std::to_string(m_sprite.getGlobalBounds().height) + std::string("\n");

			return ret;
		}

		VIRTUAL void SplashState::Update(const sf::Time& dt)
		{
			//float temp = 10.0f;
			//m_sprite.setRotation(m_sprite.getRotation() + temp * dt.asSeconds());

			draw->update(dt.asSeconds());
		}

		void SplashState::OnMove(const baka::mouse_events::MotionAction& action)
		{
			//m_sprite.setPosition(action.m_pixel.x, action.m_pixel.y);
			//m_shape.setScale( 1.0f, m_sprite.getScale().y + 0.01f);

		}

		
	}
}
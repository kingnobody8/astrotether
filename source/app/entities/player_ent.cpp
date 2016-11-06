#pragma once
#include "player_ent.h"
#include "render/render_plugin.h"
#include "utility/helper/func.h"
#include <string>
#include <regex>
#include "state/state_plugin.h"
#include "states/testbed_state.h"

const float TOPSPEED = 10.0f;

namespace app
{
	namespace entity
	{
		PlayerEnt::PlayerEnt(b2Body* pBody)
			: m_pBody(pBody)
		{
			for (int i = 0; i < EDirection::ED_COUNT; ++i)
				m_vDirections[i] = false;
		}

		VIRTUAL PlayerEnt::~PlayerEnt()
		{
		}

		VIRTUAL void PlayerEnt::Init()
		{
			baka::key_events::s_InputKeyDown.Subscribe(this, BIND1(this, &PlayerEnt::OnKeyDown));
			baka::key_events::s_InputKeyUp.Subscribe(this, BIND1(this, &PlayerEnt::OnKeyUp));

			baka::mouse_events::s_InputMouseButtonDown.Subscribe(this, BIND1(this, &PlayerEnt::OnMouseDown));
			baka::mouse_events::s_InputMouseButtonUp.Subscribe(this, BIND1(this, &PlayerEnt::OnMouseDown));
			baka::mouse_events::s_InputMouseMotion.Subscribe(this, BIND1(this, &PlayerEnt::OnMouseMove));

		}

		VIRTUAL void PlayerEnt::Exit()
		{
		}

		VIRTUAL void PlayerEnt::Update(const sf::Time& dt)
		{
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

			if (abs(vel.x) >= TOPSPEED)
				dir.x = 0;
			if (abs(vel.y) >= TOPSPEED)
				dir.y = 0;
		
			m_pBody->ApplyForceToCenter(dir, true);
		}


		void PlayerEnt::OnKeyDown(const baka::key_events::KeyAction& action)
		{
			if (action.m_code == sf::Keyboard::Right || action.m_code == sf::Keyboard::D)
				m_vDirections[ED_RIGHT] = true;
			if (action.m_code == sf::Keyboard::Left || action.m_code == sf::Keyboard::A)
				m_vDirections[ED_LEFT] = true;
			if (action.m_code == sf::Keyboard::Up || action.m_code == sf::Keyboard::W)
				m_vDirections[ED_UP] = true;
			if (action.m_code == sf::Keyboard::Down || action.m_code == sf::Keyboard::S)
				m_vDirections[ED_DOWN] = true;
		}

		void PlayerEnt::OnKeyUp(const baka::key_events::KeyAction& action)
		{
			if (action.m_code == sf::Keyboard::Right || action.m_code == sf::Keyboard::D)
				m_vDirections[ED_RIGHT] = false;
			if (action.m_code == sf::Keyboard::Left || action.m_code == sf::Keyboard::A)
				m_vDirections[ED_LEFT] = false;
			if (action.m_code == sf::Keyboard::Up || action.m_code == sf::Keyboard::W)
				m_vDirections[ED_UP] = false;
			if (action.m_code == sf::Keyboard::Down || action.m_code == sf::Keyboard::S)
				m_vDirections[ED_DOWN] = false;
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
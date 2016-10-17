#pragma once
#include "utility/time/util_time.h"
#include "utility/math/math.h"
#include "utility/event/publisher.h"
#include "utility/event/publisher.inl"
#include <SFML/Window.hpp>

namespace baka
{
	struct IEvent
	{
		sf::Event m_event;
		IEvent(const sf::Event& event)
			: m_event(event)
		{
		}
	};

	struct app_events
	{
		static util::VoidPublisher s_KillAppEvent;
	};

	struct key_events
	{
		struct KeyAction : public IEvent
		{
			sf::Keyboard::Key	m_code;
			bool			m_repeat;

			KeyAction(const sf::Event& event, const sf::Keyboard::Key& code)
				: IEvent(event), m_code(code), m_repeat(false)
			{
			}
		};

		static util::Publisher<KeyAction> s_InputKeyDown;
		static util::Publisher<KeyAction> s_InputKeyUp;
	};

	struct mouse_events
	{
		struct MotionAction : public IEvent
		{
			vec2	m_pixel;
			//vec2	m_delta;
			__todo() // we may eventually want velocity in here delta_pixels / time_since last motion

				MotionAction(const  sf::Event& event, const vec2& pixel/*, const vec2& delta*/)
				: IEvent(event), m_pixel(pixel)/*, m_delta(delta)*/
			{
			}
		};

		struct ButtonAction : public IEvent
		{
			vec2	m_pixel;
			uchar	m_button;
			//uchar	m_clicks;

			ButtonAction(const  sf::Event& event, const vec2& pixel, const uchar& button/*, const uchar& clicks*/)
				: IEvent(event), m_pixel(pixel), m_button(button)//, m_clicks(clicks)
			{
			}
		};

		struct WheelAction : public IEvent
		{
			//vec2	m_scroll;
			int m_delta;

			WheelAction(const  sf::Event& event, const int delta)
				: IEvent(event), m_delta(delta)
			{
			}
		};

		static util::Publisher<MotionAction> s_InputMouseMotion;
		static util::Publisher<ButtonAction> s_InputMouseButtonDown;
		static util::Publisher<ButtonAction> s_InputMouseButtonUp;
		static util::Publisher<WheelAction>	s_InputMouseScrollWheel;
	};

	struct touch_events
	{
		struct MotionAction : public IEvent
		{
			vec2	m_pixel;
			vec2	m_delta;
			int64	m_fingerId;
			
			MotionAction(const  sf::Event& event, const vec2& pixel, const vec2& delta, const int64& fingerId)
				: IEvent(event), m_pixel(pixel), m_delta(delta), m_fingerId(fingerId)
			{
			}
		};

		struct TouchAction : public IEvent
		{
			vec2 m_pixel;
			int64 m_fingerId;

			TouchAction(const  sf::Event& event, const vec2& pixel, const int64& fingerId)
				: IEvent(event), m_pixel(pixel), m_fingerId(fingerId)
			{
			}
		};
		
		static util::Publisher<MotionAction> s_InputTouchMotion;
		static util::Publisher<TouchAction> s_InputTouchDown;
		static util::Publisher<TouchAction> s_InputTouchUp;
	};
}
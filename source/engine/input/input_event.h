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
			bool				m_repeat;

			KeyAction(const sf::Event& event)
				: IEvent(event), m_code(event.key.code), m_repeat(false)
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
			sf::Vector2i	m_pixel;
			//vec2	m_delta;
			__todo() // we may eventually want velocity in here delta_pixels / time_since last motion

				MotionAction(const  sf::Event& event/*, const vec2& delta*/)
				: IEvent(event), m_pixel(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))/*, m_delta(delta)*/
			{
			}
		};

		struct ButtonAction : public IEvent
		{
			sf::Vector2i	m_pixel;
			uchar	m_button;
			//uchar	m_clicks;

			ButtonAction(const  sf::Event& event/*, const uchar& clicks*/)
				: IEvent(event), m_pixel(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), m_button(event.mouseButton.button)//, m_clicks(clicks)
			{
			}
		};

		struct WheelAction : public IEvent
		{
			//vec2	m_scroll;
			int m_delta;

			WheelAction(const  sf::Event& event)
				: IEvent(event), m_delta(event.mouseWheelScroll.delta)
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

	struct joypad_events
	{
		struct ButtonAction : public IEvent
		{
			unsigned int m_code;
			unsigned int m_id;

			ButtonAction(const sf::Event& event)
				: IEvent(event), m_code(event.joystickButton.button), m_id(event.joystickButton.joystickId)
			{
			}
		};

		struct AxisAction : public IEvent
		{
			unsigned int m_axis;
			unsigned int m_id;
			float m_pos;

			AxisAction(const sf::Event& event)
			: IEvent(event), m_axis(event.joystickMove.axis), m_id(event.joystickMove.joystickId), m_pos(event.joystickMove.position)
			{
			}
		};

		static util::Publisher<ButtonAction> s_InputJoypadButtonDown;
		static util::Publisher<ButtonAction> s_InputJoypadButtonUp;
		static util::Publisher<AxisAction> s_InputJoypadMove;
	};
}
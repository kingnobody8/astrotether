#pragma once
#include "utility/time/util_time.h"
#include "utility/math/math.h"
#include "utility/event/publisher.h"
#include "utility/event/publisher.inl"
#include "SDL.h"

namespace engine
{
	struct IEvent
	{
		SDL_Event m_event;
		IEvent(const SDL_Event& event)
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
			SDL_Keycode		m_code;
			bool			m_repeat;

			KeyAction(const SDL_Event& event, const SDL_Keycode& code, const bool repeat)
				: IEvent(event), m_code(code), m_repeat(repeat)
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
			vec2	m_delta;
			__todo() // we may eventually want velocity in here delta_pixels / time_since last motion

				MotionAction(const SDL_Event& event, const vec2& pixel, const vec2& delta)
				: IEvent(event), m_pixel(pixel), m_delta(delta)
			{
			}
		};

		struct ButtonAction : public IEvent
		{
			vec2	m_pixel;
			uchar	m_button; //this is SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, or SDL_BUTTON_RIGHT
			uchar	m_clicks;

			ButtonAction(const SDL_Event& event, const vec2& pixel, const uchar& button, const uchar& clicks)
				: IEvent(event), m_pixel(pixel), m_button(button), m_clicks(clicks)
			{
			}
		};

		struct WheelAction : public IEvent
		{
			vec2	m_scroll;

			WheelAction(const SDL_Event& event, const vec2 scroll)
				: IEvent(event), m_scroll(scroll)
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
			
			MotionAction(const SDL_Event& event, const vec2& pixel, const vec2& delta, const int64& fingerId)
				: IEvent(event), m_pixel(pixel), m_delta(delta), m_fingerId(fingerId)
			{
			}
		};

		struct TouchAction : public IEvent
		{
			vec2 m_pixel;
			int64 m_fingerId;

			TouchAction(const SDL_Event& event, const vec2& pixel, const int64& fingerId)
				: IEvent(event), m_pixel(pixel), m_fingerId(fingerId)
			{
			}
		};
		
		static util::Publisher<MotionAction> s_InputTouchMotion;
		static util::Publisher<TouchAction> s_InputTouchDown;
		static util::Publisher<TouchAction> s_InputTouchUp;
	};
}
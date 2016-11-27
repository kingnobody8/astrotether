#include "input_plugin.h"
#include "input_event.h"

namespace baka
{
	DEFINE_PLUGIN_TYPE_INFO(InputPlugin);

	InputPlugin::InputPlugin()
		: m_pRenWin(null)
	{
		m_bDebugDraw = true;
	}

	VIRTUAL InputPlugin::~InputPlugin()
	{
	}

	VIRTUAL void InputPlugin::Init()
	{
		assert(m_pRenWin);
	}

	VIRTUAL void InputPlugin::Exit()
	{
	}

	VIRTUAL bool InputPlugin::Update(const sf::Time& dt)
	{
		sf::Event tEvent;
		while (m_pRenWin->pollEvent(tEvent))
		{
			switch (tEvent.type)
			{
			case sf::Event::Closed:
				app_events::s_KillAppEvent.Publish(); //NOTE if we get a quit event, then we want to exit ASAP!
				return false; 
			case sf::Event::Resized: break;
			case sf::Event::LostFocus: break;
			case sf::Event::GainedFocus: break;
			case sf::Event::TextEntered: break;
			case sf::Event::KeyPressed:
				key_events::s_InputKeyDown.Publish(key_events::KeyAction(tEvent)); break;
			case sf::Event::KeyReleased: 
				key_events::s_InputKeyUp.Publish(key_events::KeyAction(tEvent)); break;
			case sf::Event::MouseWheelScrolled:
				mouse_events::s_InputMouseScrollWheel.Publish(mouse_events::WheelAction(tEvent)); break;
			case sf::Event::MouseButtonPressed:
				mouse_events::s_InputMouseButtonDown.Publish(mouse_events::ButtonAction(tEvent)); break;
			case sf::Event::MouseButtonReleased:
				mouse_events::s_InputMouseButtonUp.Publish(mouse_events::ButtonAction(tEvent)); break;
			case sf::Event::MouseMoved:
				mouse_events::s_InputMouseMotion.Publish(mouse_events::MotionAction(tEvent)); break;
			case sf::Event::MouseEntered: break;
			case sf::Event::MouseLeft: break;
			case sf::Event::JoystickButtonPressed:
				joypad_events::s_InputJoypadButtonDown.Publish(joypad_events::ButtonAction(tEvent)); break;
			case sf::Event::JoystickButtonReleased:
				joypad_events::s_InputJoypadButtonUp.Publish(joypad_events::ButtonAction(tEvent)); break;
			case sf::Event::JoystickMoved:
				joypad_events::s_InputJoypadMove.Publish(joypad_events::AxisAction(tEvent)); break;
			case sf::Event::JoystickConnected: break;
			case sf::Event::JoystickDisconnected: break;
			case sf::Event::TouchBegan: break;
			case sf::Event::TouchMoved: break;
			case sf::Event::TouchEnded: break;
			case sf::Event::SensorChanged: break;

			default:
				break;
			}
		}
		return true;
	}

	VIRTUAL const std::string InputPlugin::DebugRender(sf::RenderWindow* pRenWin)
	{
		std::string ret = "----Input----\n";
		ret += std::string("MousePos: ") + std::to_string(sf::Mouse::getPosition(*pRenWin).x) + std::string("x") + std::to_string(sf::Mouse::getPosition(*pRenWin).y);

		return ret;
	}

/*
	const vec2 InputPlugin::ConvertPixelToCartesian(int x, int y)
	{
		return vec2(x, y);
		glm::i32vec2 logical_size;
		SDL_GetWindowSize(m_pSdlWin, &logical_size.x, &logical_size.y);

		vec2 ret;
		ret.x = x;
		ret.y = logical_size.y - y;
		return ret;
	}
	
	const vec2 InputPlugin::ConvertFloatPixelToCartesian(float x, float y)
	{
		glm::i32vec2 logical_size;
		SDL_GetWindowSize(m_pSdlWin, &logical_size.x, &logical_size.y);
		
		return ConvertPixelToCartesian(x * logical_size.x, y * logical_size.y);
	}*/
}

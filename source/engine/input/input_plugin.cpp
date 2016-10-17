#include "input_plugin.h"
#include "input_event.h"

namespace baka
{
	DEFINE_PLUGIN_TYPE_INFO(InputPlugin);

	InputPlugin::InputPlugin()
		: m_pRenWin(null)
	{
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

	VIRTUAL bool InputPlugin::Update(const util::Time& dt)
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
				key_events::s_InputKeyDown.Publish(key_events::KeyAction(tEvent, tEvent.key.code)); break;
			case sf::Event::KeyReleased: 
				key_events::s_InputKeyUp.Publish(key_events::KeyAction(tEvent, tEvent.key.code)); break;
			case sf::Event::MouseWheelMoved: break;
			case sf::Event::MouseWheelScrolled: break;
			case sf::Event::MouseButtonPressed: break;
			case sf::Event::MouseButtonReleased: break;
			case sf::Event::MouseMoved: break;
			case sf::Event::MouseEntered: break;
			case sf::Event::MouseLeft: break;
			case sf::Event::JoystickButtonPressed: break;
			case sf::Event::JoystickButtonReleased: break;
			case sf::Event::JoystickMoved: break;
			case sf::Event::JoystickConnected: break;
			case sf::Event::JoystickDisconnected: break;
			case sf::Event::TouchBegan: break;
			case sf::Event::TouchMoved: break;
			case sf::Event::TouchEnded: break;
			case sf::Event::SensorChanged: break;



			//	//QUIT event
			//case SDL_QUIT:
			//	app_events::s_KillAppEvent.Publish(); //NOTE if we get a quit event, then we want to exit ASAP!
			//	return false;
			//	//MOBILE event
			//case SDL_APP_TERMINATING:
			//case SDL_APP_LOWMEMORY:
			//case SDL_APP_WILLENTERBACKGROUND:
			//case SDL_APP_DIDENTERBACKGROUND:
			//case SDL_APP_WILLENTERFOREGROUND:
			//case SDL_APP_DIDENTERFOREGROUND:
			//	break;
			//	//SYSTEM event
			//case SDL_WINDOWEVENT:
			//	break;
			//case SDL_SYSWMEVENT:	/*Disabled by default, system specific events, to enable call SDL_EventState*/
			//	break;
			//	//KEYBOARD event
			//case SDL_KEYDOWN:
			//	key_events::s_InputKeyDown.Publish(key_events::KeyAction(tEvent, tEvent.key.keysym.sym, tEvent.key.state != 0));
			//	break;
			//case SDL_KEYUP:
			//	key_events::s_InputKeyUp.Publish(key_events::KeyAction(tEvent, tEvent.key.keysym.sym, tEvent.key.state != 0));
			//	break;
			//	//TEXT event
			//case SDL_TEXTEDITING:
			//case SDL_TEXTINPUT:
			//	//MOUSE event
			//	break;
			//case SDL_MOUSEMOTION:
			//{
			//	mouse_events::MotionAction action(tEvent, ConvertPixelToCartesian(tEvent.motion.x, tEvent.motion.y), vec2(tEvent.motion.xrel, -tEvent.motion.yrel));
			//	script::ui::CUiPart::OnMouseMotion(action);
			//	mouse_events::s_InputMouseMotion.Publish(action);
			//	break;
			//}
			//case SDL_MOUSEBUTTONDOWN:
			//{
			//	mouse_events::ButtonAction action(tEvent, ConvertPixelToCartesian(tEvent.motion.x, tEvent.motion.y), tEvent.button.button, tEvent.button.clicks);
			//	script::ui::CUiPart::OnMouseButtonDown(action);
			//	mouse_events::s_InputMouseButtonDown.Publish(action);
			//	break;
			//}
			//case SDL_MOUSEBUTTONUP:
			//{
			//	mouse_events::ButtonAction action(tEvent, ConvertPixelToCartesian(tEvent.motion.x, tEvent.motion.y), tEvent.button.button, tEvent.button.clicks);
			//	script::ui::CUiPart::OnMouseButtonUp(action);
			//	mouse_events::s_InputMouseButtonUp.Publish(action);
			//	break;
			//}
			//case SDL_MOUSEWHEEL:
			//	mouse_events::s_InputMouseScrollWheel.Publish(mouse_events::WheelAction(tEvent, vec2(tEvent.wheel.x, tEvent.wheel.y)));
			//	break;
			//	//JOY event
			//case SDL_JOYAXISMOTION:
			//case SDL_JOYBALLMOTION:
			//case SDL_JOYHATMOTION:
			//case SDL_JOYBUTTONDOWN:
			//case SDL_JOYBUTTONUP:
			//case SDL_JOYDEVICEADDED:
			//case SDL_JOYDEVICEREMOVED:
			//	break;
			//	//CONTROLLER event
			//case SDL_CONTROLLERAXISMOTION:
			//case SDL_CONTROLLERBUTTONDOWN:
			//case SDL_CONTROLLERBUTTONUP:
			//case SDL_CONTROLLERDEVICEADDED:
			//case SDL_CONTROLLERDEVICEREMOVED:
			//case SDL_CONTROLLERDEVICEREMAPPED:
			//	break;
			//	//TOUCH event
			//case SDL_FINGERDOWN:
			//{
			//	touch_events::TouchAction action(tEvent, ConvertFloatPixelToCartesian(tEvent.tfinger.x, tEvent.tfinger.y), tEvent.tfinger.fingerId);
			//	//script::ui::CuiPart::OnFingerDown(action);
			//	touch_events::s_InputTouchDown.Publish(action);
			//	break;
			//}
			//case SDL_FINGERUP:
			//{
			//	touch_events::TouchAction action(tEvent, ConvertFloatPixelToCartesian(tEvent.tfinger.x, tEvent.tfinger.y), tEvent.tfinger.fingerId);
			//	//script::ui::CuiPart::OnFingerDown(action);
			//	touch_events::s_InputTouchUp.Publish(action);
			//	break;
			//}
			//case SDL_FINGERMOTION:
			//{
			//	touch_events::MotionAction action(tEvent, ConvertFloatPixelToCartesian(tEvent.tfinger.x, tEvent.tfinger.y), vec2(tEvent.tfinger.dx, -tEvent.tfinger.dy), tEvent.tfinger.fingerId);
			//	//script::ui::CuiPart::OnFingerDown(action);
			//	touch_events::s_InputTouchMotion.Publish(action);
			//	break;
			//}
			//	//GESTURE event
			//case SDL_DOLLARGESTURE:
			//case SDL_DOLLARRECORD:
			//case SDL_MULTIGESTURE:
			//	break;
			//	//CLIPBOARD event
			//case SDL_CLIPBOARDUPDATE:
			//	break;
			//	//DROP event
			//case SDL_DROPFILE:
			//	break;
			//	//USER event
			//case SDL_USEREVENT:
			//case SDL_LASTEVENT: /*Don't use*/
			//	break;
			default:
				break;
			}
		}
		return true;
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

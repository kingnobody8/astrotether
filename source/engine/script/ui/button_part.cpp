#include "button_part.h"
#include "component/object.h"
#include "../script.h"

namespace engine
{
	namespace script
	{
		namespace ui
		{
			DEFINE_PART_TYPE_INFO(CButtonPart);

			CButtonPart::CButtonPart()
				: m_bPressed(false)
				, m_pData(nullptr)
			{
				m_priority = BUTTON_PART;
			}

			CButtonPart::~CButtonPart()
			{
				m_publisher.UnsubscribeAll();
			}

			VIRTUAL void CButtonPart::Init()
			{
				CUiPart::Init();
			}

			VIRTUAL void CButtonPart::Exit()
			{
				CUiPart::Exit();
			}

			VIRTUAL void CButtonPart::LoadJson(const util::JSON& json)
			{
				CUiPart::LoadJson(json);
			}

			void CButtonPart::Subscribe(util::Subscriber* subscriber, util::Publisher<CButtonPart*>::Callback callback, const int& priority)
			{
				m_publisher.Subscribe(subscriber, callback, priority);
			}

			void CButtonPart::Unsubscribe(util::Subscriber* subscriber)
			{
				m_publisher.Unsubscribe(subscriber);
			}
			
			void CButtonPart::UnsubscribeAll()
			{
				m_publisher.UnsubscribeAll();
			}

			VIRTUAL bool CButtonPart::OnMouseButtonDownInternal(const mouse_events::ButtonAction& action, const vec2& wpos)
			{
				const bool intersection = CUiPart::OnMouseButtonDownInternal(action, wpos);

				if (m_bPressed) //if we are pressed, then return that we handled it (may happen when screen loses focus while holding)
					return true;
				else if (!intersection) //else no intersection, so return unhandled
					return false;

				m_bPressed = true;
				return true;
			}
		
			VIRTUAL bool CButtonPart::OnMouseButtonUpInternal(const mouse_events::ButtonAction& action, const vec2& wpos)
			{
				if (!m_bPressed)
					return false;

				const bool intersection = CUiPart::OnMouseButtonUpInternal(action, wpos);

				if (m_bPressed)
				{
					m_bPressed = false;
					if (intersection)
					{
						__todo() //click callback here
						m_publisher.Publish(this);
					}
					return true;
				}
				return false;
			}
			
			VIRTUAL bool CButtonPart::OnMouseMotionInternal(const mouse_events::MotionAction& action, const vec2& wpos)
			{
				if (!m_bPressed)
					return false;

				const bool intersection = CUiPart::OnMouseMotionInternal(action, wpos);
					
				if (m_bPressed)
				{
					if (!intersection)
					{
						m_bPressed = false;
					}
					//else do nothing
					return true;
				}
				return false;
			}
		}
	}
}
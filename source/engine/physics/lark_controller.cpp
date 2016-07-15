#include "lark_controller.h"
#include "physics/physics_plugin.h"
#include "callbacks/raycast_callback.h"

#define ROPE_TIME 250

namespace engine
{
	LarkController::LarkController()
		: m_pLarkBody(NULL)
		, m_bIsPushActive(false)
		, m_pWorld(NULL)
		, m_pRopeJoint(NULL)
		, m_bIsTestbed(false)
	{
	}

	LarkController::~LarkController()
	{
	}

	void LarkController::Init(b2World* pWorld, b2Body* pLarkBody)
	{
		m_pWorld = pWorld;
		m_pLarkBody = pLarkBody;

#ifdef IS_MOBILE
		touch_events::s_InputTouchDown.Subscribe(&m_sub, BIND1(this, &LarkController::OnTouchDown));
		touch_events::s_InputTouchUp.Subscribe(&m_sub, BIND1(this, &LarkController::OnTouchUp));
		touch_events::s_InputTouchMotion.Subscribe(&m_sub, BIND1(this, &LarkController::OnTouchMotion));
#elif IS_PC
		mouse_events::s_InputMouseButtonDown.Subscribe(&m_sub, BIND1(this, &LarkController::OnMouseDown));
		mouse_events::s_InputMouseButtonUp.Subscribe(&m_sub, BIND1(this, &LarkController::OnMouseUp));
		mouse_events::s_InputMouseMotion.Subscribe(&m_sub, BIND1(this, &LarkController::OnMouseMotion));
#endif
	}

	void LarkController::Update(const util::Time& dt)
	{
		vec2 screenPos;

#ifdef IS_MOBILE
		for(int i = 0; i < EFingerDefs::eFingerCount; ++i)
		{
			m_vFingerInfo[i].m_time += dt;
		}
		
		//average the all push finger positions
		vec2 averageScreenPosition;
		int count = 0;
		for (int i = 0; i < EFingerDefs::eFingerCount; ++i)
		{
			if (m_vFingerInfo[i].m_type != EFingerTypes::ePush)
			{
				if(m_vFingerInfo[i].m_type == EFingerTypes::eUnknown)
					PushCheck(i);
				continue;
			}
			averageScreenPosition += m_vFingerInfo[i].m_position;
			++count;
		}

		if(count == 0)
			return;

		averageScreenPosition /= (float)(count);
		screenPos = averageScreenPosition;
#elif IS_PC
		if (!m_bIsPushActive)
			return;
		screenPos = m_vMouseInfo[eLeftButton].m_position;
#endif
		b2Vec2 pw = physics::g_camera.ConvertScreenToWorld(b2Vec2(screenPos.x, screenPos.y));
		b2Vec2 diff = m_pLarkBody->GetPosition() - pw;
		diff.Normalize();
		diff *= 1000.0f;
		m_pLarkBody->ApplyForceToCenter(diff, true);
	}

	void LarkController::Exit()
	{
	}

#ifdef IS_MOBILE
	void LarkController::OnTouchDown(const touch_events::TouchAction& action)
	{
		if (m_bIsTestbed)
			return;

		int index = FindTouchSlot(action.m_fingerId);
		if (index == -1)
			return;

		//set values
		FingerInfo& info = m_vFingerInfo[index];
		info.m_bDown = true;
		info.m_time = 0;
		info.m_position = action.m_pixel;
		info.m_type = EFingerTypes::eUnknown;
		info.m_fingerId = action.m_fingerId;
	}

	void LarkController::OnTouchUp(const touch_events::TouchAction& action)
	{
		if (m_bIsTestbed)
			return;

		int index = FindTouchSlot(action.m_fingerId);
		if (index == -1)
			return;

		//set values (pt1)
		FingerInfo& info = m_vFingerInfo[index];
		info.m_position = action.m_pixel;
		info.m_bDown = false;

		switch (info.m_type)
		{
		case EFingerTypes::eRope:
		case EFingerTypes::eUnknown:	//Unkown and rope could both activate a rope if the timing is right
		{
			if (info.m_time.Milli() < ROPE_TIME)
			{
				OnRopeEvent(info.m_position);
			}
			break;
		}
		case EFingerTypes::ePush:
		{
			//DeactivatePush();
			break;
		}
		}

		//set values (pt2)
		info.m_time = 0;
		info.m_type = EFingerTypes::eInvalidType;
		info.m_fingerId = 0;
	}

	void LarkController::OnTouchMotion(const touch_events::MotionAction& action)
	{
		if (m_bIsTestbed)
			return;

		int index = FindTouchSlot(action.m_fingerId);
		if (index == -1)
			return;

		//set values
		FingerInfo& info = m_vFingerInfo[index];
		info.m_position = action.m_pixel;
	}

	int LarkController::FindTouchSlot(const int64& fingerId)
	{ 
		int firstEmpty = -1;
		for (int i = 0; i < EFingerDefs::eFingerCount; ++i)
		{
			if (firstEmpty == -1 && m_vFingerInfo[i].m_type == EFingerTypes::eInvalidType)
			{
				firstEmpty = i;
			}

			if (m_vFingerInfo[i].m_fingerId == fingerId)
			{
				return i;
			}
		}

		return firstEmpty;
	}

	void LarkController::PushCheck(const int& index)
	{
		FingerInfo& info = m_vFingerInfo[index];
		assert(info.m_bDown && info.m_type == EFingerTypes::eUnknown);

		////find another finger we can use
		//int otherFingerId = -1;
		//for (int i = 0; i < EFingerDefs::eFingerCount; ++i)
		//{
		//	if (i == index)
		//	{
		//		continue;
		//	}
		//	if (m_vFingerInfo[i].m_bDown && m_vFingerInfo[i].m_type == EFingerTypes::eUnknown)
		//	{
		//		otherFingerId = i;
		//		break;
		//	}
		//}

		//if (otherFingerId != -1)
		//{
		//	FingerInfo& otherInfo = m_vFingerInfo[otherFingerId];

		if(info.m_time.Milli() > ROPE_TIME)
			info.m_type = EFingerTypes::ePush;
			//otherInfo.m_type = EFingerTypes::ePush;

		//	ActivatePush();
	//	}
	}

#elif IS_PC
	void LarkController::OnMouseDown(const mouse_events::ButtonAction& action)
	{
		if (m_bIsTestbed)
			return;

		if (action.m_button == SDL_BUTTON_LEFT)
		{
			m_vMouseInfo[eLeftButton].m_bDown = true;
			m_vMouseInfo[eLeftButton].m_position = action.m_pixel;
			ActivatePush();
		}
		else if (action.m_button == SDL_BUTTON_RIGHT)
		{
			m_vMouseInfo[eRightButton].m_bDown = true;
			m_vMouseInfo[eRightButton].m_position = action.m_pixel;
		}
}

	void LarkController::OnMouseUp(const mouse_events::ButtonAction& action)
	{
		if (m_bIsTestbed)
			return;

		if (action.m_button == SDL_BUTTON_LEFT)
		{
			m_vMouseInfo[eLeftButton].m_bDown = false;
			m_vMouseInfo[eLeftButton].m_position = action.m_pixel;
			DeactivatePush();
		}
		else if (action.m_button == SDL_BUTTON_RIGHT)
		{
			m_vMouseInfo[eRightButton].m_bDown = false;
			m_vMouseInfo[eRightButton].m_position = action.m_pixel;
			OnRopeEvent(action.m_pixel);
		}
	}

	void LarkController::OnMouseMotion(const mouse_events::MotionAction& action)
	{
		if (m_bIsTestbed)
			return;

		for (int i = 0; i < EMouseDefs::eButtonCount; ++i)
		{
			m_vMouseInfo[i].m_position = action.m_pixel;
		}
	}
#endif


	void LarkController::ActivatePush()
	{
		m_bIsPushActive = true;
	}

	void LarkController::DeactivatePush()
	{
		m_bIsPushActive = false;
#ifdef IS_MOBILE
		for(int i = 0; i < EFingerDefs::eFingerCount; ++i)
		{
			if(m_vFingerInfo[i].m_type == EFingerTypes::ePush)
			{
				m_vFingerInfo[i] = FingerInfo();
			}
		}
#endif
	}

	void LarkController::OnRopeEvent(const vec2& screenPos)
	{
		if (m_pRopeJoint == NULL)
		{
			b2Vec2 pw = physics::g_camera.ConvertScreenToWorld(b2Vec2(screenPos.x, screenPos.y));
			physics::callbacks::RayCastClosestCallback callback;
			b2Vec2 direction = pw - m_pLarkBody->GetPosition();
			direction.Normalize();
			direction *= 100;
			b2Vec2 point = m_pLarkBody->GetPosition() + direction;
			m_pWorld->RayCast(&callback, m_pLarkBody->GetPosition(), point);

			if (callback.m_bHit)
			{
				b2Body* body = callback.m_pFixture->GetBody();
				b2RopeJointDef md;
				md.bodyA = m_pLarkBody;
				md.bodyB = body;
				md.localAnchorA.SetZero();
				md.localAnchorB = body->GetLocalPoint(callback.m_point);
				md.maxLength = (callback.m_point - m_pLarkBody->GetPosition()).Length() * 1.1f;
				md.collideConnected = true;
				m_pRopeJoint = (b2RopeJoint*)m_pWorld->CreateJoint(&md);
				body->SetAwake(true);
			}
		}
		else
		{
			m_pWorld->DestroyJoint(m_pRopeJoint);
			m_pRopeJoint = NULL;
		}
	}
}
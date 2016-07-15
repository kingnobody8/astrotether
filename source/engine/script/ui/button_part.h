#pragma once
#include "ui_part.h"

namespace engine
{
	namespace script
	{
		namespace ui
		{

			__todo() //note that this button class assumes that it is not being transformed while the user is holding the mouse down, we may want to put in code to handle a moving button
			class CButtonPart : public CUiPart
			{
			public:
				DECLARE_PART_TYPE_INFO(CButtonPart);

				/*Data*/
			private:
			protected:
				util::Publisher<CButtonPart*> m_publisher; __todo() //should probably break this up into mulitple publishers for the different states (up, down, single, double, triple ect)
				bool m_bPressed; __todo() //i thought about making this public so we could manually set if a ui instance would absorbe input, but i think that it would be better to hide that from the user and just make sure it works in all situations (i am mainly thinking about btn in scroll list)
				void* m_pData;

				/*Func*/
			protected:
				virtual bool OnMouseButtonDownInternal(const mouse_events::ButtonAction& action, const vec2& wpos);
				virtual bool OnMouseButtonUpInternal(const mouse_events::ButtonAction& action, const vec2& wpos);
				virtual bool OnMouseMotionInternal(const mouse_events::MotionAction& action, const vec2& wpos);

			public:
				CButtonPart();
				virtual ~CButtonPart();
				CButtonPart& operator= (const CButtonPart& that);

				virtual void Init();
				virtual void Exit();
				virtual void LoadJson(const util::JSON& json);

				void Subscribe(util::Subscriber* subscriber, util::Publisher<CButtonPart*>::Callback callback, const int& priority = -1);
				void Unsubscribe(util::Subscriber* subscriber);
				void UnsubscribeAll();

				inline void SetData(const void* const data) { m_pData = m_pData; }
				inline void* GetData() const { return m_pData; }
			};
		}
	}
}
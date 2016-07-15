#pragma once
#include "component/part.h"
#include "render/render_layer.h"
#include "input/input_event.h"

namespace engine
{
	namespace script
	{
		namespace ui
		{
			class CUiPart : public component::IPart
			{
			public:
				typedef std::list<CUiPart*> UiPartList;
				typedef std::list<CUiPart*>::iterator UiPartIter;

				//Internal
			private:
				struct UiLayer
				{
					render::CRenderLayer* m_pLayer = null;
					UiPartList m_uiParts;

					bool OnMouseButtonDown(const mouse_events::ButtonAction& action);
					bool OnMouseButtonUp(const mouse_events::ButtonAction& action);
					bool OnMouseMotion(const mouse_events::MotionAction& action);
				};
				static std::list<UiLayer> s_uiLayers;
				static UiPartList s_pendingUiParts;
				static void CleanUiParts(void);
				void DisableUiImmediate(void);


				//External
			public:
				static const UiPartList GetUiParts();
				static const UiPartList GetUiPartsOnLayer(const std::string& szLayer);
				static void OnMouseButtonDown(const mouse_events::ButtonAction& action);
				static void OnMouseButtonUp(const mouse_events::ButtonAction& action);
				static void OnMouseMotion(const mouse_events::MotionAction& action);

			private:
				bool				m_bUiEnabled;
				bool				m_bPendingUi;
				u16					m_usUiIndex;

				//------------------------------------------------------------------------------

			public:
				DECLARE_PART_TYPE_INFO(CUiPart);

				/*Data*/
			private:
			protected:
				b2AABB m_aabb;
				std::string m_szLayer;
				float m_zed;

				/*Func*/
			protected:
				void CalculateIntersectionRect();

				/*the mouse action internals return a bool to respond if they have handled the event or not (true means handled, false means unhandled) */
				__todo() //maybe make the return an enum with codes for handled/unhandled

				virtual bool OnMouseButtonDownInternal(const mouse_events::ButtonAction& action, const vec2& wpos) { return m_aabb.Contains(b2Vec2(wpos.x, wpos.y)); }
				virtual bool OnMouseButtonUpInternal(const mouse_events::ButtonAction& action, const vec2& wpos) { return m_aabb.Contains(b2Vec2(wpos.x, wpos.y)); }
				virtual bool OnMouseMotionInternal(const mouse_events::MotionAction& action, const vec2& wpos) { return m_aabb.Contains(b2Vec2(wpos.x, wpos.y)); }

			public:
				CUiPart();
				virtual ~CUiPart();
				CUiPart& operator= (const CUiPart& that);

				virtual void Init();
				virtual void Exit();
				virtual void LoadJson(const util::JSON& json);

				virtual void OnMatrixChanged(void);
				virtual void OnChildMatrixChanged(component::CObject* child);
				virtual void OnChildAppended(component::CObject* child);
				virtual void OnChildRemoved(component::CObject* child);

				void Register();
				void Unregister();

				/*void SetLayer();
				void SetZedToTop;
				void SetZedToBottom;*/

				inline const std::string& GetLayer() const { return m_szLayer; }
				inline const float& GetZed() const { return m_zed; }
			};
		}
	}
}
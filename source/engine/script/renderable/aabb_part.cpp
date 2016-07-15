#include "aabb_part.h"
#include "../script.h"
#include "render/node/rect_node.h"
#include "component/object.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			DEFINE_PART_TYPE_INFO(CAabbPart);

			CAabbPart::CAabbPart()
			{
				m_priority = EPriorities::AABB_PART;
				m_pNode = new render::CRenderNodeRect();
			}
			VIRTUAL CAabbPart::~CAabbPart()
			{
				delete m_pNode;
			}
			CAabbPart& CAabbPart::operator = (const CAabbPart& that)
			{
				IRenderPart::operator=(that);
				m_extends = that.m_extends;
				return *this;
			}

			VIRTUAL void CAabbPart::Init()
			{
				IRenderPart::Init();

				m_force = true;
				SetExtends(m_extends);
				m_force = false;
			}

			VIRTUAL void CAabbPart::LoadJson(const util::JSON& json)
			{
				IRenderPart::LoadJson(json);

				assert(json.HasMember("extends"));

				m_extends = ((const util::JSON&)(json["extends"])).GetVec2();
			}

			VIRTUAL void CAabbPart::OnMatrixChanged(void)
			{
				m_force = true;
				SetExtends(m_extends);
				m_force = false;
			}

			VIRTUAL void CAabbPart::OnZedChanged(void)
			{
				const float wzed = m_pOwner->CalcWorldZed();
				m_pNode->SetZed(wzed);
			}

			void CAabbPart::SetExtends(const util::math::vec2& extends)
			{
				if (!m_force && (m_extends != extends))
					return;

				m_extends = extends;

				//note - this is an aabb so rotation does not factor in
				const util::math::Matrix2D wmat = m_pOwner->CalcWorldMatrix();
				const util::math::vec2 wpos = wmat.GetPosition();
				const util::math::vec2 wscale = wmat.GetScale();
				const util::math::vec2 min = wpos - extends * wscale;
				const util::math::vec2 max = wpos + extends * wscale;
				b2AABB aabb(b2Vec2(min.x, min.y), b2Vec2(max.x, max.y));

				render::CRenderNodeRect* rect = static_cast<render::CRenderNodeRect*>(m_pNode);
				rect->SetAABB(aabb);
			}

		}
	}
}
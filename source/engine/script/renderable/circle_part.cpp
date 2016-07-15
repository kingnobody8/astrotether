#include "circle_part.h"
#include "../script.h"
#include "render/node/circle_node.h"
#include "component/object.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			DEFINE_PART_TYPE_INFO(CCirclePart);

			CCirclePart::CCirclePart()
				: m_radius(0)
				, m_fill(false)
			{
				m_priority = EPriorities::CIRCLE_PART;
				m_pNode = new render::CRenderNodeCircle();
			}
			VIRTUAL CCirclePart::~CCirclePart()
			{
				delete m_pNode;
			}
			CCirclePart& CCirclePart::operator = (const CCirclePart& that)
			{
				IRenderPart::operator=(that);
				m_center = that.m_center;
				m_radius = that.m_radius;
				return *this;
			}

			VIRTUAL void CCirclePart::Init()
			{
				IRenderPart::Init();

				m_force = true;
				SetCenter(m_center);
				SetRadius(m_radius);
				SetFill(m_fill);
				m_force = false;
			}

			VIRTUAL void CCirclePart::LoadJson(const util::JSON& json)
			{
				IRenderPart::LoadJson(json);

				assert(json.HasMember("center"));
				assert(json.HasMember("radius"));
				assert(json.HasMember("fill"));

				m_center = ((const util::JSON&)(json["center"])).GetVec2();
				m_radius = json["radius"].GetDouble();
				m_fill = json["fill"].GetBool();
			}

			VIRTUAL void CCirclePart::OnMatrixChanged(void)
			{
				m_force = true;
				SetCenter(m_center);
				SetRadius(m_radius);
				m_force = false;
			}

			VIRTUAL void CCirclePart::OnZedChanged(void)
			{
				const float wzed = m_pOwner->CalcWorldZed();
				m_pNode->SetZed(wzed);
			}

			void CCirclePart::SetCenter(const util::math::vec2& center)
			{
				if (!m_force && (m_center != center))
					return;

				m_center = center;

				util::math::Matrix2D wmat = m_pOwner->CalcWorldMatrix();
				const util::math::vec2 tmp = util::math::Matrix2D::Vector_Matrix_Multiply(m_center, wmat);

				render::CRenderNodeCircle* circle = static_cast<render::CRenderNodeCircle*>(m_pNode);
				circle->SetCenter(tmp);
			}

			void CCirclePart::SetRadius(const float& radius)
			{
				if (!m_force && (m_radius != radius))
					return;
				__todo()//what i think we should do about scaling and rendering the circle surface is this:
					/*
						whenever we set the radius or center of the node the node recreates the surface, but then caches it for use over and over (it basically creates and stores a texture)
						for scaling we give the circle node our matrix which it uses when rendering the texture of the circle, this way we can scale it
						this means we should set the radius and center directly as is (without matrix adjustments)
					*/

				m_radius = radius;
				render::CRenderNodeCircle* circle = static_cast<render::CRenderNodeCircle*>(m_pNode);
				circle->SetRadius(radius);
			}

			void CCirclePart::SetFill(const bool fill)
			{
				if (!m_force && (m_fill != fill))
					return;

				m_fill = fill;
				render::CRenderNodeCircle* circle = static_cast<render::CRenderNodeCircle*>(m_pNode);
				circle->SetFill(fill);
			}
		}
	}
}
#include "render_part.h"
#include "render/node/render_node.h"
#include "component/object.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			IRenderPart::IRenderPart(void)
				: m_pNode(null)
				, m_force(false)
			{
			}
			IRenderPart& IRenderPart::operator = (const IRenderPart& that)
			{
				m_szLayer = that.m_szLayer;
				m_color = that.m_color;
				if (that.m_pNode != null)
					*m_pNode = *that.m_pNode;
				m_force = that.m_force;
				return *this;
			}
			VIRTUAL	IRenderPart::~IRenderPart(void)
			{
			}

			VIRTUAL void IRenderPart::Init()
			{
				m_force = true;
				SetLayer(m_szLayer);
				SetLocalColor(m_color);
				OnMatrixChanged();
				OnZedChanged();
				m_force = false;
			}

			VIRTUAL void IRenderPart::LoadJson(const util::JSON& json)
			{
				assert(json.HasMember("layer"));
				assert(json.HasMember("color"));

				m_szLayer = json["layer"].GetString();
				m_color = ((const util::JSON&)(json["color"])).GetColor();
			}

			VIRTUAL void IRenderPart::OnZedChanged(void)
			{
				const float wzed = m_pOwner->CalcWorldZed();
				m_pNode->SetZed(wzed);
			}

			VIRTUAL void IRenderPart::OnVisibilityChanged(const bool visible)
			{
				if (!m_force && (visible == (m_pNode->GetLayer() != null)))
					return;

				if (visible)
					m_pNode->Register(m_szLayer);
				else
					m_pNode->Unregister();
			}

			VIRTUAL const b2AABB IRenderPart::CalcAABB(void)
			{
				return m_pNode->CalcAABB();
			}

			VIRTUAL const bool IRenderPart::IsRegistered() const
			{
				return (m_pNode->GetLayer() != null);
			}
			const util::Color IRenderPart::GetWorldColor() const
			{
				util::Color clr;
				m_pNode->GetColor();
				return clr;
			}

			VIRTUAL void IRenderPart::SetLocalColor(const util::Color& clr)
			{
				if (!m_force && (m_color == clr))
					return;

				__todo() //this needs to do a vertical search for ColorModPart and see what colors it applies to us so we can properly set the nod
				m_color = clr;
				m_pNode->SetColor(clr);
			}
			VIRTUAL void IRenderPart::SetLayer(const std::string& szLayer)
			{
				if (!m_force && (m_szLayer == szLayer))
					return;

				m_szLayer = szLayer;
				if (m_pOwner->GetWorldVisible())
					m_pNode->Register(szLayer);
			}

		
		}
	}
}
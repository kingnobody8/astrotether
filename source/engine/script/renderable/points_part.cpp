#include "points_part.h"
#include "../script.h"
#include "render/node/point_node.h"
#include "render/node/line_node.h"
#include "component/object.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			DEFINE_PART_TYPE_INFO(CPointsPart);

			CPointsPart::CPointsPart()
				: m_render_mode(INVALID)
			{
				m_priority = EPriorities::POINTS_PART;
			}
			VIRTUAL CPointsPart::~CPointsPart()
			{
				ClearNodes();
			}
			CPointsPart& CPointsPart::operator = (const CPointsPart& that)
			{
				IRenderPart::operator=(that);
				return *this;
			}

			VIRTUAL void CPointsPart::Init()
			{
				IRenderPart::Init();
				m_force = true;
				SetPointsAndRenderMode(m_points, m_render_mode);
				m_force = false;
			}

			VIRTUAL void CPointsPart::LoadJson(const util::JSON& json)
			{
				IRenderPart::LoadJson(json);
				
				assert(json.HasMember("render_mode"));
				assert(json.HasMember("points"));

				m_render_mode = (ERenderMode)json["render_mode"].GetInt();
				m_points = ((const util::JSON&)(json["points"])).GetArray<util::math::vec2>();

				assert(m_render_mode > INVALID && m_render_mode < COUNT);
			}

			void CPointsPart::OnMatrixChangedPoints()
			{
				const std::vector<util::math::vec2> wpoints = CalcWorldPoints();

				for (int i = 0; i < m_nodes.size(); ++i)
				{
					render::CRenderNodePoint* node = static_cast<render::CRenderNodePoint*>(m_nodes[i]);
					node->SetPoint(wpoints[i]);
				}
			}

			void CPointsPart::OnMatrixChangedLines()
			{
				const std::vector<util::math::vec2> wpoints = CalcWorldPoints();
				for (int i = 0; i < m_nodes.size(); ++i)
				{
					const int pindex = (i + 1) % wpoints.size();
					const util::math::vec2& p1 = wpoints[i];
					const util::math::vec2& p2 = wpoints[pindex];

					render::CRenderNodeLine* node = static_cast<render::CRenderNodeLine*>(m_nodes[i]);
					node->SetLine(util::shape::Segment(p1, p2));
				}
			}

			void CPointsPart::SetPointsAndRenderMode(const std::vector<util::math::vec2>& points, ERenderMode mode)
			{
				assert(!points.empty());
				assert(mode > INVALID && mode < COUNT);

				if (!m_force && (m_render_mode == mode) && (m_points == points))
					return;

				//stupidity handling
				if (m_points.size() == 1) // if we only have one point, then we shouldn't draw lines
					mode = ERenderMode::POINTS;
				else if (m_points.size() == 2 && mode == ERenderMode::CLOSED_LINE) //if only have two points and we want to draw a closed line it would just trace back on itself
					mode = ERenderMode::OPEN_LINE;

				m_points = points;
				m_render_mode = mode;

				SetupNodes();
			}

			void CPointsPart::ClearNodes()
			{
				if (!m_nodes.empty())
				{
					for (int i = 0; i < (int)m_nodes.size(); ++i)
					{
						SafeDelete(m_nodes[i]);
					}
					m_nodes.clear();
				}
			}

			void CPointsPart::SetupNodes()
			{
				ClearNodes();

				switch (m_render_mode)
				{
				case ERenderMode::POINTS:
					SetupPoints();
					break;
				case ERenderMode::OPEN_LINE:
					SetupLines(m_points.size() - 1);
					break;
				case ERenderMode::CLOSED_LINE:
					SetupLines(m_points.size());
					break;
				default:
					assert(false);
					break;
				}
				IRenderPart::Init(); __todo() //the sets that init does are needed since the nodes are recreated when the points are changed
			}

			void CPointsPart::SetupPoints()
			{
				const std::vector<util::math::vec2> wpoints = CalcWorldPoints();

				m_nodes.reserve(wpoints.size());
				for (int i = 0; i < wpoints.size(); ++i)
				{
					m_nodes.push_back(new render::CRenderNodePoint());
				}
			}

			void CPointsPart::SetupLines(const int& count)
			{
				const std::vector<util::math::vec2> wpoints = CalcWorldPoints();

				m_nodes.reserve(count);
				for (int i = 0; i < count; ++i)
				{
					const int pindex = (i + 1) % wpoints.size();
					const util::math::vec2& p1 = wpoints[i];
					const util::math::vec2& p2 = wpoints[pindex];

					m_nodes.push_back(new render::CRenderNodeLine());
					m_nodes[i]->Register(m_szLayer);
					m_nodes[i]->SetColor(m_color);
					static_cast<render::CRenderNodeLine*>(m_nodes[i])->SetLine(util::shape::Segment(p1, p2));
				}
			}

			const std::vector<util::math::vec2> CPointsPart::CalcWorldPoints() const
			{
				std::vector<util::math::vec2> ret;
				ret.reserve(m_points.size());
				const util::math::Matrix2D wmat = m_pOwner->CalcWorldMatrix();
				for (int i = 0; i < m_points.size(); ++i)
					ret.push_back(util::math::Matrix2D::Vector_Matrix_Multiply(m_points[i], wmat));
				return ret;
			}

			//IRenderPart overrides

			VIRTUAL void CPointsPart::OnZedChanged()
			{
				const float wzed = m_pOwner->CalcWorldZed();
				for (int i = 0; i < m_nodes.size(); ++i)
				{
					m_nodes[i]->SetZed(wzed);
				}
			}

			VIRTUAL void CPointsPart::OnMatrixChanged()
			{
				switch (m_render_mode)
				{
				case ERenderMode::POINTS:
					OnMatrixChangedPoints();
					break;
				case ERenderMode::CLOSED_LINE:
				case ERenderMode::OPEN_LINE:
					OnMatrixChangedLines();
					break;
				default:
					assert(false);
					break;
				}
			}

			VIRTUAL void CPointsPart::OnVisibilityChanged(const bool visible)
			{
				if (!m_force && (visible == (m_pNode->GetLayer() != null)))
					return;

				if (visible)
				{
					for (int i = 0; i < m_nodes.size(); ++i)
						m_nodes[i]->Register(m_szLayer);
				}
				else
				{
					for (int i = 0; i < m_nodes.size(); ++i)
						m_nodes[i]->Unregister();
				}
			}

			VIRTUAL const b2AABB CPointsPart::CalcAABB(void)
			{
				const std::vector<util::math::vec2> wpoints = CalcWorldPoints();

				b2AABB ret = INVALID_AABB;
				for (int i = 0; i < wpoints.size(); ++i)
				{
					if (!ret.IsValid())
						ret.lowerBound = ret.upperBound = b2Vec2(wpoints[i].x, wpoints[i].y);
					else
						ret.Combine(b2Vec2(wpoints[i].x, wpoints[i].y));
				}
				return ret;
			}

			VIRTUAL const bool CPointsPart::IsRegistered() const
			{
				if (m_nodes.empty())
					return false;
				return (m_nodes[0]->GetLayer() != null);
			}

			VIRTUAL void CPointsPart::SetLocalColor(const util::Color& clr)
			{
				if (!m_force && (m_color == clr))
					return;

				__todo() //this needs to do a vertical search for ColorModPart and see what colors it applies to us so we can properly set the nod
				m_color = clr;
				for (int i = 0; i < m_nodes.size(); ++i)
					m_nodes[i]->SetColor(clr);
			}

			VIRTUAL void CPointsPart::SetLayer(const std::string& szLayer)
			{
				if (!m_force && (m_szLayer == szLayer))
					return;

				m_szLayer = szLayer;
				if (m_pOwner->GetWorldVisible())
				{
					for (int i = 0; i < m_nodes.size(); ++i)
					{
						m_nodes[i]->Register(m_szLayer);
					}
				}
			}
		}
	}
}
#include "render_layer.h"
#include "node/render_node.h"
#include "camera.h"
#include <algorithm>

namespace baka
{
	namespace render
	{
		bool SortNodeFunc(IRenderNode* lhs, IRenderNode* rhs)
		{
			return lhs->GetZed() < rhs->GetZed();
		}

		bool SortLayerFunc(CRenderLayer* lhs, CRenderLayer* rhs)
		{
			return lhs->GetSortRank() > rhs->GetSortRank();
		}

		STATIC std::list<CRenderLayer*> CRenderLayer::s_layers;

		STATIC CRenderLayer* CRenderLayer::FindLayer(const std::string& szLayer)
		{
			for (auto iter = s_layers.begin(); iter != s_layers.end(); ++iter)
			{
				if ((*iter)->GetName() == szLayer)
				{
					return (*iter);
				}
			}
			return null;
		}

		STATIC CRenderLayer* CRenderLayer::CreateLayer(const std::string& name, const int& sort_rank, CCamera* const pCam)
		{
			assert(!name.empty() || pCam != null);

			CRenderLayer* layer = new CRenderLayer(name, sort_rank, pCam);
			s_layers.push_back(layer);
			s_layers.sort(SortLayerFunc);
			return layer;
		}

		STATIC void CRenderLayer::DestroyLayers()
		{
			for (auto iter = s_layers.begin(); iter != s_layers.end(); ++iter)
				delete (*iter);
			s_layers.clear();
		}

		STATIC void CRenderLayer::RenderAllLayers()
		{
			for (auto iter = s_layers.begin(); iter != s_layers.end(); ++iter)
				(*iter)->DoRender();
		}

		CRenderLayer::CRenderLayer(void)
			: m_pCamera(null)
			, m_sort_rank(0)
		{
		}

		CRenderLayer::CRenderLayer(const std::string& name, const int& sort_rank, CCamera* const pCam)
			: m_pCamera(pCam)
			, m_name(name)
			, m_sort_rank(sort_rank)
		{
		}

		void CRenderLayer::Cull(const b2AABB& view)
		{
			//Copy the real list to the temp list
			this->m_vCulledNodes.clear();
			this->m_vCulledNodes = this->m_vNodes;

			__todo()//TODO actually cull the nodes
			this->m_vCulledNodes.sort(SortNodeFunc);
		}

		void CRenderLayer::DoRender()
		{
			if (!m_bVisible)
				return;

			const b2AABB view = m_pCamera->CalcViewAabb();
			Cull(view);

			util::Matrix inv_cam;// = glm::inverse(m_pCamera->GetMatrix());
			for (auto iter = m_vCulledNodes.begin(); iter != m_vCulledNodes.end(); ++iter)
			{
				(*iter)->operator()(inv_cam);
			}
		}

		void CRenderLayer::RegisterNode(IRenderNode* pNode)
		{
			assert(pNode);
			auto find = std::find(this->m_vNodes.begin(), this->m_vNodes.end(), pNode);
			if (find == this->m_vNodes.end())
			{
				this->m_vNodes.push_back(pNode);
				this->m_vNodes.sort(SortNodeFunc);
			}
		}

		void CRenderLayer::UnregisterNode(IRenderNode* pNode)
		{
			assert(pNode);
			this->m_vNodes.remove(pNode);
		}

		void CRenderLayer::ClearAllNodes(void)
		{
			this->m_vCulledNodes.clear();
			this->m_vNodes.clear();
		}

		const vec2 CRenderLayer::ConvertPointFromScreenToWorld(vec2 m_screen_point)
		{
			//get screen info
			glm::i32vec2 logical_size;
			//SDL_GetWindowSize(render::GetSdlWindow(), &logical_size.x, &logical_size.y);
			vec2 origin(logical_size.x * 0.5f, logical_size.y * 0.5f);

			m_screen_point.x = m_screen_point.x - origin.x;
			m_screen_point.y = m_screen_point.y - origin.y;

			const vec4 tmp;// = m_pCamera->GetMatrix() * vec4(m_screen_point.x, m_screen_point.y, 0.0f, 1.0f);
			return vec2(tmp.x, tmp.y);
		}

		const vec2 CRenderLayer::ConvertPointFromWorldToScreen(vec2 m_world_point)
		{
			const vec4 tmp;// = glm::inverse(m_pCamera->GetMatrix()) * vec4(m_world_point.x, m_world_point.y, 0.0f, 1.0f);
			return vec2(tmp.x, tmp.y);
		}
	}
}
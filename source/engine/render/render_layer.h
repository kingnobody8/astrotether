#pragma once
#include <list>
#include "utility/helper/macro.h"
#include <string>
#include "utility/math/math.h"

namespace baka
{
	namespace render
	{
		//Forward Declare
		class IRenderNode;
		class CCamera;

		class CRenderLayer
		{
		private:
			static std::list<CRenderLayer*> s_layers;

		public:
			static CRenderLayer* FindLayer(const std::string& szLayer);
			static CRenderLayer* CreateLayer(const std::string& name, const int& sort_rank, CCamera* const pCam);
			static void DestroyLayers();
			static void RenderAllLayers();

		private:
			std::list<IRenderNode*> m_vNodes;
			std::list<IRenderNode*> m_vCulledNodes;
			CCamera* m_pCamera;
			std::string m_name;
			int m_sort_rank;
			bool m_bVisible;

		private:
			void Cull(const b2AABB& view);

			CRenderLayer();
			CRenderLayer(const std::string& name, const int& sort_rank, CCamera* const pCam);

		public:
			void RegisterNode(IRenderNode* pNode);
			void UnregisterNode(IRenderNode* pNode);
			void ClearAllNodes(void);
			void DoRender();
			inline CCamera* const GetCamera(void) const { return m_pCamera; }
			inline const std::string& GetName() const { return m_name; }
			inline const int& GetSortRank() const { return m_sort_rank; }
			const vec2 ConvertPointFromScreenToWorld(vec2 m_screen_point);
			const vec2 ConvertPointFromWorldToScreen(vec2 m_world_point);

			inline void SetVisible(const bool b) { m_bVisible = b; }
			inline bool GetVisible() const { return m_bVisible; }
		};
	}
}
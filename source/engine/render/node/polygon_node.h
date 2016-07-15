#pragma once
#include "render_node.h"
#include <vector>

namespace engine
{
	namespace render
	{
		class PolygonNode : public IRenderNode
		{
		private: __todo() //maybe we should template this class so that we can have different types of verts
			Vertex* m_vVerts;
			int	m_vertCount;
			int* m_vIndicies;
			int m_indexCount;

		protected:
			virtual void CalcAabbInternal();

		public:
			PolygonNode();
			virtual ~PolygonNode();
			virtual void operator () (const util::Matrix& inv_cam);

			const std::vector<Vertex> GetVerts();
			const std::vector<int> GetIndicies();

			void InitVBO(const std::vector<Vertex>& verts, const std::vector<int>& indicies);
			virtual void FreeVBO();
		};
	}
}
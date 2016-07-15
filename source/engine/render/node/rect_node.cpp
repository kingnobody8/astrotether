#include "rect_node.h"

namespace engine
{
	namespace render
	{
		RectNode::RectNode()
			: m_bFill(true)
		{
		}

		void RectNode::InitVBO(const vec2& extends, const util::Color& clr)
		{
			std::vector<Vertex> verts;
			verts.resize(4);
			verts[0] = Vertex(vec2(-extends.x, -extends.y), vec2(), clr);
			verts[1] = Vertex(vec2(-extends.x, extends.y), vec2(), clr);
			verts[2] = Vertex(vec2(extends.x, extends.y), vec2(), clr);
			verts[3] = Vertex(vec2(extends.x, -extends.y), vec2(), clr);

			std::vector<int> indicies;
			indicies.resize(4);
			indicies[0] = 0;
			indicies[1] = 1;
			indicies[2] = 2;
			indicies[3] = 3;

			PolygonNode::InitVBO(verts, indicies);
		}
	}
}
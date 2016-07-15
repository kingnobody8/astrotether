#include "circle_node.h"
#include "math/consts.h"

namespace engine
{
	namespace render
	{
		CircleNode::CircleNode()
			: m_bFill(true)
		{
		}

		void CircleNode::InitVBO(const glm::float32& radius, const int& segments, const util::Color& clr)
		{
			const float32 increment = 2.0f * b2_pi / segments;
			float32 sinInc = sinf(increment);
			float32 cosInc = cosf(increment);
			vec2 inc(cosInc, sinInc);

			std::vector<Vertex> verts;
			verts.resize(segments + 2);
			verts[0] = Vertex(vec2(), vec2(), clr);
			for (int i = 1; i < segments+2; ++i)
			{
				vec2 tmp;
				tmp.x = sin((float(i - 1) / segments+1) * 360 * DEG_TO_RAD);// cosInc * inc.x - sinInc * inc.y;
				tmp.y = cos((float(i - 1) / segments+1) * 360 * DEG_TO_RAD);// sinInc * inc.y + cosInc * inc.y;
				tmp *= radius;
				verts[i] = Vertex(tmp, vec2(), clr);
			}

			std::vector<int> indicies;
			indicies.resize(segments + 2);
			for (int i = 0; i < segments + 2; ++i)
			{
				indicies[i] = i;
			}

			PolygonNode::InitVBO(verts, indicies);
		}
	}
}
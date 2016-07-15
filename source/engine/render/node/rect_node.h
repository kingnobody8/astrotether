#pragma once
#include "polygon_node.h"

namespace engine
{
	namespace render
	{
		class RectNode : public PolygonNode
		{
		private:
			bool m_bFill;

		public:
			RectNode();
			void InitVBO(const vec2& extends, const util::Color& clr);
		};
	}
}
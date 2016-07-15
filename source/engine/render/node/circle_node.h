#pragma once
#include "polygon_node.h"

namespace engine
{
	namespace render
	{
		class CircleNode : public PolygonNode
		{
		private:
			bool m_bFill; __todo() // move this into polygon??

		public:
			CircleNode();
			void InitVBO(const glm::float32& radius, const int& segments, const util::Color& clr);
		};
	}
}
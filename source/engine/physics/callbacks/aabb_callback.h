#pragma once
#include "box2d/box2d.h"
#include "utility/types/types.h"

namespace engine
{
	namespace physics
	{
		namespace callbacks
		{
			//Callback for an aabb
			class AabbCallback : public b2QueryCallback
			{
			public:
				AabbCallback(const b2Vec2& point)
					: m_point(point), m_pFixture(null)
				{
				}

				bool ReportFixture(b2Fixture* fixture)
				{
					b2Body* body = fixture->GetBody();
					if (body->GetType() == b2_dynamicBody)
					{
						bool inside = fixture->TestPoint(m_point);
						if (inside)
						{
							m_pFixture = fixture;

							// We are done, terminate the query.
							return false;
						}
					}

					// Continue the query.
					return true;
				}

				b2Vec2 m_point;
				b2Fixture* m_pFixture;
			};
		}
	}
}
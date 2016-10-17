#pragma once
#include "box2d/box2d.h"
#include "utility/types/types.h"

namespace baka
{
	namespace physics
	{
		namespace callbacks
		{
			// This callback finds the closest hit. Polygon 0 is filtered.
			class RayCastClosestCallback : public b2RayCastCallback
			{
			public:
				RayCastClosestCallback()
					: m_pFixture(null)
					, m_bHit(null)
				{
				}

				float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
				{
					b2Body* body = fixture->GetBody();
					void* userData = body->GetUserData();
					if (userData)
					{
						int32 index = *(int32*)userData;
						if (index == 0)
						{
							// By returning -1, we instruct the calling code to ignore this fixture and
							// continue the ray-cast to the next fixture.
							return -1.0f;
						}
					}

					m_bHit = true;
					m_point = point;
					m_normal = normal;
					m_pFixture = fixture;

					// By returning the current fraction, we instruct the calling code to clip the ray and
					// continue the ray-cast to the next fixture. WARNING: do not assume that fixtures
					// are reported in order. However, by clipping, we can always get the closest fixture.
					return fraction;
				}

				b2Fixture* m_pFixture;
				b2Vec2 m_point;
				b2Vec2 m_normal;
				bool m_bHit;
			};

			// This callback finds any hit. Polygon 0 is filtered. For this type of query we are usually
			// just checking for obstruction, so the actual fixture and hit point are irrelevant. 
			class RayCastAnyCallback : public b2RayCastCallback
			{
			public:
				RayCastAnyCallback()
					: m_bHit(false)
				{
				}

				float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
				{
					B2_NOT_USED(fraction);
					b2Body* body = fixture->GetBody();
					void* userData = body->GetUserData();
					if (userData)
					{
						int32 index = *(int32*)userData;
						if (index == 0)
						{
							// By returning -1, we instruct the calling code to ignore this fixture
							// and continue the ray-cast to the next fixture.
							return -1.0f;
						}
					}

					m_bHit = true;
					m_point = point;
					m_normal = normal;

					// At this point we have a hit, so we know the ray is obstructed.
					// By returning 0, we instruct the calling code to terminate the ray-cast.
					return 0.0f;
				}

				bool m_bHit;
				b2Vec2 m_point;
				b2Vec2 m_normal;
			};

			// This ray cast collects multiple hits along the ray. Polygon 0 is filtered.
			// The fixtures are not necessary reported in order, so we might not capture
			// the closest fixture.
			class RayCastMultipleCallback : public b2RayCastCallback
			{
			public:
				enum
				{
					e_maxCount = 3
				};

				RayCastMultipleCallback()
					: m_count(0)
				{
				}

				float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
				{
					B2_NOT_USED(fraction);
					b2Body* body = fixture->GetBody();
					void* userData = body->GetUserData();
					if (userData)
					{
						int32 index = *(int32*)userData;
						if (index == 0)
						{
							// By returning -1, we instruct the calling code to ignore this fixture
							// and continue the ray-cast to the next fixture.
							return -1.0f;
						}
					}

					b2Assert(m_count < e_maxCount);

					m_points[m_count] = point;
					m_normals[m_count] = normal;
					++m_count;

					if (m_count == e_maxCount)
					{
						// At this point the buffer is full.
						// By returning 0, we instruct the calling code to terminate the ray-cast.
						return 0.0f;
					}

					// By returning 1, we instruct the caller to continue without clipping the ray.
					return 1.0f;
				}

				b2Vec2 m_points[e_maxCount];
				b2Vec2 m_normals[e_maxCount];
				int32 m_count;
			};
		}
	}
}
#include "util_math.h"

namespace util
{
	namespace math
	{
		typedef b2CircleShape		Circle;
		typedef b2AABB				Aabb;
		
		class InfiniteShape : public b2Shape //todo move this to its own file
		{
			/// Implement b2Shape.
			b2Shape* Clone(b2BlockAllocator* allocator) const
			{
				void* mem = allocator->Allocate(sizeof(InfiniteShape));
				InfiniteShape* clone = new (mem)InfiniteShape;
				*clone = *this;
				return clone;
			}

			/// @see b2Shape::GetChildCount
			int32 GetChildCount() const
			{
				return 1;
			}

			/// Implement b2Shape.
			bool TestPoint(const b2Transform& transform, const b2Vec2& p) const
			{
				return true;
			}

			// @see b2Shape::ComputeDistance
			void ComputeDistance(const b2Transform& xf, const b2Vec2& p, float32* distance, b2Vec2* normal, int32 childIndex) const
			{
				*distance = 0.0f;
				*normal = p;
			}

			/// Implement b2Shape.
			bool RayCast(b2RayCastOutput* output, const b2RayCastInput& input,
				const b2Transform& transform, int32 childIndex) const
			{
				return true;
			}

			/// @see b2Shape::ComputeAABB
			void ComputeAABB(b2AABB* aabb, const b2Transform& transform, int32 childIndex) const
			{
				*aabb = INVALID_AABB;
			}

			/// @see b2Shape::ComputeMass
			void ComputeMass(b2MassData* massData, float32 density) const
			{
			}
		};
	}
}
#pragma once
#include "util_math.h"

namespace util
{
	class Box : public AABB
	{
	public:
		Box()
		{
		}
		Box(const vec2& min, const vec2& max, const Angle& rotation)
			: m_rotation(rotation)
		{
			lowerBound.Set(min.x, min.y);
			upperBound.Set(max.x, max.y);
		}

		__todo() //make an aabb class with extra funcs like this
		const vec2 CalculatePointWithPivot(const vec2& pivot) const
		{
			const float width = (upperBound.x - lowerBound.x) * pivot.x;
			const float height = (upperBound.y - lowerBound.y) * pivot.y;
			return vec2(lowerBound.x + width, lowerBound.y + height);
		}
		vec2 CalculateCenter() const
		{
			return (vec2(lowerBound.x, lowerBound.y) + vec2(upperBound.x, upperBound.y)) / 2.0f;
		}


		inline const Angle GetRotation() const { return m_rotation; }
		inline void SetRotation(const Angle rotation) { m_rotation = rotation; }

	private:
		Angle m_rotation;
	};
}

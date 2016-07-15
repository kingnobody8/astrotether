#pragma once
#include "util_math.h"

namespace util
{
	class Angle
	{
	public:
		Angle();
		Angle(const float& rad);

		inline float GetDegrees() const { return m_radians * RAD_TO_DEG; }
		inline float GetRadians() const { return m_radians; }

		void Normalize360();
		void Normalize180();

		Angle operator-(const Angle& other) const;
		Angle& operator-=(const Angle& other);
		Angle operator+(const Angle& other) const;
		Angle& operator+=(const Angle& other);

		inline operator float() const { return m_radians; }

		static Angle FromDegrees(float deg);
		static Angle FromRadians(float rad);

		static float WrapRadians(float radians);
		static Angle Delta(const Angle& a, const Angle& b);

		static float Normalize360(float rads);
		static float Normalize180(float rads);

	private:
		float m_radians;
	};
}


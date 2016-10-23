#pragma once
#include "types/types.h"
#include "box2d/Box2D.h"
#include "glm/glm.hpp"

namespace util
{
	// TYPEDEFS
	typedef glm::f32vec2		vec2;
	typedef glm::f32vec3		vec3;
	typedef glm::f32vec4		vec4;
//	typedef glm::f32mat4x4		Matrix;

	typedef b2AABB				AABB;


	// CONSTS
	const float DEG_TO_RAD = 0.0174532925f;
	const float RAD_TO_DEG = 57.2957795f;
	const float DEGREE_360 = 360.0f;
	const float DEGREE_180 = 180.0f;
	const float DEGREE_90 = 90.0f;
	const float kPI = 3.14159f;			// Pie is not round, pie r squared
	const float kTWO_PI = 2.0f * kPI;
	const float EPSILON = 0.0001f;				// Roundoff error to be ignored in floating point number comparisons

	// MACROS
#define EPSI(test, goal) (test < goal + EPSILON && test > goal - EPSILON)

	const b2AABB INVALID_AABB = b2AABB(b2Vec2(1.0f, 1.0f), b2Vec2(-1.0f, -1.0f));


	// FUNCTIONS

	template<typename TReal>
	inline bool IsNaN(const TReal& a)
	{
		return a != a;
	}
	template<typename TReal>
	inline int Sign(TReal a)
	{
		return a >= 0 ? 1 : -1;
	}
}

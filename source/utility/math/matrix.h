#pragma once
#include "util_math.h"
#include "glm/glm.hpp"
#include "glm/Matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "angle.h"

namespace util
{
	// | a b c 0 |
	// | d e f 0 |
	// | 0 0 1 0 |
	// | 0 0 0 1 |

	class Matrix : public glm::f32mat4x4
	{
	public:
		Matrix() {}
		Matrix(const glm::f32mat4x4& that) { *this = that; }
		Matrix& operator= (const glm::f32mat4x4& that)
		{
			glm::f32mat4x4::operator=(that);
			return *this;
		}

		Matrix& BuildTransformationMatrix(const vec2& translation, const vec2& scale, const Angle& rotation, const vec2& pivot);

		const vec2 GetTranslation() const;
		const vec2 CalculateScale() const;
		const Angle CalculateRotation() const;


		Matrix& SelfMultiply(const Matrix& mat);
	};
}

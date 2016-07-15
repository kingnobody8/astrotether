#include "Matrix.h"
#include <math.h>

const int CR_POS_X = 1;
const int CR_POS_Y = 1;
const int CR_POS_Z = 1;

namespace util
{
	Matrix& Matrix::BuildTransformationMatrix(const vec2& translation, const vec2& scale, const Angle& rotation, const vec2& pivot)
	{
		// https://en.wikipedia.org/wiki/Transformation_matrix

		glm::float32 rx = cos(rotation);
		glm::float32 ry = sin(rotation);

		glm::float32 sx = scale.x;
		glm::float32 sy = scale.y;

		glm::float32 tx = translation.x;
		glm::float32 ty = translation.y;

		glm::float32 px = pivot.x;
		glm::float32 py = pivot.y;

		glm::float32 a = sx * rx;
		glm::float32 b = -sy * ry;
		glm::float32 c = px * sx * rx - py * sy * ry + tx;

		glm::float32 d = sx * ry;
		glm::float32 e = sy * rx;
		glm::float32 f = px * sx * ry + py * sy * rx + ty;

		this->value[0] = vec4(a, b, c, 0);
		this->value[1] = vec4(d, e, f, 0);
		this->value[2] = vec4(0, 0, 1, 0);
		this->value[3] = vec4(0, 0, 0, 1);

		return *this;
	}

	const vec2 Matrix::GetTranslation() const
	{
		vec2 outTranslation;
		outTranslation.x = this->value[0].z;
		outTranslation.y = this->value[1].z;
		return outTranslation;
	}
	
	const vec2 Matrix::CalculateScale() const
	{
		vec2 outScale;
		outScale.x = sqrt(pow(this->value[0].x, 2) + pow(this->value[1].x, 2));
		outScale.y = sqrt(pow(this->value[0].y, 2) + pow(this->value[1].y, 2));
		return outScale;
	}
	
	const Angle Matrix::CalculateRotation() const
	{
		vec2 scale = CalculateScale();
		Angle outAngle;
		outAngle = Angle::FromRadians(atan2(-this->value[0].y / scale.y, this->value[0].x / scale.x));
		return outAngle;
	}

	Matrix& Matrix::SelfMultiply(const Matrix& mat)
	{
		*this = mat * *this;
		return *this;
	}

}
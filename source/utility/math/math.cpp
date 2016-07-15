#include "math.h"

//GLM to BOX2D
const b2Vec2 Tob2(const vec2& v)
{
	return b2Vec2(v.x, v.y);
}
const b2Vec3 Tob2(const vec3& v)
{
	return b2Vec3(v.x, v.y, v.z);
}
const b2Vec4 Tob2(const vec4& v)
{
	return b2Vec4(v.x, v.y, v.z, v.w);
}

//BOX2D to GLM
const vec2 Toglm(const b2Vec2& v)
{
	return vec2(v.x, v.y);
}
const vec3 Toglm(const b2Vec3& v)
{
	return vec3(v.x, v.y, v.z);
}
const vec4 Toglm(const b2Vec4& v)
{
	return vec4(v.x, v.y, v.z, v.w);
}

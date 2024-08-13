#pragma once

#include <cmath>
#include <limits>
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4x4.h"


const float PI = 3.14159265358f;
const float EPSILON = 0.00001f;

const float DEG2RAD = (PI * 2.0f) / 360.0f;
const float RAD2DEG = 360.0f / (PI * 2.0f);

const float POS_INFINITY = std::numeric_limits<float>::infinity();
const float NEG_INFINITY = std::numeric_limits<float>::infinity();

using Vec2 = Vector2;
using Vec3 = Vector3;
using Mat4 = Matrix4x4;

class Math
{
public:
	static float Clamp(const float& val, const float& min, const float& max);
	static float Lerp(const float& a, const float& b, const float& t);

};


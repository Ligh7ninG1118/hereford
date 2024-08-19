#pragma once

#include <cmath>
#include <limits>
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"


const float PI = 3.14159265358f;
const float EPSILON = 0.00001f;

const float DEG2RAD = (PI * 2.0f) / 360.0f;
const float RAD2DEG = 360.0f / (PI * 2.0f);

const float POS_INFINITY = std::numeric_limits<float>::infinity();
const float NEG_INFINITY = std::numeric_limits<float>::infinity();

using Vec2 = Vector2;
using Vec3 = Vector3;
using Mat4 = Matrix4x4;
using Quat = Quaternion;

using Uint32 = unsigned int;

class Math
{
public:
	template <typename T>
	static T Clamp(const T& val, const T& min, const T& max)
	{
		if (val < min)
			return min;
		else if (val > max)
			return max;
		else
			return val;
	}
	
	template <typename T>
	static T Lerp(const T& a, const T& b, const float& t)
	{
		return a * (1.0f - t) + b * t;
	}

	static Quat Slerp(const Quat& a, const Quat& b, const float& t)
	{
		float cosTheta = a.Dot(b);
		float sign = 1.0f;
		if (cosTheta < 0.0f)
		{
			cosTheta = -cosTheta;
			sign = -1.0f;
		}

		float scaleA, scaleB;

		if (1.0f - cosTheta > EPSILON)
		{
			float theta = acosf(cosTheta);
			float sinTheta = sin(theta);
			scaleA = sin((1.0f - t) * theta) / sinTheta;
			scaleB = sign * sin(t * theta) / sinTheta;
		}
		// If two quat are almost identical, do a LERP instead (THANK YOU JOLT)
		else
		{
			scaleA = 1.0f - t;
			scaleB = sign * t;
		}

		Quat result = scaleA * a + scaleB * b;
		return result.normalized();
	}

};


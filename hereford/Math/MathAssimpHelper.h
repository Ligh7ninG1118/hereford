#pragma once
#include "Math/Math.h"
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>
#include <assimp/vector2.h>


static class MathAssimpHelper
{
public:
	inline static Mat4 ConvertAssimpMat4(const aiMatrix4x4& from)
	{
		Mat4 to;
		to.m[0][0] = from.a1; to.m[0][1] = from.a2; to.m[0][2] = from.a3; to.m[0][3] = from.a4;
		to.m[1][0] = from.b1; to.m[1][1] = from.b2; to.m[1][2] = from.b3; to.m[1][3] = from.b4;
		to.m[2][0] = from.c1; to.m[2][1] = from.c2; to.m[2][2] = from.c3; to.m[2][3] = from.c4;
		to.m[3][0] = from.d1; to.m[3][1] = from.d2; to.m[3][2] = from.d3; to.m[3][3] = from.d4;
		return to;
	}

	inline static Quat ConvertAssimpQuat(const aiQuaternion& from)
	{
		return Quat(from.x, from.y, from.z, from.w);
	}

	inline static Vec3 ConvertAssimpVec3(const aiVector3D& from)
	{
		return Vec3(from.x, from.y, from.z);
	}

	inline static Vec2 ConvertAssimpVec2(const aiVector2D& from)
	{
		return Vec2(from.x, from.y);
	}

	inline static Vec2 ConvertAssimpVec2(const aiVector3D& from)
	{
		return Vec2(from.x, from.y);
	}

};
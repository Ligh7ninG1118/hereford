#pragma once
#include "Math/Math.h"
#include <variant>

struct SpherePrimitive
{
	float mRadius;
};

struct AABBPrimitive
{
	Vec3 mExtend;
};

struct PlanePrimitive
{
	Vec3 mNormal;
	float mDistance;
};

struct CapsulePrimitive
{
	float mRadius;
	float mHalfHeight;
};

struct PhysicsPrimitive
{
	std::variant<
		SpherePrimitive,
		AABBPrimitive,
		PlanePrimitive,
		CapsulePrimitive> mPrimitive;
	Vec3 mPosOffset;
};
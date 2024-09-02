#pragma once
#include "Math/Math.h"

struct EventOnAnimFinished
{
	int mAnimIndex;

	EventOnAnimFinished(int animIndex)
		: mAnimIndex(animIndex)
	{}
};

struct EventOnPlayerWeaponFired
{
	Vec2 mRecoilDeviation;

	EventOnPlayerWeaponFired(Vec2 recoilDev)
		: mRecoilDeviation(recoilDev)
	{}
};
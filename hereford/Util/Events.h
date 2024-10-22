#pragma once
#include "Math/Math.h"

struct EventOnAnimFinished
{
	int mAnimIndex;

	EventOnAnimFinished(int animIndex)
		: mAnimIndex(animIndex)
	{}
};

struct EventOnWeaponAmmoChanged
{
	int ammoLeft;
	int ammoMax;

	EventOnWeaponAmmoChanged(int left, int max)
		: ammoLeft(left), ammoMax(max)
	{}
};

struct EventOnWeaponAiming
{
	float aimingProgress;

	EventOnWeaponAiming(float progress)
		: aimingProgress(progress)
	{}
};
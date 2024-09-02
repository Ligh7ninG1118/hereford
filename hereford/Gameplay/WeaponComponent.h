#pragma once
#include "Core/Component.h"
#include "Math/Math.h"

enum class ERecoilType
{
	ACCURACY_SPREAD = 0,
	RECOIL_DIAMOND,
	RECOIL_RIG,
};


class WeaponComponent : public Component
{
	friend class Weapon;

public:
	WeaponComponent(class Actor* owner);
	~WeaponComponent();

	void Update(float deltaTime) override;

	bool TryFire(bool checkOnly = false);
	bool TryReload();
	void ApplyReload();

	Vec2 CalculateRecoilDeviation() const;
	
private:
	class Weapon* mWeapon;
	// General
	float mReloadAnimDuration;

	// Fire Rate Parameters
	bool mIsSemiAuto;
	float mFireRatePerMin;
	float mFireRateCooldown;

	// Fire Rate Runtime Variables
	float mFireRateTimer;
	bool mIsLMBPressed;

	// Recoil and Heat Parameters
	ERecoilType mRecoilType;
	Vec2 mRecoilDiamond;
	float mAccuracySpreadFactor;

	float mHeatReduceDelayCooldown;
	float mHeatReduceRatePerSec;

	// Recoil and Heat Runtime Variables
	float mHeatReduceDelayTimer;
	float mCurrentHeat;

	// Ammo Parameters
	uint16 mMaxMagazineCapacity;
	uint16 mMaxReserveCapacity;
	bool mIsOpenBolt;

	// Ammo Runtime Variables
	uint16 mCurrentMagazineAmmo;
	uint16 mCurrentReserveAmmo;

};


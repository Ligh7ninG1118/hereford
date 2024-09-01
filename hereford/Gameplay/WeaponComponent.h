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
public:
	WeaponComponent(class Actor* owner);
	~WeaponComponent();

	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY) override;

	void Fire();
	void Reload();
	
private:
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
	//TODO: switch to use short/uint16
	int mMaxMagazineCapacity;
	int mMaxReserveCapacity;
	bool mIsOpenBolt;

	// Ammo Runtime Variables
	int mCurrentMagazineAmmo;
	int mCurrentReserveAmmo;

};


#pragma once
#include "Core/OldComponent.h"
#include "Math/Math.h"
#include "Util/DelayedAction.h"


enum class EWeaponState
{
	READY = 0,
	RELOADING,
	SWAPPING
};

enum class ERecoilType
{
	ACCURACY_SPREAD = 0,
	RECOIL_DIAMOND,
	RECOIL_RIG,
};


class WeaponComponent : public OldComponent
{
	friend class Weapon;
	friend class WeaponPistol;
	friend class WeaponSMG;


public:
	WeaponComponent(class Actor* owner, class AnimationStateMachine* inASM);
	~WeaponComponent();

	void Update(float deltaTime) override;

	bool CanFire();
	void Fire();
	bool CanReload();
	void FinishedReload();

	inline uint16 GetCurrentMagazineAmmo() const { return mCurrentMagazineAmmo; }
	inline uint16 GetCurrentReserveAmmo() const { return mCurrentReserveAmmo; }

	inline uint16 GetMaxMagazineCapacity() const { return mMaxMagazineCapacity; }
	inline uint16 GetMaxReserveCapacity() const { return mMaxReserveCapacity; }

	inline void SetAccuracySpreadMultiplier(float multiplier = 1.0f) { mTargetAccuracySpreadFactor = mDefaultAccuracySpreadFactor * multiplier; }

	inline float GetAccuracyDeviation() const { return mAccuracySpreadFactor; }
	
private:
	Vec2 CalculateRecoilDeviation() const;

	DAHandle mHReloadCallback;

	class AnimationStateMachine* mAnimStateMachine;

	EWeaponState mCurrentState;
	

	// General
	

	// Fire Rate Parameters
	bool mIsSemiAuto;
	float mFireRatePerMin;
	float mFireRateCooldown;

	// Fire Rate Runtime Variables
	float mFireRateTimer;

	// Recoil and Heat Parameters
	ERecoilType mRecoilType;
	Vec2 mRecoilDiamond;
	float mAccuracySpreadFactor;
	float mDefaultAccuracySpreadFactor;
	float mTargetAccuracySpreadFactor;

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


#pragma once
#include "Core/Component.h"
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


class WeaponComponent : public Component
{
	friend class Weapon;

public:
	WeaponComponent(class Actor* owner, std::weak_ptr<class AnimationStateMachine> inASM);
	~WeaponComponent();

	void Update(float deltaTime) override;
	void ProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY) override;

	void Fire();
	void Reload();

	inline uint16 GetCurrentMagazineAmmo() const { return mCurrentMagazineAmmo; }
	inline uint16 GetCurrentReserveAmmo() const { return mCurrentReserveAmmo; }

	inline uint16 GetMaxMagazineCapacity() const { return mMaxMagazineCapacity; }
	inline uint16 GetMaxReserveCapacity() const { return mMaxReserveCapacity; }
	
private:
	void FinishedReload();

	bool TryFire(bool checkOnly = false);
	bool TryReload();

	Vec2 CalculateRecoilDeviation() const;

	std::weak_ptr<class AnimationStateMachine> mAnimStateMachine;

	EWeaponState mCurrentState;
	DAHandle mReloadAction;

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


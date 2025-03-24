#include "WeaponComponent.h"
#include "Animation/AnimationStateMachine.h"
#include "Core/Actor.h"
#include "Core/GameContext.h"
#include "Graphics/Renderer.h"
#include "Gameplay/Player.h"
#include "Gameplay/CameraComponent.h"
#include "Gameplay/Actions/ActionComponent.h"
#include "Gameplay/CameraComponent.h"
#include "Physics/PhysicsManager.h"
#include "Util/Random.h"
#include "Util/GameEvent.h"

#include <SDL2/SDL.h>

WeaponComponent::WeaponComponent(Actor* owner, AnimationStateMachine* inASM)
	: OldComponent(owner),
	mAnimStateMachine(inASM)
{
	mCurrentState = EWeaponState::READY;

	mIsSemiAuto = true;
	mFireRatePerMin = 300.0f;
	mFireRateCooldown = 60.0f / mFireRatePerMin;

	mFireRateTimer = -0.1f;

	mRecoilType = ERecoilType::RECOIL_DIAMOND;
	mRecoilDiamond = Vec2(0.1f, 0.2f);
	mDefaultAccuracySpreadFactor = 0.02f;
	mTargetAccuracySpreadFactor = mDefaultAccuracySpreadFactor;
	mAccuracySpreadFactor = mDefaultAccuracySpreadFactor;

	mHeatReduceDelayCooldown = 0.2f;
	mHeatReduceRatePerSec = 30.0f;

	mHeatReduceDelayTimer = -0.1f;
	mCurrentHeat = 0.0f;
	
	mMaxMagazineCapacity = 12;
	mMaxReserveCapacity = 5 * mMaxMagazineCapacity;
	mIsOpenBolt = false;

	mCurrentMagazineAmmo = mMaxMagazineCapacity;
	mCurrentReserveAmmo = mMaxReserveCapacity;
}

WeaponComponent::~WeaponComponent()
{
}

void WeaponComponent::Update(float deltaTime)
{
	if (mFireRateTimer > 0.0f)
		mFireRateTimer -= deltaTime;

	if (mHeatReduceDelayTimer > 0.0f)
		mHeatReduceDelayTimer -= deltaTime;
	else if (mCurrentHeat > 0.0f)
		mCurrentHeat -= mHeatReduceRatePerSec * deltaTime;

	// Smooth crosshair transition
	if (abs(mAccuracySpreadFactor - mTargetAccuracySpreadFactor) > EPSILON)
		mAccuracySpreadFactor = Math::Lerp(mAccuracySpreadFactor, mTargetAccuracySpreadFactor, 0.3f);
}

bool WeaponComponent::CanFire()
{
	//Prevent fire when reloading

	if (mFireRateTimer > 0.0f)
		return false;

	if (mCurrentMagazineAmmo <= 0)
		return false;

	if (mCurrentState != EWeaponState::READY)
		return false;

	return true;
}

void WeaponComponent::Fire()
{
	mCurrentHeat += 1.0f;
	mHeatReduceDelayTimer = mHeatReduceDelayCooldown;

	mCurrentMagazineAmmo -= 1;
	mFireRateTimer = mFireRateCooldown;
}

bool WeaponComponent::CanReload()
{
	if (mCurrentReserveAmmo <= 0)
		return false;

	int maxCapacity = mIsOpenBolt ? mMaxMagazineCapacity : mMaxMagazineCapacity + 1;
	if (mCurrentMagazineAmmo >= maxCapacity)
		return false;

	if (mCurrentState != EWeaponState::READY)
		return false;

	return true;
}

void WeaponComponent::FinishedReload()
{
	uint16 neededAmmo = mMaxMagazineCapacity - mCurrentMagazineAmmo;
	if (mCurrentMagazineAmmo > 0 && !mIsOpenBolt)
		neededAmmo += 1;

	uint16 reducedAmmo = neededAmmo <= mCurrentReserveAmmo ? neededAmmo : mCurrentReserveAmmo;

	mCurrentMagazineAmmo += reducedAmmo;
	mCurrentReserveAmmo -= reducedAmmo;

	printf("Current Ammo: %d/%d\n", mCurrentMagazineAmmo, mCurrentReserveAmmo);

	mCurrentState = EWeaponState::READY;
}

Vec2 WeaponComponent::CalculateRecoilDeviation() const
{
	if (mRecoilType == ERecoilType::ACCURACY_SPREAD)
	{
		float accSpreadVal = mAccuracySpreadFactor * mCurrentHeat;
		return Vector2(Random::Range(-accSpreadVal, accSpreadVal), Random::Range(-accSpreadVal, accSpreadVal));
	}
	else if (mRecoilType == ERecoilType::RECOIL_DIAMOND)
	{
		float recoilOnY = Random::Range(0.0f, mRecoilDiamond.mY);
		float ratio = 1.0f - (recoilOnY / mRecoilDiamond.mY);
		float recoilOnX = Random::Range(-mRecoilDiamond.mX, mRecoilDiamond.mX) * ratio;
		return Vector2(recoilOnX, recoilOnY);
	}
	else
	{
		return Vec2(0.0f, 0.0f);
	}
}


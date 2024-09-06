#include "Weapon.h"
#include "WeaponComponent.h"

#include "Util/Random.h"

#include <SDL2/SDL.h>

WeaponComponent::WeaponComponent(Actor* owner)
	: Component(owner)
{
	mWeapon = (Weapon*)owner;

	mReloadAnimDuration = 2.0f;

	mIsSemiAuto = true;
	mFireRatePerMin = 300.0f;
	mFireRateCooldown = 60.0f / mFireRatePerMin;

	mFireRateTimer = -0.1f;
	mIsLMBPressed = false;

	mRecoilType = ERecoilType::RECOIL_DIAMOND;
	mRecoilDiamond = Vec2(0.1f, 0.2f);
	mAccuracySpreadFactor = 0.05f;

	mHeatReduceDelayCooldown = 0.2f;
	mHeatReduceRatePerSec = 30.0f;

	mHeatReduceDelayTimer = -0.1f;
	mCurrentHeat = 0.0f;
	
	mMaxMagazineCapacity = 12;
	mMaxReserveCapacity = 5 * mMaxMagazineCapacity;
	mIsOpenBolt = true;

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
}


bool WeaponComponent::TryFire(bool checkOnly)
{
	if (mFireRateTimer > 0.0f)
		return false;

	if (mCurrentMagazineAmmo <= 0)
		return false;

	if (checkOnly)
		return true;

	mCurrentHeat += 1.0f;
	mHeatReduceDelayTimer = mHeatReduceDelayCooldown;

	mCurrentMagazineAmmo -= 1;
	mFireRateTimer = mFireRateCooldown;
	return true;
}

bool WeaponComponent::TryReload()
{
	if (mCurrentReserveAmmo <= 0)
		return false;

	int maxCapacity = mIsOpenBolt ? mMaxMagazineCapacity : mMaxMagazineCapacity + 1;
	if (mCurrentMagazineAmmo >= maxCapacity)
		return false;

	return true;
}

void WeaponComponent::ApplyReload()
{
	uint16 neededAmmo = mMaxMagazineCapacity - mCurrentMagazineAmmo;
	if (mCurrentMagazineAmmo > 0 && !mIsOpenBolt)
		neededAmmo += 1;

	uint16 reducedAmmo = neededAmmo <= mCurrentReserveAmmo ? neededAmmo : mCurrentReserveAmmo;

	mCurrentMagazineAmmo += reducedAmmo;
	mCurrentReserveAmmo -= reducedAmmo;

	printf("Current Ammo: %d/%d\n", mCurrentMagazineAmmo, mCurrentReserveAmmo);
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

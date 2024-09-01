#include "WeaponComponent.h"

#include "Util/Random.h"

WeaponComponent::WeaponComponent(Actor* owner)
{
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

void WeaponComponent::ProcessInput(const Uint8* keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
}

void WeaponComponent::Fire()
{
	if (mFireRateTimer > 0.0f)
		return;

	if (mCurrentMagazineAmmo <= 0)
		return;

	mCurrentHeat += 1.0f;
	mHeatReduceDelayTimer = mHeatReduceDelayCooldown;
	
	Vector2 recoilSpread;

	switch (mRecoilType)
	{
	case ERecoilType::ACCURACY_SPREAD:
		float accSpreadVal = mAccuracySpreadFactor * mCurrentHeat;
		recoilSpread = Vector2(Random::Range(-accSpreadVal, accSpreadVal), Random::Range(-accSpreadVal, accSpreadVal));
		break;
	case ERecoilType::RECOIL_DIAMOND:
		float recoilOnY = Random::Range(0.0f, mRecoilDiamond.mY);
		float ratio = 1.0f - (recoilOnY / mRecoilDiamond.mY);
		float recoilOnX = Random::Range(-mRecoilDiamond.mX, mRecoilDiamond.mX) * ratio;
		recoilSpread = Vector2(recoilOnX, recoilOnY);
		break;
	case ERecoilType::RECOIL_RIG:
		break;
	default:
		break;
	}
	
	



	//TODO: Hitscan detection

	mCurrentMagazineAmmo -= 1;
	mFireRateTimer = mFireRateCooldown;

	//TODO: Animation, sound, vfx...

}

void WeaponComponent::Reload()
{
	if (mCurrentReserveAmmo <= 0)
		return;

	if (mCurrentMagazineAmmo >= mMaxMagazineCapacity)
		return;

	int neededAmmo = mMaxMagazineCapacity - mCurrentMagazineAmmo;
	if (mCurrentMagazineAmmo > 0 && !mIsOpenBolt)
		neededAmmo += 1;

	int reducedAmmo = neededAmmo <= mCurrentReserveAmmo ? neededAmmo : mCurrentReserveAmmo;
	
	mCurrentMagazineAmmo += reducedAmmo;
	mCurrentReserveAmmo -= reducedAmmo;

	//TODO: Animation
}

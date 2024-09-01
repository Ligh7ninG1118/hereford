#include "WeaponComponent.h"
#include "Util/Random.h"

#include <SDL2/SDL.h>

WeaponComponent::WeaponComponent(Actor* owner)
	: Component(owner)
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
	if ((mouseState & SDL_BUTTON_LEFT))
	{
		if (!mIsLMBPressed)
		{
			Fire();
		}
	}
	else
	{
		mIsLMBPressed = false;
	}
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

	if (mRecoilType == ERecoilType::ACCURACY_SPREAD)
	{
		float accSpreadVal = mAccuracySpreadFactor * mCurrentHeat;
		recoilSpread = Vector2(Random::Range(-accSpreadVal, accSpreadVal), Random::Range(-accSpreadVal, accSpreadVal));
	}
	else if (mRecoilType == ERecoilType::RECOIL_DIAMOND)
	{
		float recoilOnY = Random::Range(0.0f, mRecoilDiamond.mY);
		float ratio = 1.0f - (recoilOnY / mRecoilDiamond.mY);
		float recoilOnX = Random::Range(-mRecoilDiamond.mX, mRecoilDiamond.mX) * ratio;
		recoilSpread = Vector2(recoilOnX, recoilOnY);
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

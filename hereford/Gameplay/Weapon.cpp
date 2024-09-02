#include "Weapon.h"
#include "Animation/Animator.h"
#include "WeaponComponent.h"
#include "SDL2/SDL.h"

#include "Util/GameEvent.h"

Weapon::Weapon(GameContext* gameCtx, Animator* animator)
	:Actor(gameCtx),
	mAnimator(animator)
{
	mWeaponComponent = new WeaponComponent(this);
}

Weapon::~Weapon()
{
}

void Weapon::OnUpdate(float deltaTime)
{
}

void Weapon::OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
	EMouseState targetFlag = mWeaponComponent->mIsSemiAuto ? LMB_DOWN : LMB_HOLD;
	if (mouseState & targetFlag)
		Fire();


	if (keyState[SDL_SCANCODE_R] == EInputState::KEY_DOWN)
	{
		Reload();
	}
}

void Weapon::Fire()
{
	if (mWeaponComponent->TryFire() && mCurrentState == EWeaponState::READY)
	{
		mAnimator->PlayAnimation(4, false, 0.2f);
		GameEvent::Publish<EventOnPlayerWeaponFired>(EventOnPlayerWeaponFired(mWeaponComponent->CalculateRecoilDeviation()));
	}
}

void Weapon::Reload()
{
	if (mWeaponComponent->TryReload() && mCurrentState == EWeaponState::READY)
	{
		mCurrentState = EWeaponState::RELOADING;
		mAnimator->PlayAnimation(3, false, mWeaponComponent->mReloadAnimDuration);
		DelayedActionManager::AddAction(mReloadAction, std::bind(&Weapon::FinishedReload, this), mWeaponComponent->mReloadAnimDuration, false);
	}
}

void Weapon::FinishedReload()
{
	mWeaponComponent->ApplyReload();
	mCurrentState = EWeaponState::READY;
}



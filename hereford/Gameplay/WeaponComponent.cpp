#include "WeaponComponent.h"
#include "Animation/AnimationStateMachine.h"
#include "Core/Actor.h"
#include "Core/GameContext.h"
#include "Graphics/Renderer.h"
#include "Gameplay/Player.h"
#include "Gameplay/CameraComponent.h"
#include "Physics/PhysicsManager.h"
#include "Util/Random.h"
#include "Util/GameEvent.h"

#include <SDL2/SDL.h>

WeaponComponent::WeaponComponent(Actor* owner, std::weak_ptr<AnimationStateMachine> inASM)
	: Component(owner),
	mAnimStateMachine(inASM)
{
	mCurrentState = EWeaponState::READY;

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

void WeaponComponent::ProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
	EMouseState targetFlag = mIsSemiAuto ? LMB_DOWN : LMB_HOLD;
	if (mouseState & targetFlag)
		Fire();

	if (keyState[SDL_SCANCODE_R] == EInputState::KEY_DOWN)
	{
		Reload();
	}
}

void WeaponComponent::Fire()
{
	if (TryFire() && mCurrentState == EWeaponState::READY)
	{
		auto lock = mAnimStateMachine.lock();
		if(lock)
			lock->PlayAnimation(4, false, 0.2f);

		Player* player = static_cast<Player*>(GetOwner());
		const CameraComponent& cam = player->GetMainCamera();

		Vec3 origin = cam.GetCameraPosition();
		Vec3 dir = cam.GetFrontVector().normalized();
		HitInfo hitInfo;
		if (player->GetGame()->GetPhysicsManager().RaycastQuery(origin, dir, 1000.0f, hitInfo))
		{
			if (hitInfo.hitActor != nullptr)
				hitInfo.hitActor->SetState(ActorState::Destroy);
		}

		GetOwner()->GetGame()->GetRenderer().AddDebugLines(origin, origin + dir * 50.0f);

		GameEvent::Publish<EventOnPlayerWeaponFired>(EventOnPlayerWeaponFired(CalculateRecoilDeviation()));
	}
}

void WeaponComponent::Reload()
{
	if (TryReload() && mCurrentState == EWeaponState::READY)
	{
		mCurrentState = EWeaponState::RELOADING;
		auto lock = mAnimStateMachine.lock();
		if (lock)
			lock->PlayAnimation(3, false, mReloadAnimDuration);
		DelayedActionManager::AddAction(mReloadAction, std::bind(&WeaponComponent::FinishedReload, this), mReloadAnimDuration, false);
	}
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

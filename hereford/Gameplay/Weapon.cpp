#include "Weapon.h"
#include "Gameplay/WeaponComponent.h"
#include "Gameplay/CameraComponent.h"
#include "Gameplay/Actions/ActionComponent.h"
#include "Gameplay/Player.h"
#include "Gameplay/IHittable.h"
#include "Core/GameContext.h"
#include "Animation/AnimationStateMachine.h"
#include "Graphics/AnimatedRenderComponent.h"

#include "Audio/AudioComponent.h"
#include <SDL2/SDL.h>
#include <functional>


Weapon::Weapon(GameContext* gameCtx)
	: Actor(gameCtx), mPtrPlayer(nullptr)
{
	
}

Weapon::~Weapon()
{
}

void Weapon::Init(Player* playerPtr)
{
	mPtrPlayer = playerPtr;
}

void Weapon::OnUpdate(float deltaTime)
{
}

void Weapon::OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
	EMouseState targetFlag = mPtrWeaponComp->mIsSemiAuto ? LMB_DOWN : LMB_HOLD;
	if (mouseState & targetFlag)
	{
		mPtrWeaponComp->Fire();
	}

	if (mouseState & RMB_DOWN)
	{
		TimelineActionManager::PlayFromStart(mHAimingTimeline, std::bind(&Weapon::AimingTimeline, this, std::placeholders::_1), 0.2f);
		mPtrPlayer->GetMainCamera().mAimingSensMultiplier = 0.85f;
	}
	else if (mouseState & RMB_UP)
	{
		TimelineActionManager::ReverseFromEnd(mHAimingTimeline, std::bind(&Weapon::AimingTimeline, this, std::placeholders::_1), 0.2f);
		mPtrPlayer->GetMainCamera().mAimingSensMultiplier = 1.0f;
	}

	if (keyState[SDL_SCANCODE_R] == EInputState::KEY_DOWN)
	{
		if (mPtrPlayer->GetActionComp()->StartActionByName("Reload"))
		{
			Reload();
		}
	}
}

void Weapon::Fire()
{
	if (mPtrWeaponComp->CanFire())
	{
		mPtrWeaponComp->Fire();
		mPtrAnimStateMachine->PlayAnimation(4, false, 0.2f);
		
		CameraComponent& cam = mPtrPlayer->GetMainCamera();

		Vec3 origin = cam.GetCameraPosition();
		Vec3 dir = cam.GetFrontVector().normalized();

		/*dir.mX += Random::Range(-mAccuracySpreadFactor, mAccuracySpreadFactor);
		dir.mZ += Random::Range(-mAccuracySpreadFactor, mAccuracySpreadFactor);

		dir.Normalize();*/


		HitInfo hitInfo;
		GetGameContext()->GetPhysicsManager().RaycastQuery(origin, dir, 1000.0f, hitInfo);

		if (hitInfo.hitActor != nullptr)
			if (auto hittable = dynamic_cast<IHittable*>(hitInfo.hitActor); hittable != nullptr)
			{
				//TODO: make this a var
				hitInfo.hitPower = 1.0f;
				hittable->Hit(hitInfo);
			}

		cam.RotateCamera(mPtrWeaponComp->CalculateRecoilDeviation() * 5.0f);

		mPtrAudioComponent->Play();
	}
}

void Weapon::Reload()
{
	if (mPtrWeaponComp->CanReload())
	{
		mPtrAnimStateMachine->PlayAnimation(3, false, mPtrWeaponComp->mReloadAnimDuration);
		DelayedActionManager::AddAction(mHReloadCallback, std::bind(&Weapon::FinishedReload, this), mPtrWeaponComp->mReloadAnimDuration, false);
	}
}

void Weapon::AimingTimeline(float alpha)
{
	mPtrPlayer->GetMainCamera().SetHorFOV(Math::Lerp(80.0f, 50.0f, alpha));
	//TODO: parameterize this
	mPtrAnimRenderComp->SetTranslateOffset(Math::Lerp(Vec3(-0.2f, -0.4f, 0.0f), Vec3(-0.5f, -0.275f, 0.215f), alpha));
}

void Weapon::FinishedReload()
{
	mPtrPlayer->GetActionComp()->StopActionByName("Reload");
}

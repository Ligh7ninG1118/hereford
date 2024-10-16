#include "Weapon.h"
#include "Gameplay/WeaponComponent.h"
#include "Gameplay/CameraComponent.h"
#include "Gameplay/Actions/ActionComponent.h"
#include "Gameplay/Player.h"
#include "Gameplay/IHittable.h"
#include "Core/GameContext.h"
#include "Animation/AnimationStateMachine.h"
#include "Graphics/AnimatedRenderComponent.h"
#include "imgui/imgui.h"

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
	SetPosition(mPtrPlayer->GetPosition());
	SetRotation(mPtrPlayer->GetRotation());

	//TODO 
	/*if (mPtrActionComp->GetActiveGameplayTags().HasTag(GameplayTag(EActionType::CROUCHING)))
			mPtrActiveWeaponComp->SetAccuracySpreadMultiplier(1.125f);
		else
			mPtrActiveWeaponComp->SetAccuracySpreadMultiplier(1.5f);*/
	/*if (mPtrActionComp->GetActiveGameplayTags().HasTag(GameplayTag(EActionType::CROUCHING)))
			mPtrActiveWeaponComp->SetAccuracySpreadMultiplier(0.75f);
		else
			mPtrActiveWeaponComp->SetAccuracySpreadMultiplier(1.0f);*/

	/*static float scaleOffset = 0.01f;

	ImGui::Begin("Arm Offset", 0, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat("X Offset", &currentArmTranslationOffset.mX, -10.0f, 10.0f);
	ImGui::SliderFloat("Y Offset", &currentArmTranslationOffset.mY, -10.0f, 10.0f);
	ImGui::SliderFloat("Z Offset", &currentArmTranslationOffset.mZ, -10.0f, 10.0f);
	ImGui::SliderFloat("RX Offset", &currentArmRotationOffset.mX, -360.0f, 360.0f);
	ImGui::SliderFloat("RY Offset", &currentArmRotationOffset.mY, -360.0f, 360.0f);
	ImGui::SliderFloat("RZ Offset", &currentArmRotationOffset.mZ, -360.0f, 360.0f);
	ImGui::SliderFloat("Scale Offset", &scaleOffset, -5.0f, 5.0f);


	ImGui::End();


	mPtrAnimRenderComp->SetTranslateOffset(currentArmTranslationOffset);
	mPtrAnimRenderComp->SetRotateOffset(currentArmRotationOffset);
	mPtrAnimRenderComp->SetScaleOffset(Vec3(scaleOffset));*/
}

void Weapon::OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
	EMouseState targetFlag = mPtrWeaponComp->mIsSemiAuto ? LMB_DOWN : LMB_HOLD;
	if (mouseState & targetFlag)
	{
		Fire();
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

	{
		mCurrentArmRotationOffset.mX -= mouseDeltaY * 0.1f;
		mCurrentArmRotationOffset.mZ += mouseDeltaX * 0.1f;
	}
}

void Weapon::SetArmOffset(Vec3 translationOffset)
{
	Vec3 newTransOffset = mCurrentArmTranslationOffset + translationOffset;
	mPtrAnimRenderComp->SetTranslateOffset(newTransOffset);

	mCurrentArmRotationOffset.mX = Math::Lerp(mCurrentArmRotationOffset.mX, mHipArmRotationOffset.mX, 0.2f);
	mCurrentArmRotationOffset.mZ = Math::Lerp(mCurrentArmRotationOffset.mZ, mHipArmRotationOffset.mZ, 0.2f);
	mPtrAnimRenderComp->SetRotateOffset(mCurrentArmRotationOffset);
}

void Weapon::Fire()
{
	if (mPtrWeaponComp->CanFire())
	{
		mPtrWeaponComp->Fire();
		mPtrAnimStateMachine->PlayAnimation(mFireAnimIndex, false, 0.2f);
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
		mPtrAnimStateMachine->PlayAnimation(mReloadAnimIndex, false, mPtrWeaponComp->mReloadAnimDuration);
		DelayedActionManager::AddAction(mHReloadCallback, std::bind(&Weapon::FinishedReload, this), mPtrWeaponComp->mReloadAnimDuration, false);
	}
}

void Weapon::AimingTimeline(float alpha)
{
	mPtrPlayer->GetMainCamera().SetHorFOV(Math::Lerp(80.0f, 50.0f, alpha));
	mCurrentArmTranslationOffset = Math::Lerp(mHipArmTranslationOffset, mADSArmTranslationOffset, alpha);
	mPtrAnimRenderComp->SetTranslateOffset(mCurrentArmTranslationOffset);
}

void Weapon::FinishedReload()
{
	mPtrWeaponComp->FinishedReload();
	mPtrPlayer->GetActionComp()->StopActionByName("Reload");
}

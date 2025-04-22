#include "Gameplay/Player.h"
#include "Gameplay/CameraComponent.h"
#include "Gameplay/Weapon.h"
#include "Gameplay/WeaponComponent.h"
#include "Gameplay/IInteractable.h"
#include "Graphics/Renderer.h"
#include "Core/GameContext.h"
#include <SDL2/SDL.h>
#include "imgui/imgui.h"
#include "Physics/PhysicsManager.h"
#include "Audio/AudioComponent.h"
#include "Actions/ActionComponent.h"
#include "Asset/AssetManager.h"
#include "Actions/Action.h"
#include "Util/GameplayTagContainer.h"
#include "Input/InputManager.h"
#include "Gameplay/WeaponPistol.h"
#include "Gameplay/WeaponSMG.h"

#include "UI/UIAmmoIndicator.h"
#include "UI/UICrosshair.h"

#include "stdio.h"

Player::Player(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	mPtrCameraComp = std::make_unique<CameraComponent>(static_cast<Actor*>(this));

	mWeaponList.push_back(new WeaponSMG(gameCtx));
	mWeaponList.push_back(new WeaponPistol(gameCtx));

	for (auto& w : mWeaponList)
	{
		w->Init(this);
		w->SetState(EActorState::Disabled, true);
	}

	mCurrentWeaponIndex = 0;
	mPtrActiveWeapon = mWeaponList[mCurrentWeaponIndex];
	mPtrActiveWeapon->SetState(EActorState::Enabled, true);

	std::shared_ptr<Texture> ammoTex = AssetManager::LoadAsset<Texture>(std::string("LocalResources/rifle-round-silhouette.png"));
	std::shared_ptr<Shader> ammoShader = AssetManager::LoadAsset<Shader>(std::string("Shaders/ui_image_ammo_count_vert.glsl*Shaders/ui_image_ammo_count_frag.glsl"));

	Renderer* renderer = &mGameCtx->GetRenderer();
	Vec2 screenDimension = renderer->GetScreenDimension();

	//TODO: Separate HUD Actor class to handle all HUD elements?
	mPtrUIAmmo = new UIAmmoIndicator(renderer, ammoShader.get(), ammoTex);
	Mat4 uiProj = mPtrCameraComp->GetOrthoMatrix(0.0f, screenDimension.mX, 0.0f, screenDimension.mY);
	mPtrUIAmmo->Initialize(mPtrActiveWeapon->GetWeaponComponent());
	mPtrUIAmmo->SetUIProjection(uiProj);

	std::shared_ptr<Shader> crosshairShader = AssetManager::LoadAsset<Shader>(std::string("Shaders/ui_crosshair_vert.glsl*Shaders/ui_crosshair_frag.glsl"));

	mPtrUICrosshair = new UICrosshair(renderer, crosshairShader.get());
	mPtrUICrosshair->Initialize(mPtrActiveWeapon->GetWeaponComponent());

	mPtrActionComp = std::make_unique<ActionComponent>(this);

	Action reloadAction("Reload");
	reloadAction.AddGrantsTag(GameplayTag(EActionType::RELOADING));
	
	Action crouchAction("Crouch");
	crouchAction.AddGrantsTag(GameplayTag(EActionType::CROUCHING));
	crouchAction.AddBlockeTag(GameplayTag(EActionType::SPRINTING));

	Action sprintAction("Sprint");
	sprintAction.AddGrantsTag(GameplayTag(EActionType::SPRINTING));
	sprintAction.AddBlockeTag(GameplayTag(EActionType::CROUCHING));

	mPtrActionComp->AddAction(reloadAction);
	mPtrActionComp->AddAction(crouchAction);
	mPtrActionComp->AddAction(sprintAction);

	totalRuntime = 0.0f;
	currentTopSpeed = topWalkingSpeed;

	mPtrAudioComponent = std::make_unique<AudioComponent>(this, gameCtx->GetAudioManager());
	mPtrAudioComponent->InitAsset("Walk-Gravel.wav");

	mIsGrounded = true;

	mInputMgr = &gameCtx->GetInputManager();
	hCrouchSub = mInputMgr->Subscribe(EInputAction::PLAYER_CROUCH, std::bind(&Player::OnCrouchInput, this, std::placeholders::_1), EInputState::PRESSED);
	hJumpSub = mInputMgr->Subscribe(EInputAction::PLAYER_JUMP, std::bind(&Player::OnJumpInput, this, std::placeholders::_1), EInputState::PRESSED);
	hSprintSub = mInputMgr->Subscribe(EInputAction::PLAYER_SPRINT, std::bind(&Player::OnSprintInput, this, std::placeholders::_1));
	hInteractSub = mInputMgr->Subscribe(EInputAction::PLAYER_INTERACT, std::bind(&Player::OnInteractInput, this, std::placeholders::_1), EInputState::PRESSED);
	hWeaponSwapUpSub = mInputMgr->Subscribe(EInputAction::WEAPON_SWAP_UP, std::bind(&Player::OnWeaponSwapInput, this, std::placeholders::_1), EInputState::PRESSED);
	hWeaponSwapDownSub = mInputMgr->Subscribe(EInputAction::WEAPON_SWAP_DOWN, std::bind(&Player::OnWeaponSwapInput, this, std::placeholders::_1), EInputState::PRESSED);
}

Player::~Player()
{
	mInputMgr->Unsubscribe(EInputAction::PLAYER_CROUCH, hCrouchSub);
	mInputMgr->Unsubscribe(EInputAction::PLAYER_JUMP, hJumpSub);
	mInputMgr->Unsubscribe(EInputAction::PLAYER_SPRINT, hSprintSub);
	mInputMgr->Unsubscribe(EInputAction::PLAYER_INTERACT, hInteractSub);
	mInputMgr->Unsubscribe(EInputAction::WEAPON_SWAP_UP, hWeaponSwapUpSub);
	mInputMgr->Unsubscribe(EInputAction::WEAPON_SWAP_DOWN, hWeaponSwapDownSub);

}

void Player::OnUpdate(float deltaTime)
{
	ProcessMovement(deltaTime);
	ProcessInteractionPrompt();
	totalRuntime += deltaTime;

	float speed = currentVelocity.Magnitude();

	float yD = sinf(totalRuntime * 6.0f) * speed * 0.001f;
	float zD = sinf(totalRuntime * 3.0f) * speed * 0.001f;

	mPtrActiveWeapon->SetArmOffset(Vec3(0.0f, yD, zD));

	if ((currentVelocity.SqrMagnitude() > EPISILON) && mIsGrounded)
	{
		if (mPtrAudioComponent->GetSoundState() == ESoundState::Paused)
		{
			mPtrAudioComponent->Resume();
		}
		else if (mPtrAudioComponent->GetSoundState() == ESoundState::Stopped)
		{
			mPtrAudioComponent->Play(true);
		}
	}
	else
	{
		if (mPtrAudioComponent->GetSoundState() == ESoundState::Playing)
		{
			mPtrAudioComponent->Pause();
		}
	}

	//ShowDebugInfo();
}


void Player::ProcessMovement(float deltaTime)
{
	Vec2 rawInputDir = mInputMgr->ReadValue<Vec2>(EInputAction::PLAYER_MOVEMENT);

	float inputSpeedModifier = rawInputDir.Magnitude();
	inputSpeedModifier = Math::Clamp(inputSpeedModifier, 0.0f, 1.0f);

	rawInputDir.Normalize();

	Vec3 moveDir = rawInputDir.mX * GetForward()
		+ Vec3::Up
		+ rawInputDir.mY * (GetForward().Cross(Vec3::Up));

	//TODO: reduce control while in air
	float targetSpeed = (rawInputDir.SqrMagnitude() >= EPISILON) ? currentTopSpeed : 0.0f;
	float currentHorSpeed = Vector3(currentVelocity.mX, 0.0f, currentVelocity.mZ).Magnitude();
	float currentVerticalSpeed = currentVelocity.mY;

	if (currentHorSpeed < targetSpeed - speedOffset || currentHorSpeed > targetSpeed + speedOffset)
		currentVelocity = Math::Lerp(currentHorSpeed, targetSpeed, deltaTime * maxSpeedChangingRate) * moveDir;
	else
		currentVelocity = targetSpeed * moveDir;

	if(!mIsGrounded)
		currentVerticalSpeed += OVERRIDE_GRAVITY_CONSTANT * deltaTime;

	currentVelocity.mY = currentVerticalSpeed;

	currentVelocity *= inputSpeedModifier;

	Vector3 updatedPos = GetPosition();
	updatedPos += currentVelocity * deltaTime;

	if (updatedPos.mY <= 0.0f)
	{
		currentVelocity.mY = 0.0f;
		updatedPos.mY = 0.0f;
		mIsGrounded = true;
	}

	SetPosition(updatedPos);
}

void Player::OnJumpInput(EInputState state)
{
	if (mIsGrounded)
	{
		currentVelocity.mY = std::sqrtf(JUMP_HEIGHT_CONSTANT * -2.0f * OVERRIDE_GRAVITY_CONSTANT);
		mIsGrounded = false;
	}
}

void Player::OnCrouchInput(EInputState state)
{
	//TODO: Block when already crouching (or add the buffer system)
			//TODO: Creating additional copy here. Overload enum version
	if (mPtrActionComp->GetActiveGameplayTags().HasTag(GameplayTag(EActionType::CROUCHING)))
	{
		mPtrActionComp->StopActionByName("Crouch");
		TimelineActionManager::ReverseFromEnd(mHCrouchTimeline, std::bind(&Player::CrouchTimeline, this, std::placeholders::_1), 0.25f);
		currentTopSpeed = topWalkingSpeed;
	}
	else
	{
		mPtrActionComp->StartActionByName("Crouch");
		TimelineActionManager::PlayFromStart(mHCrouchTimeline, std::bind(&Player::CrouchTimeline, this, std::placeholders::_1), 0.25f);
		currentTopSpeed = topCrouchSpeed;
	}
}

void Player::OnSprintInput(EInputState state)
{
	switch (state)
	{
	case EInputState::PRESSED:
		mPtrActionComp->StartActionByName("Sprint");
		currentTopSpeed = topSprintingSpeed;
		break;
	case EInputState::RELEASED:
		mPtrActionComp->StopActionByName("Sprint");
		currentTopSpeed = topWalkingSpeed;
		break;
	default:
		break;
	}
}

void Player::OnWeaponSwapInput(EInputState state)
{
	//TODO: Check nullptr
	//TODO: Handle double swapping
	mPtrActiveWeapon->Holster();
	DelayedActionManager::AddAction(mHWeaponSwitch, std::bind(&Player::WeaponSwitchCallback, this), mPtrActiveWeapon->mHolsterTime, false);
}

void Player::ProcessInteractionPrompt()
{
	Vec3 origin = mPtrCameraComp->GetCameraPosition();
	Vec3 dir = mPtrCameraComp->GetFrontVector().normalized();

	static float interactRadius = 0.5f;
	static float interactDis = 0.5f;

	HitInfo hitInfo;
	mInteractCandidate = nullptr;

	if (GetGameContext()->GetPhysicsManager().SpherecastQuery(origin, dir, 2.0f, 0.5f, hitInfo))
	{
		if (hitInfo.hitActor != nullptr)
			if (auto interactable = dynamic_cast<IInteractable*>(hitInfo.hitActor); interactable != nullptr)
			{
				mInteractCandidate = interactable;
			}
	}
}

void Player::OnInteractInput(EInputState state)
{
	if (mInteractCandidate != nullptr)
	{
		mInteractCandidate->Interact();
	}
}

void Player::ShowDebugInfo()
{
	ImGui::Begin("Player");
	Vec3 pos = GetPosition();
	Vec3 pRot = GetRotation();

	Vec3 rot = mPtrCameraComp->GetRotation();
	Vec3 frt = mPtrCameraComp->GetFrontVector();
	ImGui::Text("Pos (%.2f, %.2f, %.2f)\tRot (%.2f, %.2f, %.2f)", pos.mX, pos.mY, pos.mZ, rot.mX, rot.mY, rot.mZ);
	ImGui::Text("Cam Front (%.2f, %.2f, %.2f)", frt.mX, frt.mY, frt.mZ);
	ImGui::Text("Player Rot (%.2f, %.2f, %.2f)", pRot.mX, pRot.mY, pRot.mZ);
	ImGui::End();
}

void Player::WeaponSwitchCallback()
{
	mPtrActiveWeapon->SetState(EActorState::Disabled, true);
	mCurrentWeaponIndex++;
	mCurrentWeaponIndex %= mWeaponList.size();
	
	mPtrActiveWeapon = mWeaponList[mCurrentWeaponIndex];
	mPtrActiveWeapon->SetState(EActorState::Enabled, true);
	mPtrActiveWeapon->Draw();

	mPtrUIAmmo->Initialize(mPtrActiveWeapon->GetWeaponComponent());
	mPtrUICrosshair->Initialize(mPtrActiveWeapon->GetWeaponComponent());
}

void Player::CrouchTimeline(float alpha)
{
	float heightVal = Math::Lerp(1.8f, 0.9f, alpha);
	float armOffsetVal = Math::Lerp(1.32f, 0.56f, alpha);

	mPtrCameraComp->SetEyeHeight(heightVal);
}

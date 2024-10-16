#include "Gameplay/Player.h"
#include "Gameplay/CameraComponent.h"
#include "Gameplay/Weapon.h"
#include "Gameplay/WeaponComponent.h"

#include "Gameplay/IInteractable.h"
#include "Core/GameContext.h"
#include <SDL2/SDL.h>
#include "imgui/imgui.h"

#include "Audio/AudioComponent.h"
#include "Actions/ActionComponent.h"
#include "Asset/AssetManager.h"
#include "Actions/Action.h"
#include "Util/GameplayTagContainer.h"

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
}

Player::~Player()
{
	//printf("Player Destructor\n");
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

	if (hasMovementInput && mIsGrounded)
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

	ShowDebugInfo();
}

void Player::OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
	// Horizontal Movement
	{
		inputMoveDir = Vector3::Zero;
		hasMovementInput = false;

		if (keyState[SDL_SCANCODE_W] == EInputState::KEY_HOLD)
		{
			inputMoveDir += GetForward();
			hasMovementInput = true;
		}
		if (keyState[SDL_SCANCODE_S] == EInputState::KEY_HOLD)
		{
			inputMoveDir -= GetForward();
			hasMovementInput = true;
		}
		if (keyState[SDL_SCANCODE_A] == EInputState::KEY_HOLD)
		{
			// Cross with World Up to get Right vector;
			inputMoveDir -= GetForward().Cross(Vector3(0.0f, 1.0f, 0.0f));
			hasMovementInput = true;
		}
		if (keyState[SDL_SCANCODE_D] == EInputState::KEY_HOLD)
		{
			inputMoveDir += GetForward().Cross(Vector3(0.0f, 1.0f, 0.0f));
			hasMovementInput = true;
		}
		if (keyState[SDL_SCANCODE_Q] == EInputState::KEY_HOLD)
		{
			inputMoveDir -= Vector3(0.0f, 1.0f, 0.0f);
			hasMovementInput = true;
		}
		if (keyState[SDL_SCANCODE_E] == EInputState::KEY_HOLD)
		{
			inputMoveDir += Vector3(0.0f, 1.0f, 0.0f);
			hasMovementInput = true;
		}

		//inputMoveDir.mY = 0.0f;
		inputMoveDir.Normalize();
	}

	// Movement States (Crouching, Sprinting, etc.)
	{
		if (keyState[SDL_SCANCODE_C] == EInputState::KEY_DOWN)
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

		if (keyState[SDL_SCANCODE_LSHIFT] == EInputState::KEY_DOWN)
		{
			mPtrActionComp->StartActionByName("Sprint");
			currentTopSpeed = topSprintingSpeed;
		}
		if (keyState[SDL_SCANCODE_LSHIFT] == EInputState::KEY_UP)
		{
			mPtrActionComp->StopActionByName("Sprint");
			currentTopSpeed = topWalkingSpeed;
		}

		if (keyState[SDL_SCANCODE_SPACE] == EInputState::KEY_DOWN)
		{
			Jump();
		}
	}

	// Object Interaction
	{
		if (keyState[SDL_SCANCODE_F] == EInputState::KEY_DOWN)
		{
			Interaction();
		}
	}

	// Weapon & Gadget
	{
		if (keyState[SDL_SCANCODE_1] == EInputState::KEY_DOWN ||
			keyState[SDL_SCANCODE_2] == EInputState::KEY_DOWN ||
			mouseState & EMouseState::SCROLL_UP ||
			mouseState & EMouseState::SCROLL_DOWN)
		{
			//TODO: Handle double swapping
			mPtrActiveWeapon->Holster();
			DelayedActionManager::AddAction(mHWeaponSwitch, std::bind(&Player::WeaponSwitchCallback, this), mPtrActiveWeapon->mHolsterTime, false);
		}
	}
}


void Player::ProcessMovement(float deltaTime)
{
	//TODO: reduce control while in air
    float targetSpeed = hasMovementInput ? currentTopSpeed : 0.0f;
	float currentHorSpeed = Vector3(currentVelocity.mX, 0.0f, currentVelocity.mZ).Magnitude();
	float currentVerticalSpeed = currentVelocity.mY;

	if (currentHorSpeed < targetSpeed - speedOffset || currentHorSpeed > targetSpeed + speedOffset)
		currentVelocity = Math::Lerp(currentHorSpeed, targetSpeed, deltaTime * maxSpeedChangingRate) * inputMoveDir;
	else
		currentVelocity = targetSpeed * inputMoveDir;

	if(!mIsGrounded)
		currentVerticalSpeed += OVERRIDE_GRAVITY_CONSTANT * deltaTime;

	currentVelocity.mY = currentVerticalSpeed;

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

void Player::Jump()
{
	if (mIsGrounded)
	{
		currentVelocity.mY = std::sqrtf(JUMP_HEIGHT_CONSTANT * -2.0f * OVERRIDE_GRAVITY_CONSTANT);
		mIsGrounded = false;
	}
}

void Player::ProcessInteractionPrompt()
{
	Vec3 origin = mPtrCameraComp->GetCameraPosition();
	Vec3 dir = mPtrCameraComp->GetFrontVector().normalized();

	static float interactRadius = 0.5f;
	static float interactDis = 0.5f;

	ImGui::Begin("Interaction", 0, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat("Radius", &interactRadius, 0.0f, 2.0f);
	ImGui::SliderFloat("Distance", &interactDis, 0.0f, 2.0f);

	HitInfo hitInfo;
	mInteractCandidate = nullptr;

	if (GetGameContext()->GetPhysicsManager().SpherecastQuery(origin, dir, 2.0f, 0.5f, hitInfo))
	{
		if (hitInfo.hitActor != nullptr)
			if (auto interactable = dynamic_cast<IInteractable*>(hitInfo.hitActor); interactable != nullptr)
			{
				mInteractCandidate = interactable;
				ImGui::Text("Result: %s", interactable->GetInteractPrompt().c_str());
			}
	}
	else
	{
		ImGui::Text("Result: No");
	}
	ImGui::End();
}

void Player::Interaction()
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

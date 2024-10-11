#include "Gameplay/Player.h"
#include "Gameplay/CameraComponent.h"
#include "Gameplay/WeaponComponent.h"
#include "Core/GameContext.h"
#include <SDL2/SDL.h>
#include "imgui/imgui.h"

#include "Asset/AssetManager.h"
#include "Audio/AudioComponent.h"
#include "Animation/Animation.h"
#include "Animation/Animator.h"
#include "Animation/AnimationStateMachine.h"
#include "Actions/ActionComponent.h"
#include "Actions/Action.h"
#include "Util/GameplayTagContainer.h"

#include "Graphics/AnimatedRenderComponent.h"

#include "UI/UIAmmoIndicator.h"
#include "UI/UICrosshair.h"

#include "stdio.h"

Player::Player(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	mPtrCameraComp = std::make_unique<CameraComponent>(static_cast<Actor*>(this));

	
	mPtrAnimRenderComp = std::make_unique<AnimatedRenderComponent>(this, mGameCtx->GetRenderer(), ERenderLayer::Weapon);
	mPtrAnimRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/mark23/source/Mark23v3.fbx")));

	mPtrAnimRenderComp->SetShader(AssetManager::LoadAsset<Shader>(std::string("Shaders/model_tex_pbr_vert.glsl*Shaders/model_tex_phong_frag.glsl")));
	currentArmTranslationOffset = hipArmTranslationOffset;
	currentArmRotationOffset = hipArmRotationOffset;
	mPtrAnimRenderComp->SetTranslateOffset(currentArmTranslationOffset);
	mPtrAnimRenderComp->SetScaleOffset(Vec3(0.02f));
	mPtrAnimRenderComp->SetRotateOffset(currentArmRotationOffset);

	mPtrAnimRenderComp->SetCamera(mPtrCameraComp.get());

	std::unique_ptr<Animator> animator = std::make_unique<Animator>(
		Animator(Animation::LoadAnimations("LocalResources/mark23/source/Mark23v3.fbx", mPtrAnimRenderComp->GetModel())));

	// 0: Draw, 1: Hide, 2: Static, 3: Reload, 4: Fire
	// Construct shared ptr in place to avoid copying unique ptr inside ASM class
	mPtrAnimStateMachine = std::shared_ptr<AnimationStateMachine>(new AnimationStateMachine(this, std::move(animator)));
	mPtrAnimStateMachine->AddTransitionRule(0, AnimState(2, false));
	mPtrAnimStateMachine->AddTransitionRule(3, AnimState(2, false));
	mPtrAnimStateMachine->AddTransitionRule(4, AnimState(2, false));

	mPtrAnimRenderComp->SetAnimator(mPtrAnimStateMachine->GetAnimator());

	mPtrActiveWeaponComp = new WeaponComponent(static_cast<Actor*>(this), mPtrAnimStateMachine);
	mPtrWeaponFiredEvent = GameEvent::Subscribe<EventOnPlayerWeaponFired>(std::bind(&Player::WeaponFiredEventListener, this, std::placeholders::_1));

	std::shared_ptr<Texture> ammoTex = AssetManager::LoadAsset<Texture>(std::string("LocalResources/rifle-round-silhouette.png"));
	std::shared_ptr<Shader> ammoShader = AssetManager::LoadAsset<Shader>(std::string("Shaders/ui_image_ammo_count_vert.glsl*Shaders/ui_image_ammo_count_frag.glsl"));

	Renderer* renderer = &mGameCtx->GetRenderer();
	Vec2 screenDimension = renderer->GetScreenDimension();

	//TODO: Separate HUD Actor class to handle all HUD elements?
	mPtrUIAmmo = new UIAmmoIndicator(renderer, ammoShader.get(), ammoTex, mPtrActiveWeaponComp);
	Mat4 uiProj = mPtrCameraComp->GetOrthoMatrix(0.0f, screenDimension.mX, 0.0f, screenDimension.mY);
	mPtrUIAmmo->Initialize();
	mPtrUIAmmo->SetUIProjection(uiProj);

	std::shared_ptr<Shader> crosshairShader = AssetManager::LoadAsset<Shader>(std::string("Shaders/ui_crosshair_vert.glsl*Shaders/ui_crosshair_frag.glsl"));

	mPtrUICrosshair = new UICrosshair(renderer, crosshairShader.get(), mPtrActiveWeaponComp);
	mPtrUICrosshair->Initialize();

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
	mPtrAudioComponent->InitAsset("USP_SingleFire.wav");

	mPtrAudioComponent2 = std::make_unique<AudioComponent>(this, gameCtx->GetAudioManager());
	mPtrAudioComponent2->InitAsset("Walk-Gravel.wav");

}

Player::~Player()
{
	//printf("Player Destructor\n");
}

void Player::OnUpdate(float deltaTime)
{
	ProcessMovement(deltaTime);
	totalRuntime += deltaTime;

	float speed = currentVelocity.Magnitude();

	float yD = sinf(totalRuntime * 6.0f) * speed * 0.001f;
	float zD = sinf(totalRuntime * 3.0f) * speed * 0.001f;

	Vec3 offset = currentArmTranslationOffset;
	offset.mY += yD;
	offset.mZ += zD;
	mPtrAnimRenderComp->SetTranslateOffset(offset);

	if (hasMovementInput)
	{
		if (mPtrActionComp->GetActiveGameplayTags().HasTag(GameplayTag(EActionType::CROUCHING)))
			mPtrActiveWeaponComp->SetAccuracySpreadMultiplier(1.125f);
		else
			mPtrActiveWeaponComp->SetAccuracySpreadMultiplier(1.5f);

		if (mPtrAudioComponent2->GetSoundState() == ESoundState::Paused)
		{
			mPtrAudioComponent2->Resume();
		}
		else if(mPtrAudioComponent2->GetSoundState() == ESoundState::Stopped)
		{
			mPtrAudioComponent2->Play(true);
		}
	}
	else
	{
		if (mPtrActionComp->GetActiveGameplayTags().HasTag(GameplayTag(EActionType::CROUCHING)))
			mPtrActiveWeaponComp->SetAccuracySpreadMultiplier(0.75f);
		else
			mPtrActiveWeaponComp->SetAccuracySpreadMultiplier(1.0f);

		if (mPtrAudioComponent2->GetSoundState() == ESoundState::Playing)
		{
			mPtrAudioComponent2->Pause();
		}
	}

	currentArmRotationOffset.mX = Math::Lerp(currentArmRotationOffset.mX, 0.0f, 0.2f);

	currentArmRotationOffset.mZ = Math::Lerp(currentArmRotationOffset.mZ, 90.0f, 0.2f);

	mPtrAnimRenderComp->SetRotateOffset(currentArmRotationOffset);


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
	}

	{
		currentArmRotationOffset.mX -= mouseDeltaY * 0.1f;
		currentArmRotationOffset.mZ += mouseDeltaX * 0.1f;
	}
}

void Player::SetArmRotateOffset(Vec3 offset)
{
	mPtrAnimRenderComp->SetRotateOffset(offset);
}

void Player::SetArmTranslateOffset(Vec3 offset)
{
	currentArmTranslationOffset = offset;
	mPtrAnimRenderComp->SetTranslateOffset(currentArmTranslationOffset);
}

void Player::ProcessMovement(const float& deltaTime)
{
    float targetSpeed = hasMovementInput ? currentTopSpeed : 0.0f;
	float currentHorSpeed = Vector3(currentVelocity.mX, currentVelocity.mY, currentVelocity.mZ).Magnitude();

	if (currentHorSpeed < targetSpeed - speedOffset || currentHorSpeed > targetSpeed + speedOffset)
		currentVelocity = Math::Lerp(currentHorSpeed, targetSpeed, deltaTime * maxSpeedChangingRate) * inputMoveDir;
	else
		currentVelocity = targetSpeed * inputMoveDir;

	Vector3 updatedPos = GetPosition();
	updatedPos += currentVelocity * deltaTime;
	SetPosition(updatedPos);
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

	/*ImGui::Begin("Arm Offset", 0, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat("X Offset", &translationOffset.mX, -2.0f, 2.0f);
	ImGui::SliderFloat("Y Offset", &translationOffset.mY, -2.0f, 2.0f);
	ImGui::SliderFloat("Z Offset", &translationOffset.mZ, -2.0f, 2.0f);
	ImGui::End();


	mPtrAnimRenderComp->SetTranslateOffset(translationOffset);*/

}

void Player::WeaponFiredEventListener(EventOnPlayerWeaponFired inEvent)
{
	mPtrCameraComp->RotateCamera(inEvent.mRecoilDeviation * 5.0f);
	mPtrAudioComponent->Play();
}

void Player::CrouchTimeline(float alpha)
{
	float heightVal = Math::Lerp(1.8f, 0.9f, alpha);
	float armOffsetVal = Math::Lerp(1.32f, 0.56f, alpha);

	mPtrCameraComp->SetEyeHeight(heightVal);
}

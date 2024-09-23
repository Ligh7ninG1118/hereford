#include "Gameplay/Player.h"
#include "Gameplay/CameraComponent.h"
#include "Gameplay/WeaponComponent.h"
#include "Core/GameContext.h"
#include <SDL2/SDL.h>
#include "imgui/imgui.h"

#include "Asset/AssetManager.h"
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

	mPtrAnimRenderComp->SetTranslateOffset(Vec3(0.0f, 1.32f, 0.0f));
	mPtrAnimRenderComp->SetScaleOffset(Vec3(0.03f));
	mPtrAnimRenderComp->SetRotateOffset(Vec3(0.0f, 0.0f, 90.0f));

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
}

Player::~Player()
{
	//printf("Player Destructor\n");
}

void Player::OnUpdate(float deltaTime)
{
	ProcessMovement(deltaTime);
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

	// Rotation
	{
		mRotation.mY += mouseDeltaX * mPtrCameraComp->GetMouseSens();
		if (mRotation.mY > 360.0f)
			mRotation.mY = 0.0f;
		if (mRotation.mY < -360.0f)
			mRotation.mY = 0.0f;
	}

	{
		if (keyState[SDL_SCANCODE_C] == EInputState::KEY_DOWN)
		{
			//TODO: Creating additional copy here. Overload enum version
			if (mPtrActionComp->GetActiveGameplayTags().HasTag(GameplayTag(EActionType::CROUCHING)))
			{
				mPtrActionComp->StopActionByName("Crouch");
				mPtrCameraComp->SetEyeHeight(1.8f);
				mPtrAnimRenderComp->SetTranslateOffset(Vec3(0.0f, 1.32f, 0.0f));
			}
			else
			{
				mPtrActionComp->StartActionByName("Crouch");
				mPtrCameraComp->SetEyeHeight(0.9f);
				mPtrAnimRenderComp->SetTranslateOffset(Vec3(0.0f, 0.56f, 0.0f));
			}
		}

		if (keyState[SDL_SCANCODE_LSHIFT] == EInputState::KEY_DOWN)
		{
			mPtrActionComp->StartActionByName("Sprint");
		}
		if (keyState[SDL_SCANCODE_LSHIFT] == EInputState::KEY_UP)
		{
			mPtrActionComp->StopActionByName("Sprint");
		}

		
	}
}

void Player::ProcessMovement(const float& deltaTime)
{
	// Update Position
	Vector3 updatedPos = GetPosition();
	updatedPos += currentVelocity * deltaTime;
	SetPosition(updatedPos);

	float topSpeed = topWalkingSpeed;
	if (mPtrActionComp->GetActiveGameplayTags().HasTag(GameplayTag(EActionType::CROUCHING)))
		topSpeed = topCrouchSpeed;
	else if (mPtrActionComp->GetActiveGameplayTags().HasTag(GameplayTag(EActionType::SPRINTING)))
		topSpeed = topSprintingSpeed;

	// Update Velocity
	if (hasMovementInput)
	{
		currentVelocity += inputMoveDir * accelerationSpeed * deltaTime;
	}
	else // In case of no-gravity space combat, comment this part
	{
		currentVelocity -= currentVelocity.normalized() * decelerationSpeed * deltaTime;
	}

	// Cap velocity at top speed
	if (currentVelocity.SqrMagnitude() > topSpeed * topSpeed)
	{
		currentVelocity = currentVelocity.normalized() * topSpeed;
	}

	// Prevent drifting
	if (currentVelocity.SqrMagnitude() < minVelocityOffset)
	{
		currentVelocity = Vector3::Zero;
	}
}

void Player::ShowDebugInfo()
{
	ImGui::Begin("Player");
	Vec3 pos = GetPosition();
	Vec3 rot = mPtrCameraComp->GetRotation();
	ImGui::Text("Pos (%.2f, %.2f, %.2f)\tRot (%.2f, %.2f, %.2f)", pos.mX, pos.mY, pos.mZ, rot.mX, rot.mY, rot.mZ);
	ImGui::End();

}

void Player::WeaponFiredEventListener(EventOnPlayerWeaponFired inEvent)
{
	mPtrCameraComp->RotateCamera(inEvent.mRecoilDeviation);
}

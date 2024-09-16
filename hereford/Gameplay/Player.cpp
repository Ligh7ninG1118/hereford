#include "Gameplay/Player.h"
#include "Gameplay/CameraComponent.h"
#include "Gameplay/WeaponComponent.h"
#include "Core/GameContext.h"
#include <SDL2/SDL.h>

#include "Asset/AssetManager.h"
#include "Animation/Animation.h"
#include "Animation/Animator.h"
#include "Animation/AnimationStateMachine.h"

#include "Graphics/AnimatedRenderComponent.h"

#include "UI/UIAmmoIndicator.h"
#include "UI/UICrosshair.h"

#include "stdio.h"

Player::Player(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	mPtrCameraComp = std::make_unique<CameraComponent>(static_cast<Actor*>(this));

	
	mPtrAnimRenderComp = std::make_unique<AnimatedRenderComponent>(this, mGame->GetRenderer());
	mPtrAnimRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/mark23/source/Mark23v3.fbx")));
	mPtrAnimRenderComp->SetShader(AssetManager::LoadAsset<Shader>(std::string("Shaders/model_tex_vert.glsl*Shaders/model_tex_frag.glsl")));

	mPtrAnimRenderComp->SetTranslateOffset(Vec3(0.0f, 1.32f, 0.0f));
	mPtrAnimRenderComp->SetScaleOffset(Vec3(0.03f));
	mPtrAnimRenderComp->SetRotateOffset(Vec3(90.0f, 0.0f, 0.0f));

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

	Renderer* renderer = &mGame->GetRenderer();
	Vec2 screenDimension = renderer->GetScreenDimension();

	//TODO: Separate HUD Actor class to handle all HUD elements?
	mPtrUIAmmo = new UIAmmoIndicator(renderer, ammoShader.get(), ammoTex, mPtrActiveWeaponComp);
	Mat4 uiProj = mPtrCameraComp->GetOrthoMatrix(0.0f, screenDimension.mX, 0.0f, screenDimension.mY);
	mPtrUIAmmo->Initialize();
	mPtrUIAmmo->SetUIProjection(uiProj);

	std::shared_ptr<Shader> crosshairShader = AssetManager::LoadAsset<Shader>(std::string("Shaders/ui_crosshair_vert.glsl*Shaders/ui_crosshair_frag.glsl"));

	mPtrUICrosshair = new UICrosshair(renderer, crosshairShader.get(), mPtrActiveWeaponComp);
	mPtrUICrosshair->Initialize();
}

Player::~Player()
{
	//printf("Player Destructor\n");
}

void Player::OnUpdate(float deltaTime)
{
	ProcessMovement(deltaTime);
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

			/*printf("Event fired.\n");
			GameEvent::Publish<EventTest>(EventTest(42));*/
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
}

void Player::ProcessMovement(const float& deltaTime)
{
	// Update Position
	Vector3 updatedPos = GetPosition();
	updatedPos += currentVelocity * deltaTime;
	SetPosition(updatedPos);

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
	if (currentVelocity.SqrMagnitude() > topWalkingSpeed * topWalkingSpeed)
	{
		currentVelocity = currentVelocity.normalized() * topWalkingSpeed;
	}

	// Prevent drifting
	if (currentVelocity.SqrMagnitude() < minVelocityOffset)
	{
		currentVelocity = Vector3::Zero;
	}
}

void Player::WeaponFiredEventListener(EventOnPlayerWeaponFired inEvent)
{
	mPtrCameraComp->RotateCamera(inEvent.mRecoilDeviation);
}

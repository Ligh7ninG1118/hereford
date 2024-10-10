#include "Weapon.h"
#include "Core/GameContext.h"

#include "Gameplay/WeaponComponent.h"
#include "Graphics/AnimatedRenderComponent.h"
#include "Asset/AssetManager.h"



Weapon::Weapon(GameContext* gameCtx)
	: Actor(gameCtx)
{
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
}

Weapon::~Weapon()
{
}

void Weapon::OnUpdate(float deltaTime)
{
}

void Weapon::OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
}

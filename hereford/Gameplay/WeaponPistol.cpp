#include "WeaponPistol.h"
#include <memory>
#include "Asset/AssetManager.h"
#include "Animation/Animator.h"
#include "Animation/AnimationStateMachine.h"
#include "Core/GameContext.h"
#include "Gameplay/Player.h"
#include "Gameplay/WeaponComponent.h"
#include "Graphics/AnimatedRenderComponent.h"
#include "Audio/AudioComponent.h"

WeaponPistol::WeaponPistol(GameContext* gameCtx)
	: Weapon(gameCtx)
{
}

WeaponPistol::~WeaponPistol()
{
}

void WeaponPistol::Init(Player* playerPtr)
{
	Weapon::Init(playerPtr);

	mPtrAnimRenderComp = std::make_unique<AnimatedRenderComponent>(this, mGameCtx->GetRenderer(), ERenderLayer::Weapon);
	mPtrAnimRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/mark23/source/Mark23v3.fbx")));

	mPtrAnimRenderComp->SetShader(AssetManager::LoadAsset<Shader>(std::string("Shaders/model_tex_pbr_vert.glsl*Shaders/model_tex_phong_frag.glsl")));
	//currentArmTranslationOffset = hipArmTranslationOffset;
	//currentArmRotationOffset = hipArmRotationOffset;
	mPtrAnimRenderComp->SetTranslateOffset(Vec3(-0.2f, -0.4f, 0.0f));
	mPtrAnimRenderComp->SetScaleOffset(Vec3(0.02f));
	mPtrAnimRenderComp->SetRotateOffset(Vec3(0.0f, 0.0f, 90.0f));


	mPtrAnimRenderComp->SetCamera(&(mPtrPlayer->GetMainCamera()));

	std::unique_ptr<Animator> animator = std::make_unique<Animator>(
		Animator(Animation::LoadAnimations("LocalResources/mark23/source/Mark23v3.fbx", mPtrAnimRenderComp->GetModel())));

	// 0: Draw, 1: Hide, 2: Static, 3: Reload, 4: Fire
	// Construct shared ptr in place to avoid copying unique ptr inside ASM class
	mPtrAnimStateMachine = std::shared_ptr<AnimationStateMachine>(new AnimationStateMachine(this, std::move(animator)));
	mPtrAnimStateMachine->AddTransitionRule(0, AnimState(2, false));
	mPtrAnimStateMachine->AddTransitionRule(3, AnimState(2, false));
	mPtrAnimStateMachine->AddTransitionRule(4, AnimState(2, false));

	mPtrAnimRenderComp->SetAnimator(mPtrAnimStateMachine->GetAnimator());

	mPtrWeaponComp = std::make_unique<WeaponComponent>(static_cast<Actor*>(this), mPtrAnimStateMachine);

	mPtrAudioComponent = std::make_unique<AudioComponent>(this, GetGameContext()->GetAudioManager());
	mPtrAudioComponent->InitAsset("USP_SingleFire.wav");
}

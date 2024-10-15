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
	mPtrAnimRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/mark23/source/Mark23v5.fbx")));

	mPtrAnimRenderComp->SetShader(AssetManager::LoadAsset<Shader>(std::string("Shaders/model_tex_pbr_vert.glsl*Shaders/model_tex_phong_frag.glsl")));
	hipArmTranslationOffset = Vec3(-0.2f, -0.4f, 0.0f);
	currentArmTranslationOffset = hipArmTranslationOffset;
	hipArmRotationOffset = Vec3(0.0f, 0.0f, 90.0f);
	currentArmRotationOffset = hipArmRotationOffset;
	mPtrAnimRenderComp->SetTranslateOffset(currentArmTranslationOffset);
	mPtrAnimRenderComp->SetScaleOffset(Vec3(0.02f));
	mPtrAnimRenderComp->SetRotateOffset(currentArmRotationOffset);


	mPtrAnimRenderComp->SetCamera(&(mPtrPlayer->GetMainCamera()));

	std::unique_ptr<Animator> animator = std::make_unique<Animator>(
		Animator(Animation::LoadAnimations("LocalResources/mark23/source/Mark23v5.fbx", mPtrAnimRenderComp->GetModel())));

	// 0: Hide, 1: Static, 2: Reload, 3: Fire, 4: Equip
	// Construct shared ptr in place to avoid copying unique ptr inside ASM class
	mPtrAnimStateMachine = std::shared_ptr<AnimationStateMachine>(new AnimationStateMachine(this, std::move(animator)));
	mPtrAnimStateMachine->AddTransitionRule(0, AnimState(1, false));
	mPtrAnimStateMachine->AddTransitionRule(2, AnimState(1, false));
	mPtrAnimStateMachine->AddTransitionRule(3, AnimState(1, false));
	mPtrAnimStateMachine->AddTransitionRule(4, AnimState(1, false));

	mPtrAnimRenderComp->SetAnimator(mPtrAnimStateMachine->GetAnimator());

	mPtrWeaponComp = std::make_unique<WeaponComponent>(static_cast<Actor*>(this), mPtrAnimStateMachine);

	mPtrAudioComponent = std::make_unique<AudioComponent>(this, GetGameContext()->GetAudioManager());
	mPtrAudioComponent->InitAsset("USP_SingleFire.wav");

	//TODO: Specific weapon parameters

	//TODO: separate into differnt init functions?
}

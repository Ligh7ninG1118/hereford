#include "WeaponSMG.h"
#include <memory>
#include "Asset/AssetManager.h"
#include "Animation/Animator.h"
#include "Animation/AnimationStateMachine.h"
#include "Core/GameContext.h"
#include "Gameplay/Player.h"
#include "Gameplay/WeaponComponent.h"
#include "Graphics/AnimatedRenderComponent.h"
#include "Audio/AudioComponent.h"

WeaponSMG::WeaponSMG(GameContext* gameCtx)
	: Weapon(gameCtx)
{
}

WeaponSMG::~WeaponSMG()
{
}

void WeaponSMG::Init(Player* playerPtr)
{
	Weapon::Init(playerPtr);

	mPtrAnimRenderComp = std::make_unique<AnimatedRenderComponent>(this, mGameCtx->GetRenderer(), ERenderLayer::Weapon);
	mPtrAnimRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/mp5sd/MP5SDv4.fbx")));

	mPtrAnimRenderComp->SetShader(AssetManager::LoadAsset<Shader>(std::string("Shaders/model_tex_pbr_vert.glsl*Shaders/model_tex_phong_frag.glsl")));
	hipArmTranslationOffset = Vec3(-0.2f, -2.0f, 0.0f);
	currentArmTranslationOffset = hipArmTranslationOffset;
	hipArmRotationOffset = Vec3(0.0f, 0.0f, 90.0f);
	currentArmRotationOffset = hipArmRotationOffset;
	mPtrAnimRenderComp->SetTranslateOffset(currentArmTranslationOffset);
	mPtrAnimRenderComp->SetScaleOffset(Vec3(0.010f));
	mPtrAnimRenderComp->SetRotateOffset(currentArmRotationOffset);


	mPtrAnimRenderComp->SetCamera(&(mPtrPlayer->GetMainCamera()));

	std::unique_ptr<Animator> animator = std::make_unique<Animator>(
		Animator(Animation::LoadAnimations("LocalResources/mp5sd/MP5SDv4.fbx", mPtrAnimRenderComp->GetModel())));

	// 0: Draw, 1: Hide, 2: Static, 3: Reload, 4: Fire
	// Construct shared ptr in place to avoid copying unique ptr inside ASM class
	mPtrAnimStateMachine = std::shared_ptr<AnimationStateMachine>(new AnimationStateMachine(this, std::move(animator)));
	mPtrAnimStateMachine->AddTransitionRule(0, AnimState(2, false));
	mPtrAnimStateMachine->AddTransitionRule(3, AnimState(2, false));
	mPtrAnimStateMachine->AddTransitionRule(4, AnimState(2, false));

	mPtrAnimRenderComp->SetAnimator(mPtrAnimStateMachine->GetAnimator());

	mPtrWeaponComp = std::make_unique<WeaponComponent>(static_cast<Actor*>(this), mPtrAnimStateMachine);
	mPtrWeaponComp->mIsSemiAuto = false;
	mPtrWeaponComp->mFireRatePerMin = 700.0f;
	mPtrWeaponComp->mFireRateCooldown = 60.0f / 700.0f;

	mPtrWeaponComp->mMaxMagazineCapacity = 30;
	mPtrWeaponComp->mMaxReserveCapacity = 5 * mPtrWeaponComp->mMaxMagazineCapacity;
	mPtrWeaponComp->mIsOpenBolt = true;

	mPtrWeaponComp->mCurrentMagazineAmmo = mPtrWeaponComp->mMaxMagazineCapacity;
	mPtrWeaponComp->mCurrentReserveAmmo = mPtrWeaponComp->mMaxReserveCapacity;


	mPtrAudioComponent = std::make_unique<AudioComponent>(this, GetGameContext()->GetAudioManager());
	mPtrAudioComponent->InitAsset("MP5SD_SingleFire.wav");

}

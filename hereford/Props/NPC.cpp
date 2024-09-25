#include "NPC.h"
#include "Core/GameContext.h"
#include "Graphics/AnimatedRenderComponent.h"
#include "Animation/AnimationStateMachine.h"
#include "Animation/Animator.h"
#include "Asset/AssetManager.h"
#include "Asset/Model.h"
#include "Asset/Shader.h"

NPC::NPC(GameContext* gameCtx)
	: Actor(gameCtx)
{
	mPtrAnimRenderComp = new AnimatedRenderComponent(static_cast<Actor*>(this), gameCtx->GetRenderer());
	mPtrAnimRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/SillyDancing/Silly Dancing.fbx")));
	mPtrAnimRenderComp->SetShader(AssetManager::LoadAsset<Shader>(std::string("Shaders/model_tex_pbr_vert.glsl*Shaders/model_tex_pbr_frag.glsl")));
	mPtrAnimRenderComp->SetScaleOffset(Vec3(0.0125f));
	mPtrAnimRenderComp->SetRotateOffset(Vec3(90.0f, 0.0f, 0.0f));

	std::unique_ptr<Animator> animator = std::make_unique<Animator>(
		Animator(Animation::LoadAnimations("LocalResources/SillyDancing/Silly Dancing.fbx", mPtrAnimRenderComp->GetModel())));
	mPtrAnimStateMachine = new AnimationStateMachine(static_cast<Actor*>(this), std::move(animator));
	mPtrAnimStateMachine->PlayAnimation(0, true);

	mPtrAnimRenderComp->SetAnimator(mPtrAnimStateMachine->GetAnimator());

}

NPC::~NPC()
{
}

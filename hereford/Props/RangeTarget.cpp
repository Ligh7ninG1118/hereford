#include "Props/RangeTarget.h"
#include "Graphics/RenderComponent.h"
#include "Core/GameContext.h"
#include "Physics/PhysicsComponent.h"
#include "Audio/AudioComponent.h"
#include "Asset/Primitive.h"
#include "Asset/AssetManager.h"
#include "Asset/Texture.h"

RangeTarget::RangeTarget(GameContext* gameCtx)
	:
	Actor(gameCtx),
	hasHit(false)
{
	mPtrRenderComp = new RenderComponent(static_cast<Actor*>(this), gameCtx->GetRenderer());

	mPtrRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/rangedummy/model3.fbx")));
	mPtrRenderComp->SetScaleOffset(Vec3(0.015f));
	mPtrRenderComp->SetRotateOffset(Vec3(-90.0f, 0.0f, 90.0f));
	mPtrRenderComp->SetTranslateOffset(Vec3(0.0f, 0.0f, -0.05f));

	mPtrRenderComp->SetRenderModeFlag(RM_DIFFUSETEX | RM_STATIC | RM_PBR | RM_MODELMESH);

	auto model = mPtrRenderComp->GetModel();
	
	Texture diffuseTex("LocalResources/rangedummy/01 - Default_albedo.jpg");
	diffuseTex.SetType(ETextureType::DIFFUSE);
	model.lock()->ManualLoadTexture(diffuseTex);

	Texture aoTex("LocalResources/rangedummy/01 - Default_AO.jpg");
	aoTex.SetType(ETextureType::AMBIENT);
	model.lock()->ManualLoadTexture(aoTex);

	Texture metallicTex("LocalResources/rangedummy/01 - Default_metallic.jpg");
	metallicTex.SetType(ETextureType::METALLIC);
	model.lock()->ManualLoadTexture(metallicTex);

	Texture normalTex("LocalResources/rangedummy/01 - Default_normal.png");
	normalTex.SetType(ETextureType::NORMALS);
	model.lock()->ManualLoadTexture(normalTex);

	Texture roughnessTex("LocalResources/rangedummy/01 - Default_roughness.jpg");
	roughnessTex.SetType(ETextureType::ROUGHNESS);
	model.lock()->ManualLoadTexture(roughnessTex);

	std::shared_ptr<Shader> shader = AssetManager::LoadAsset<Shader>(std::string("Shaders/model_tex_pbr_vert.glsl*Shaders/model_tex_pbr_frag.glsl"));
	mPtrRenderComp->SetShader(shader);

	PhysicsPrimitive bodyPrimitive = PhysicsPrimitive{ AABBPrimitive{Vec3(0.05f, 0.4f, 0.4f)}, Vec3(0.0f, 1.3f, 0.0f) };
	mPtrPhysicsCompBody = new PhysicsComponent(static_cast<Actor*>(this), gameCtx->GetPhysicsManager(), bodyPrimitive);

	PhysicsPrimitive headPrimitive = PhysicsPrimitive{ AABBPrimitive{Vec3(0.05f, 0.225f, 0.15f)}, Vec3(0.0f, 1.95f, 0.0f) };
	mPtrPhysicsCompHead = new PhysicsComponent(static_cast<Actor*>(this), gameCtx->GetPhysicsManager(), headPrimitive);

	mPtrAudioComponent = std::make_unique<AudioComponent>(this, gameCtx->GetAudioManager(), true);
	mPtrAudioComponent->InitAsset("bullet_impact_metal.wav");
}

RangeTarget::~RangeTarget()
{
}


void RangeTarget::Hit(const HitInfo& info)
{
	printf("Impact Point: (%f, %f, %f)\n", info.impactPoint.mX, info.impactPoint.mY, info.impactPoint.mZ);
	printf("Distance: %f\n", info.distance);

	mPtrAudioComponent->Play();

	if (!hasHit)
	{
		TimelineActionManager::PlayFromStart(mHHitTimeline, std::bind(&RangeTarget::HitTimeline, this, std::placeholders::_1), 0.15f);
		hasHit = true;
		mPtrPhysicsCompBody->SetState(EComponentState::Disabled);
		mPtrPhysicsCompHead->SetState(EComponentState::Disabled);
	}
	/*else
	{
		ResetTarget();
	}*/
}

void RangeTarget::ResetTarget()
{
	hasHit = false;
	mPtrPhysicsCompBody->SetState(EComponentState::Enabled);
	mPtrPhysicsCompHead->SetState(EComponentState::Enabled);
	Vec3 rot = GetRotation();
	rot.mY = 0.0f;
	SetRotation(rot);
}

void RangeTarget::HitTimeline(float alpha)
{
	Vec3 rot = GetRotation();
	rot.mY = Math::Lerp(rot.mY, 80.0f, alpha);

	SetRotation(rot);
}


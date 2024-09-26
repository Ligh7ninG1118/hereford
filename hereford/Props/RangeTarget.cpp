#include "Props/RangeTarget.h"
#include "Graphics/RenderComponent.h"
#include "Core/GameContext.h"
#include "Physics/PhysicsComponent.h"
#include "Asset/Primitive.h"
#include "Asset/AssetManager.h"
#include "Asset/Texture.h"

RangeTarget::RangeTarget(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	mPtrRenderComp = new RenderComponent(static_cast<Actor*>(this), gameCtx->GetRenderer());
	mPtrPhysicsComp = new PhysicsComponent(static_cast<Actor*>(this), gameCtx->GetPhysicsManager());

	PrimitiveInfo cubeInfo;
	Primitive::GenerateCube(cubeInfo);
	mPtrRenderComp->SetVAOID(cubeInfo.mVAO);
	mPtrRenderComp->SetVBOID(cubeInfo.mVBO);
	mPtrRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/rangedummy/model3.fbx")));
	mPtrRenderComp->SetScaleOffset(Vec3(0.015f));
	mPtrRenderComp->SetRotateOffset(Vec3(-90.0f, 0.0f, 90.0f));

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

}

RangeTarget::~RangeTarget()
{
}

void RangeTarget::OnUpdate(float deltaTime)
{

}

void RangeTarget::Hit(const HitInfo& info)
{
	printf("Impact Point: (%f, %f, %f)\n", info.impactPoint.mX, info.impactPoint.mY, info.impactPoint.mZ);
	printf("Distance: %f\n", info.distance);
	SetState(ActorState::Destroy);
}

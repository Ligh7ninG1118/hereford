#include "Ground.h"

#include "Core/GameContext.h"
#include "Graphics/RenderComponent.h"
#include "Physics/PhysicsComponent.h"
#include "Asset/Primitive.h"
#include "Asset/AssetManager.h"

Ground::Ground(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	mPtrRenderComp = new RenderComponent(static_cast<Actor*>(this), gameCtx->GetRenderer());
	mPtrPhysicsComp = new PhysicsComponent(static_cast<Actor*>(this), gameCtx->GetPhysicsManager());

	PrimitiveInfo planeInfo;
	Primitive::GeneratePlane(planeInfo, 100, 100);
	mPtrRenderComp->SetVAOID(planeInfo.mVAO);
	mPtrRenderComp->SetVBOID(planeInfo.mVBO);
	mPtrRenderComp->LoadTexture("LocalResources/gravel/gravel_albedo.png", ETextureType::DIFFUSE);
	mPtrRenderComp->LoadTexture("LocalResources/gravel/gravel_ao.png", ETextureType::AMBIENT);
	mPtrRenderComp->LoadTexture("LocalResources/gravel/gravel_height.png", ETextureType::HEIGHT);
	mPtrRenderComp->LoadTexture("LocalResources/gravel/gravel_metallic.png", ETextureType::METALLIC);
	mPtrRenderComp->LoadTexture("LocalResources/gravel/gravel_normal-ogl.png", ETextureType::NORMALS);
	mPtrRenderComp->LoadTexture("LocalResources/gravel/gravel_roughness.png", ETextureType::ROUGHNESS);
	mPtrRenderComp->SetRenderModeFlag(RM_EXPLICITTEX | RM_STATIC | RM_LIGHTINGANDIBL | RM_SIMPLEMESH);

	std::shared_ptr<Shader> shader = AssetManager::LoadAsset<Shader>(std::string("Shaders/model_tex_pbr_vert.glsl*Shaders/model_tex_pbr_frag.glsl"));
	mPtrRenderComp->SetShader(shader);

	mPtrRenderComp->SetColor(Vec3(0.5f, 0.5f, 0.5f));
}

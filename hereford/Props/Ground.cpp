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

	std::shared_ptr<Shader> shader = AssetManager::LoadAsset<Shader>(std::string("Shaders/standard_vert.glsl*Shaders/standard_frag.glsl"));
	mPtrRenderComp->SetShader(shader);

	mPtrRenderComp->SetColor(Vec3(0.5f, 0.5f, 0.5f));
}

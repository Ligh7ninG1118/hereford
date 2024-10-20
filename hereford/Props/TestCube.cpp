#include "Props/TestCube.h"
#include "Graphics/RenderComponent.h"
#include "Core/GameContext.h"
#include "Physics/PhysicsComponent.h"
#include "Asset/Primitive.h"
#include "Util/Random.h"
#include "Asset/AssetManager.h"

TestCube::TestCube(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	mPtrRenderComp = std::make_unique<RenderComponent>(static_cast<Actor*>(this), gameCtx->GetRenderer());
	mPtrPhysicsComp = std::make_unique<PhysicsComponent>(static_cast<Actor*>(this), gameCtx->GetPhysicsManager());

	PrimitiveInfo cubeInfo;
	Primitive::GenerateCube(cubeInfo);
	mPtrRenderComp->SetVAOID(cubeInfo.mVAO);
	mPtrRenderComp->SetVBOID(cubeInfo.mVBO);
	mPtrRenderComp->LoadTexture("LocalResources/used-stainless-steel2/used-stainless-steel2_albedo.png", ETextureType::DIFFUSE);
	mPtrRenderComp->LoadTexture("LocalResources/used-stainless-steel2/used-stainless-steel2_ao.png", ETextureType::AMBIENT);
	mPtrRenderComp->LoadTexture("LocalResources/used-stainless-steel2/used-stainless-steel2_height.png", ETextureType::HEIGHT);
	mPtrRenderComp->LoadTexture("LocalResources/used-stainless-steel2/used-stainless-steel2_metallic.png", ETextureType::METALLIC);
	mPtrRenderComp->LoadTexture("LocalResources/used-stainless-steel2/used-stainless-steel2_normal-ogl.png", ETextureType::NORMALS);
	mPtrRenderComp->LoadTexture("LocalResources/used-stainless-steel2/used-stainless-steel2_roughness.png", ETextureType::ROUGHNESS);



	std::shared_ptr<Shader> shader = AssetManager::LoadAsset<Shader>(std::string("Shaders/model_tex_pbr_vert.glsl*Shaders/model_tex_pbr_frag.glsl"));
	mPtrRenderComp->SetShader(shader);

	mPtrRenderComp->SetColor(Vec3(Random::Range(0.1f, 0.9f), Random::Range(0.1f, 0.9f), Random::Range(0.1f, 0.9f)));

	mPtrRenderComp->SetRenderModeFlag(RM_DIFFUSETEX | RM_STATIC | RM_PBR | RM_SIMPLEMESH);
}

TestCube::~TestCube()
{
}

void TestCube::OnUpdate(float deltaTime)
{

}

void TestCube::Hit(const HitInfo& info)
{
	printf("Impact Point: (%f, %f, %f)\n", info.impactPoint.mX, info.impactPoint.mY, info.impactPoint.mZ);
	printf("Distance: %f\n", info.distance);
	SetState(EActorState::Destroy);

	//m_pPhysicsComponent->AddImpulse(info.impactPoint, info.hitPower);
}

void TestCube::Interact()
{
	printf("Interacted\n");
	SetState(EActorState::Destroy);
}

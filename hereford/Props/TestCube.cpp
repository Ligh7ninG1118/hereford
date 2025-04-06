#include "Props/TestCube.h"
#include "Graphics/RenderComponent.h"
#include "Core/GameContext.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/PhysicsPrimitive.h"
#include "Asset/Primitive.h"
#include "Util/Random.h"
#include "Asset/AssetManager.h"

TestCube::TestCube(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	mPtrPhysicsComp = std::make_unique<PhysicsComponent>(static_cast<Actor*>(this), gameCtx->GetPhysicsManager());
	mPtrPhysicsComp->SetBoundingPrimitive(PhysicsPrimitive{ AABBPrimitive{Vec3(0.5f)}, Vec3::Zero });


	mPtrRenderComp = std::make_unique<RenderComponent>(static_cast<Actor*>(this), gameCtx->GetRenderer());

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

	mPtrRenderComp->SetBoundingPrimitive(mPtrPhysicsComp->GetBoundingPrimitive());

	std::shared_ptr<Shader> shader = AssetManager::LoadAsset<Shader>(std::string("Shaders/standard_instanced_vert.glsl*Shaders/model_tex_pbr_frag.glsl"));
	mPtrRenderComp->SetShader(shader);

	mPtrRenderComp->SetColor(Vec3(Random::Range(0.1f, 0.9f), Random::Range(0.1f, 0.9f), Random::Range(0.1f, 0.9f)));

	mPtrRenderComp->SetRenderModeFlag(RM_EXPLICITTEX | RM_STATIC | RM_LIGHTINGANDIBL | RM_SIMPLEMESH);

	mPtrRenderComp->bInstancedDraw = true;
	
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

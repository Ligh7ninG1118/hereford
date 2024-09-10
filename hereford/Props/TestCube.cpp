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
	m_pRenderComponent = new RenderComponent(static_cast<Actor*>(this), gameCtx->GetRenderer());
	m_pPhysicsComponent = new PhysicsComponent(static_cast<Actor*>(this), gameCtx->GetPhysicsManager());

	PrimitiveInfo cubeInfo;
	Primitive::GenerateCube(cubeInfo);
	m_pRenderComponent->SetVAOID(cubeInfo.mVAO);
	m_pRenderComponent->SetVBOID(cubeInfo.mVBO);

	std::shared_ptr<Shader> shader = AssetManager::LoadAsset<Shader>(std::string("Shaders/standard_vert.glsl*Shaders/standard_frag.glsl"));
	m_pRenderComponent->SetShader(shader);

	m_pRenderComponent->SetColor(Vec3(Random::Range(0.1f, 0.9f), Random::Range(0.1f, 0.9f), Random::Range(0.1f, 0.9f)));
}

TestCube::~TestCube()
{
}

void TestCube::OnUpdate(float deltaTime)
{
	/*Vec3 pos = GetPosition();

	if (pos.mZ > 5.0f)
	{
		multiplier = -1.0f;
	}
	else if (pos.mZ < -5.0f)
	{
		multiplier = 1.0f;
	}

	pos.mZ += multiplier * deltaTime;
	SetPosition(pos);*/
}

void TestCube::Hit(const HitInfo& info)
{
	SetState(ActorState::Destroy);
}

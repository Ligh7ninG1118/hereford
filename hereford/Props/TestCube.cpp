#include "Props/TestCube.h"
#include "Graphics/RenderComponent.h"
#include "Core/GameContext.h"
#include "Physics/PhysicsComponent.h"
#include "Util/Random.h"

TestCube::TestCube(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	m_pRenderComponent = new RenderComponent(static_cast<Actor*>(this), gameCtx->GetRenderer());
	m_pPhysicsComponent = new PhysicsComponent(static_cast<Actor*>(this), gameCtx->GetPhysicsManager());
	m_pRenderComponent->color = Vec3(Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f));
}

TestCube::~TestCube()
{
}

void TestCube::OnUpdate(float deltaTime)
{
	Vec3 pos = GetPosition();

	if (pos.mZ > 5.0f)
	{
		multiplier = -1.0f;
	}
	else if (pos.mZ < -5.0f)
	{
		multiplier = 1.0f;
	}

	pos.mZ += multiplier * deltaTime;
	SetPosition(pos);
}
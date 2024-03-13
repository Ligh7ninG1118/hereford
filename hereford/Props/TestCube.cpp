#include "Props/TestCube.h"
#include "Graphics/RenderComponent.h"
#include "Core/GameContext.h"
#include "Physics/PhysicsComponent.h"

TestCube::TestCube(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	m_pRenderComponent = new RenderComponent(static_cast<Actor*>(this), gameCtx->GetRenderer());
	m_pPhysicsComponent = new PhysicsComponent(static_cast<Actor*>(this), gameCtx->GetPhysicsManager());
}

TestCube::~TestCube()
{
}

#include "TestCube.h"
#include "../Graphics/RenderComponent.h"
#include "../Core/GameContext.h"

TestCube::TestCube(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	m_pRenderComponent = new RenderComponent(static_cast<Actor*>(this), gameCtx->GetRenderer());
}

TestCube::~TestCube()
{
}

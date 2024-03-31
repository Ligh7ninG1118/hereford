#include "Props/LightBulb.h"
#include "Graphics/RenderComponent.h"
#include "Graphics/LightComponent.h"
#include "Core/GameContext.h"

LightBulb::LightBulb(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	m_pRenderComponent = new RenderComponent(static_cast<Actor*>(this), gameCtx->GetRenderer());
	m_pLightComponent = new LightComponent(static_cast<Actor*>(this), gameCtx->GetRenderer());
}

LightBulb::~LightBulb()
{
}

void LightBulb::OnUpdate(float deltaTime)
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

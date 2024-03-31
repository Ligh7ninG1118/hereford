#pragma once
#include "../Core/Actor.h"

class LightBulb : public Actor
{
public:
	LightBulb(class GameContext* gameCtx);
	~LightBulb();
	void OnUpdate(float deltaTime) override;
	class RenderComponent* GetRenderComponent() const { return m_pRenderComponent; }
	class LightComponent* GetLightComponent() const { return m_pLightComponent; }


private:
	class RenderComponent* m_pRenderComponent;
	class LightComponent* m_pLightComponent;
	float multiplier = 1.0f;
};


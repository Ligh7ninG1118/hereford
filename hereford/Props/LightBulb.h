#pragma once
#include "../Core/Actor.h"
#include <memory>

class LightBulb : public Actor
{
public:
	LightBulb(class GameContext* gameCtx);
	~LightBulb();
	void OnUpdate(float deltaTime) override;
	class RenderComponent* GetRenderComponent() const { return mPtrRenderComponent.get(); }
	class LightComponent* GetLightComponent() const { return mPtrLightComponent.get(); }


private:
	std::unique_ptr<class RenderComponent> mPtrRenderComponent;
	std::unique_ptr<class LightComponent> mPtrLightComponent;
	float multiplier = 1.0f;
};


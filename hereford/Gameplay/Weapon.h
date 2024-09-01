#pragma once

#include "Core/Actor.h"


class Weapon : public Actor
{
public:
	Weapon(class GameContext* gameCtx);
	~Weapon();
	void OnUpdate(float deltaTime) override;

	//class RenderComponent* GetRenderComponent() const { return m_pRenderComponent; }

private:

	//class RenderComponent* m_pRenderComponent;
};


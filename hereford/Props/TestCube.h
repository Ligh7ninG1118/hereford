#pragma once
#include "../Core/Actor.h"

class TestCube : public Actor
{
public:
	TestCube(class GameContext* gameCtx);
	~TestCube();
	void OnUpdate(float deltaTime) override;

	class RenderComponent* GetRenderComponent() const { return m_pRenderComponent; }
	class PhysicsComponent* GetPhysicsComponent() const { return m_pPhysicsComponent; }

private:
	float multiplier = 1.0f;
	class RenderComponent* m_pRenderComponent;
	class PhysicsComponent* m_pPhysicsComponent;
};


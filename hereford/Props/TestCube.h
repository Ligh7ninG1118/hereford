#pragma once
#include "Core/Actor.h"
#include "Gameplay/IHittable.h"

class TestCube : public Actor, public IHittable
{
public:
	TestCube(class GameContext* gameCtx);
	~TestCube();
	void OnUpdate(float deltaTime) override;

	void Hit(const struct HitInfo& info) override;

	class RenderComponent* GetRenderComponent() const { return m_pRenderComponent; }
	class PhysicsComponent* GetPhysicsComponent() const { return m_pPhysicsComponent; }

private:
	float multiplier = 1.0f;
	Vec3 mColor;
	class RenderComponent* m_pRenderComponent;
	class PhysicsComponent* m_pPhysicsComponent;
};


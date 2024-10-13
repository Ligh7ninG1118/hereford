#pragma once
#include "Core/Actor.h"
#include "Gameplay/IHittable.h"
#include "Gameplay/IInteractable.h"
#include "Util/Reflection.h"

class TestCube : public Actor, public IHittable, public IInteractable
{
	REFLECT(TestCube)

public:
	TestCube(class GameContext* gameCtx);
	~TestCube();
	void OnUpdate(float deltaTime) override;

	void Hit(const struct HitInfo& info) override;
	void Interact() override;

	class RenderComponent* GetRenderComponent() const { return mPtrRenderComp; }
	class PhysicsComponent* GetPhysicsComponent() const { return mPtrPhysicsComp; }

private:
	float multiplier = 1.0f;
	Vec3 mColor;
	class RenderComponent* mPtrRenderComp;
	class PhysicsComponent* mPtrPhysicsComp;
};

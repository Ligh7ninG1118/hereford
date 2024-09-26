#pragma once
#include "Core/Actor.h"
#include "Gameplay/IHittable.h"
#include "Util/Reflection.h"

class TestCube : public Actor, public IHittable
{
	REFLECT(TestCube)

public:
	TestCube(class GameContext* gameCtx);
	~TestCube();
	void OnUpdate(float deltaTime) override;

	void Hit(const struct HitInfo& info) override;

	class RenderComponent* GetRenderComponent() const { return mPtrRenderComp; }
	class PhysicsComponent* GetPhysicsComponent() const { return mPtrPhysicsComp; }

private:
	float multiplier = 1.0f;
	Vec3 mColor;
	class RenderComponent* mPtrRenderComp;
	class PhysicsComponent* mPtrPhysicsComp;
};

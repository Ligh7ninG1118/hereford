#pragma once
#include "Core/Actor.h"
#include "Gameplay/IHittable.h"
#include "Gameplay/IInteractable.h"
#include "Util/Reflection.h"
#include <memory>


class TestCube : public Actor, public IHittable, public IInteractable
{
	REFLECT(TestCube)

public:
	TestCube(class GameContext* gameCtx);
	~TestCube();
	void OnUpdate(float deltaTime) override;

	void Hit(const struct HitInfo& info) override;
	void Interact() override;

	class RenderComponent* GetRenderComponent() const { return mPtrRenderComp.get(); }
	class PhysicsComponent* GetPhysicsComponent() const { return mPtrPhysicsComp.get(); }

private:
	float multiplier = 1.0f;
	Vec3 mColor;
	std::unique_ptr<class RenderComponent> mPtrRenderComp;
	std::unique_ptr<class PhysicsComponent> mPtrPhysicsComp;
};

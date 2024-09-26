#pragma once
#include "Core/Actor.h"
#include "Gameplay/IHittable.h"
#include "Util/Reflection.h"

class RangeTarget : public Actor, public IHittable
{
	REFLECT(RangeTarget)

public:
	RangeTarget(class GameContext* gameCtx);
	~RangeTarget();
	void OnUpdate(float deltaTime) override;

	void Hit(const struct HitInfo& info) override;

	class RenderComponent* GetRenderComponent() const { return mPtrRenderComp; }
	class PhysicsComponent* GetPhysicsComponent() const { return mPtrPhysicsComp; }

private:
	class RenderComponent* mPtrRenderComp;
	class PhysicsComponent* mPtrPhysicsComp;
};

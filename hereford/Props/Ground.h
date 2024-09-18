#pragma once
#include "Core/Actor.h"
#include "Util/Reflection.h"


class Ground : public Actor
{
	REFLECT(Ground)

public:
	Ground(class GameContext* gameCtx);
	~Ground() {}

	class RenderComponent* GetRenderComponent() const { return mPtrRenderComp; }
	class PhysicsComponent* GetPhysicsComponent() const { return mPtrPhysicsComp; }

private:
	Vec3 mColor;
	class RenderComponent* mPtrRenderComp;
	class PhysicsComponent* mPtrPhysicsComp;
};


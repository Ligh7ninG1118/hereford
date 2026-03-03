#pragma once
#include "Core/Actor.h"
#include "Util/Reflection.h"
#include <memory>


class Ground : public Actor
{
	REFLECT(Ground)

public:
	Ground(class GameContext* gameCtx);
	~Ground();

	class RenderComponent* GetRenderComponent() const { return mPtrRenderComp.get(); }
	class PhysicsComponent* GetPhysicsComponent() const { return mPtrPhysicsComp.get(); }

private:
	Vec3 mColor;
	std::unique_ptr<class RenderComponent> mPtrRenderComp;
	std::unique_ptr<class PhysicsComponent> mPtrPhysicsComp;
};


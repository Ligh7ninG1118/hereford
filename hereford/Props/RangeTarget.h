#pragma once
#include "Core/Actor.h"
#include "Gameplay/IHittable.h"
#include "Util/Reflection.h"
#include "Util/TimelineAction.h"
#include <memory>

class RangeTarget : public Actor, public IHittable
{
	REFLECT(RangeTarget)

public:
	RangeTarget(class GameContext* gameCtx);
	~RangeTarget();

	void Hit(const struct HitInfo& info) override;
	void ResetTarget();

private:
	bool hasHit;
	void HitTimeline(float alpha);


	std::unique_ptr<class RenderComponent> mPtrRenderComp;
	std::unique_ptr<class PhysicsComponent> mPtrPhysicsCompBody;
	std::unique_ptr<class PhysicsComponent> mPtrPhysicsCompHead;

	std::unique_ptr<class AudioComponent> mPtrAudioComponent;

	TAHandle mHHitTimeline;

};

#pragma once
#include "Core/Actor.h"
#include "Gameplay/IHittable.h"
#include "Util/Reflection.h"
#include "Util/TimelineAction.h"

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


	class RenderComponent* mPtrRenderComp;
	class PhysicsComponent* mPtrPhysicsCompBody;
	class PhysicsComponent* mPtrPhysicsCompHead;

	std::unique_ptr<class AudioComponent> mPtrAudioComponent;

	TAHandle mHHitTimeline;

};

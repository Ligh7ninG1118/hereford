#pragma once
#include "Core/Component.h"
#include "Math/Math.h"
#include "PhysicsPrimitive.h"


class PhysicsComponent : public Component
{
public:
	PhysicsComponent(class Actor* owner, class PhysicsManager& phyMgrRef);
	~PhysicsComponent();

	bool Initialize();
	bool CheckCollision(const PhysicsComponent& collider) const;

	Vec3 GetOwnerPosition() const;

	void AddImpulse(Vec3 impactPoint, float impulseForce);

private:
	class PhysicsManager& m_PhysicsManager;

	friend PhysicsManager;
	bool mIsTrigger;
	bool mUseGravity;
	float mMass;
	
	PhysicsPrimitive mPhyPrimitive;

	Vec3 mVelocity;
	Vec3 mAngVelocity;
	Vec3 mAttemptPos;
	Vec3 mAttemptRot;

	bool mIsGrounded;

	float mDrag;
	float mAngDrag;

	//drag, angleDrag
};


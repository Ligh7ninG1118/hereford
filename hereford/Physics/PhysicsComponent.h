#pragma once
#include "Core/Component.h"
#include "Math/Math.h"
#include "PhysicsPrimitive.h"


class PhysicsComponent : public Component
{
public:
	PhysicsComponent(class Actor* owner, class PhysicsManager& phyMgrRef, PhysicsPrimitive primitive = PhysicsPrimitive{ AABBPrimitive{Vec3(0.1f)}, Vec3::Zero});
	~PhysicsComponent();

	bool Initialize();
	bool CheckCollision(const PhysicsComponent& collider) const;

	Vec3 GetOwnerPosition() const;

	void SetPhyPrimitive(const PhysicsPrimitive& primitive) { mPhyPrimitive = primitive; }
	void SetBoundingPrimitive(const PhysicsPrimitive& primitive) { mBoundingPrimitive = primitive; }
	PhysicsPrimitive* GetBoundingPrimitive() { return &mBoundingPrimitive; }

	void AddImpulse(Vec3 impactPoint, float impulseForce);

private:
	class PhysicsManager& m_PhysicsManager;

	friend PhysicsManager;

	//TODO: bool Deactive field

	bool mIsTrigger;
	bool mUseGravity;
	float mMass;
	
	PhysicsPrimitive mPhyPrimitive;
	PhysicsPrimitive mBoundingPrimitive;

	Vec3 mVelocity;
	Vec3 mAngVelocity;
	Vec3 mAttemptPos;
	Vec3 mAttemptRot;

	bool mIsGrounded;

	float mDrag;
	float mAngDrag;

	//drag, angleDrag
};


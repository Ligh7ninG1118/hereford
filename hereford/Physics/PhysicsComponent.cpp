#include "Core/Actor.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/PhysicsManager.h"
#include "PhysicsPrimitive.h"

PhysicsComponent::PhysicsComponent(Actor* owner, class PhysicsManager& phyMgrRef, PhysicsPrimitive primitive)
	: Component(owner),
	mPhysicsManager(phyMgrRef),
	mPhyPrimitive(primitive),
	mIsTrigger(false),
	mUseGravity(false),
	mMass(1.0f),
	mVelocity(Vec3::Zero),
	mAngVelocity(Vec3::Zero),
	mAttemptPos(Vec3::Zero),
	mAttemptRot(Vec3::Zero),
	mIsGrounded(false),
	mDrag(0.1f),
	mAngDrag(0.0f),
	mRestitution(0.5f),
	mFriction(0.5f),
	mInvMass(1.0f / 1.0f),
	mInvInertia(Mat4::Identity)
{
	mPhysicsManager.AddPhysicsComponent(this);
}

PhysicsComponent::~PhysicsComponent()
{
	mPhysicsManager.RemovePhysicsComponent(this);
}

bool PhysicsComponent::Initialize()
{


	return true;
}

bool PhysicsComponent::CheckCollision(const PhysicsComponent& collider) const
{
	return false;
}

Vec3 PhysicsComponent::GetOwnerPosition() const
{
	return GetOwner()->GetPosition();
}

void PhysicsComponent::AddImpulse(Vec3 impactPoint, float impulseForce)
{
	Vec3 impulse = impactPoint * impulseForce;

	mVelocity += impulse / mMass;

	Vec3 r = GetOwnerPosition() - impactPoint;
	Vec3 iNorm = impulse.normalized();

	Vec3 angularDir = iNorm.Cross(r);
	Vec3 angDirNorm = angularDir.normalized();
	//Vec3 angularDir = impulse.Cross(r);

	mAngVelocity += angDirNorm;
}

#include "Core/Actor.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/PhysicsManager.h"
#include "PhysicsPrimitive.h"

PhysicsComponent::PhysicsComponent(Actor* owner, class PhysicsManager& phyMgrRef, PhysicsPrimitive primitive)
	: OldComponent(owner),
	m_PhysicsManager(phyMgrRef),
	mPhyPrimitive(primitive),
	mUseGravity(false),
	mMass(1.0f),
	mDrag(0.1f),
	mAngDrag(0.0f)
{
	m_PhysicsManager.AddPhysicsComponent(this);
}

PhysicsComponent::~PhysicsComponent()
{
	m_PhysicsManager.RemovePhysicsComponent(this);
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

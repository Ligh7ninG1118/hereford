#include "Core/Actor.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/PhysicsManager.h"
#include "PhysicsPrimitive.h"

PhysicsComponent::PhysicsComponent(Actor* owner, class PhysicsManager& phyMgrRef)
	: Component(owner),
	m_PhysicsManager(phyMgrRef)
{
	m_PhysicsManager.AddPhysicsComponent(this);

	mPrimitive = PhysicsPrimitive{ AABBPrimitive{Vec3(0.5f)}, Vec3::Zero};
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

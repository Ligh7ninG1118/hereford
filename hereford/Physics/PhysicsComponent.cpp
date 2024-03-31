#include "Core/Actor.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/PhysicsManager.h"


PhysicsComponent::PhysicsComponent(Actor* owner, class PhysicsManager& phyMgrRef)
	: Component(owner),
	m_PhysicsManager(phyMgrRef),
	m_BVPositionOffset(Vec3(0.0f, 0.0f, 0.0f))
{
	m_PhysicsManager.AddPhysicsComponent(this);
	m_BVPositionOffset = Vec3::Zero;
	m_BVExtend = Vec3(0.5f, 0.5f, 0.5f);
	m_SphereExtend = 0.5f;
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

Vec3 PhysicsComponent::GetBVPosition() const
{
	return mOwner->GetPosition() + m_BVPositionOffset;
}

//bool PhysicsComponent:



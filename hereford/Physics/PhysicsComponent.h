#pragma once
#include "Core/Component.h"
#include "Math/Math.h"



class PhysicsComponent : public Component
{
public:
	PhysicsComponent(class Actor* owner, class PhysicsManager& phyMgrRef);
	~PhysicsComponent();

	bool Initialize();
	bool CheckCollision(const PhysicsComponent& collider) const;

	Vec3 GetBVPosition() const;

private:
	class PhysicsManager& m_PhysicsManager;

	friend PhysicsManager;
	Vec3 m_BVPositionOffset;
	Vec3 m_BVExtend;
	float m_SphereExtend;
};


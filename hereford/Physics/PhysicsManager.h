#pragma once
#include <vector>
//#include "Core/Actor.h"

//TODO a common file for all enums and structs?
struct HitInfo
{
	class Actor* hitActor = nullptr;
	struct Vector3 hitPoint = Vector3::Zero;
};

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();
	bool Initialize();
	void Shutdown();

	static bool RaycastQuery(const struct Vector3& origin, const struct Vector3& dir, 
		const float& maxDistance, HitInfo& outInfo);

private:
	static std::vector<class PhysicsComponent*> mPhysicsComponents;

	friend class PhysicsComponent;
	void AddPhysicsComponent(class PhysicsComponent* c);
	void RemovePhysicsComponent(class PhysicsComponent* c);

	const float GRAVITY = -9.8f;
};


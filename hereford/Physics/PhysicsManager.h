#pragma once
#include <vector>
//#include "Core/Actor.h"

//TODO a common file for all enums and structs?
struct HitInfo
{
	class Actor* hitActor = nullptr;
	struct Vector3 impactPoint = Vector3::Zero;
	float distance = 0.0f;
	float hitPower = 1.0f;
};

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();
	bool Initialize();
	void Shutdown();
	void UpdatePhysics(float deltaTime);

	//TODO: Add parameter penetration power/return all hit result
	static bool RaycastQuery(const struct Vector3& origin, const struct Vector3& dir, 
		float maxDistance, HitInfo& outInfo);

	static bool SpherecastQuery(const struct Vector3& origin, const struct Vector3& dir, float sphereRadius,
		float maxDistance, HitInfo& outInfo);

private:
	void UpdatePosition(float deltaTime);
	void ResolveCollision(float deltaTime);

	static bool RayAgainstSphere(const struct Vector3& origin, const struct Vector3& dir,
		float maxDistance, const struct PhysicsPrimitive& primitive, const struct Vector3& colliderPos, HitInfo& outInfo);

	static bool RayAgainstAABB(const struct Vector3& origin, const struct Vector3& dir,
		float maxDistance, const struct PhysicsPrimitive& primitive, const struct Vector3& colliderPos, HitInfo& outInfo);

	static bool RayAgainstPlane(const struct Vector3& origin, const struct Vector3& dir,
		float maxDistance, const struct PhysicsPrimitive& primitive, const struct Vector3& colliderPos, HitInfo& outInfo);

	static bool RayAgainstCapsule(const struct Vector3& origin, const struct Vector3& dir,
		float maxDistance, const struct PhysicsPrimitive& primitive, const struct Vector3& colliderPos, HitInfo& outInfo);

	static bool SpherecastAgainstSphere(const struct Vector3& origin, const struct Vector3& dir, float sphereRadius,
		float maxDistance, const struct PhysicsPrimitive& primitive, const struct Vector3& colliderPos, HitInfo& outInfo);

	static bool SpherecastAgainstAABB(const struct Vector3& origin, const struct Vector3& dir, float sphereRadius,
		float maxDistance, const struct PhysicsPrimitive& primitive, const struct Vector3& colliderPos, HitInfo& outInfo);

	static bool SpherecastAgainstPlane(const struct Vector3& origin, const struct Vector3& dir, float sphereRadius,
		float maxDistance, const struct PhysicsPrimitive& primitive, const struct Vector3& colliderPos, HitInfo& outInfo);

	static bool SpherecastAgainstCapsule(const struct Vector3& origin, const struct Vector3& dir, float sphereRadius,
		float maxDistance, const struct PhysicsPrimitive& primitive, const struct Vector3& colliderPos, HitInfo& outInfo);



	static std::vector<class PhysicsComponent*> mPhysicsComponents;

	friend class PhysicsComponent;
	void AddPhysicsComponent(class PhysicsComponent* c);
	void RemovePhysicsComponent(class PhysicsComponent* c);

	const float GRAVITY_CONSTANT = -9.8f;
};


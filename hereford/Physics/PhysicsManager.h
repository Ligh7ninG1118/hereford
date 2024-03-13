#pragma once
#include <vector>

struct HitInfo
{
	class Actor* hitActor;
	struct Vector3 hitPoint;
};

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();
	bool Initialize();
	void Shutdown();

	static bool Raycast(const struct Vector3& origin, const struct Vector3& dir, const float& maxDistance);

private:
	static std::vector<class PhysicsComponent*> mPhysicsComponents;

	friend class PhysicsComponent;
	void AddPhysicsComponent(class PhysicsComponent* c);
	void RemovePhysicsComponent(class PhysicsComponent* c);
};


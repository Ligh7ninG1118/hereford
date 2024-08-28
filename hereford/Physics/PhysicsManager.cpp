#include "Math/Math.h"
#include "Physics/PhysicsManager.h"
#include "Physics/PhysicsComponent.h"
#include <vector>
#include <stdio.h>

std::vector<PhysicsComponent*> PhysicsManager::mPhysicsComponents;


PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
}

bool PhysicsManager::Initialize()
{


	return false;
}

void PhysicsManager::Shutdown()
{
}

bool PhysicsManager::RaycastQuery(const struct Vector3& origin, const struct Vector3& dir, const float& maxDistance, HitInfo& outInfo)
{
	printf("-------------------------------------\n");
	bool hasHit = false;

	for (auto collider : mPhysicsComponents)
	{
		// Ray against sphere
		/*float radius = collider->m_SphereExtend;
		Vector3 originToCenter = collider->GetBVPosition() - origin;
		float projectedDis = originToCenter.Dot(dir);

		float originToCenterLenSq = originToCenter.SqrMagnitude();

		if (projectedDis < 0.0f)
		{
			if (originToCenterLenSq < radius * radius)
			{
				printf("inside\n");
				outInfo.hitActor = collider->GetOwner();
				return true;
			}
			else
			{
				printf("no collision\n");
				continue;
			}
		}

		float centerToProjectedDis = sqrt(originToCenterLenSq - projectedDis * projectedDis);
		float sideDis = sqrt(radius * radius - centerToProjectedDis * centerToProjectedDis);

		if (radius * radius - originToCenterLenSq + projectedDis * projectedDis < 0.0f)
		{
			printf("no collision\n");
			continue;
		}
		else if (originToCenterLenSq < radius * radius)
		{
			printf("inside\n");
			outInfo.hitActor = collider->GetOwner();
			return true;
		}
		else
		{
			printf("collided\n");
			outInfo.hitActor = collider->GetOwner();
			return true;
		}*/

		Vec3 aabbMin = collider->GetBVPosition() - collider->GetBVExtend();
		Vec3 aabbMax = collider->GetBVPosition() + collider->GetBVExtend();

		float t1 = (aabbMin.mX - origin.mX) / dir.mX;
		float t2 = (aabbMax.mX - origin.mX) / dir.mX;
		float t3 = (aabbMin.mY - origin.mY) / dir.mY;
		float t4 = (aabbMax.mY - origin.mY) / dir.mY;
		float t5 = (aabbMin.mZ - origin.mZ) / dir.mZ;
		float t6 = (aabbMax.mZ - origin.mZ) / dir.mZ;

		float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

		if (tmax < 0.0f)
		{
			printf("behind\n");
			continue;
		}

		if (tmin > tmax)
		{
			printf("no collision\n");
			continue;
		}

		if (tmin < 0.0f)
		{
			printf("collided\n");
			outInfo.hitActor = collider->GetOwner();
			return true;
		}

		printf("collided\n");
		outInfo.hitActor = collider->GetOwner();
		return true;
	}
	printf("-------------------------------------\n");

	return false;
}

void PhysicsManager::AddPhysicsComponent(PhysicsComponent* c)
{
	mPhysicsComponents.push_back(c);
}

void PhysicsManager::RemovePhysicsComponent(PhysicsComponent* c)
{
	mPhysicsComponents.erase(std::find(mPhysicsComponents.begin(), mPhysicsComponents.end(), c));
}

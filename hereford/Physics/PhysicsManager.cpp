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

bool PhysicsManager::Raycast(const struct Vector3& origin, const struct Vector3& dir, const float& maxDistance)
{
	printf("-------------------------------------\n");

	for (auto collider : mPhysicsComponents)
	{
		float radius = collider->m_SphereExtend;
		Vector3 originToCenter = collider->GetBVPosition() - origin;
		float projectedDis = originToCenter.Dot(dir);

		float originToCenterLenSq = originToCenter.SqrMagnitude();

		if (projectedDis < 0.0f)
		{
			if (originToCenterLenSq < radius * radius)
			{
				printf("inside\n");
				continue;
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
			printf("no collision\n");
		else if (originToCenterLenSq < radius * radius)
			printf("inside\n");
		else
		{
			printf("collided\n");
		}

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

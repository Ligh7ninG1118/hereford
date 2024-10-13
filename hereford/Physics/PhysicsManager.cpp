#include "Math/Math.h"
#include "PhysicsManager.h"
#include "PhysicsComponent.h"
#include "PhysicsPrimitive.h"
#include "Core/Actor.h"
#include <vector>
#include <stdio.h>
#include <variant>


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

void PhysicsManager::UpdatePhysics(float deltaTime)
{
	UpdatePosition(deltaTime);
	ResolveCollision(deltaTime);
}



void PhysicsManager::UpdatePosition(float deltaTime)
{
	for (auto collider : mPhysicsComponents)
	{
		/*if (collider->mUseGravity)
		{
			collider->mVelocity.mY += GRAVITY_CONSTANT * deltaTime;
			Vector3 posChange = collider->mVelocity * deltaTime;
			Vector3 currentPos = collider->GetOwner()->GetPosition();
			collider->mAttemptPos = currentPos + posChange;
		}
		else*/
		{
			Vector3 posChange = collider->mVelocity * deltaTime;
			Vector3 currentPos = collider->GetOwner()->GetPosition();
			collider->mAttemptPos = currentPos + posChange;
			collider->mVelocity = collider->mVelocity * (1.0f - collider->mDrag);

			Vector3 angularChange = collider->mAngVelocity * deltaTime;
			Vector3 realAngular = Vec3(angularChange.mY, 0.0f, 0.0f);
			Vector3 currentRotation = collider->GetOwner()->GetRotation();
			collider->mAttemptRot = currentRotation + realAngular;
			collider->mAngVelocity = collider->mAngVelocity * (1.0f - collider->mAngDrag);
		}
	}
}

void PhysicsManager::ResolveCollision(float deltaTime)
{
	//for (auto collider : mPhysicsComponents)
	//{
	//	AABBPrimitive aabb = std::get<AABBPrimitive>(collider->mPhyPrimitive.mPrimitive);
	//	Vec3 lowestPoint = collider->GetOwnerPosition();
	//	lowestPoint += collider->mPhyPrimitive.mPosOffset;
	//	lowestPoint.mY -= aabb.mExtend.mY;

	//	/*if (lowestPoint.mY < 0.0f)
	//	{
	//		Vec3 normal = Vector3::Up;
	//		Vec3 relativeVelocity = collider->mVelocity - Vec3::Zero;
	//		float velocityAlongNormal = relativeVelocity.Dot(normal);
	//		if (velocityAlongNormal > 0.0f)
	//			continue;

	//		float restitution = 0.85f;
	//		float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;
	//		impulseScalar /= (1.0f / collider->mMass);
	//		Vector3 impulse = impulseScalar * normal;
	//		collider->mVelocity += (1.0f / collider->mMass) * impulse;

	//		float penetrationDepth = 0.01f;
	//		Vector3 correction = penetrationDepth * normal;

	//		collider->mAttemptPos += correction * (1.0f / collider->mMass);

	//	}*/

	//	collider->GetOwner()->SetPosition(collider->mAttemptPos);
	//	collider->GetOwner()->SetRotation(collider->mAttemptRot);
	//}
}

bool PhysicsManager::RaycastQuery(const struct Vector3& origin, const struct Vector3& dir, float maxDistance, HitInfo& outInfo)
{
	//printf("-------------------------------------\n");
	bool hasHit = false;
	float nearestDis = maxDistance;

	for (auto collider : mPhysicsComponents)
	{
		PhysicsPrimitive primitive = collider->mPhyPrimitive;
		HitInfo tempInfo;

		if (std::holds_alternative<SpherePrimitive>(primitive.mPrimitive))
		{ 
			bool result = RayAgainstSphere(origin, dir, maxDistance, primitive, collider->GetOwnerPosition(), tempInfo);
			if (result && tempInfo.distance < nearestDis)
			{
				hasHit = true;
				nearestDis = tempInfo.distance;
				outInfo.hitActor = collider->GetOwner();
				outInfo.impactPoint = tempInfo.impactPoint;
			}
		}
		else if (std::holds_alternative<AABBPrimitive>(primitive.mPrimitive))
		{
			bool result = RayAgainstAABB(origin, dir, maxDistance, primitive, collider->GetOwnerPosition(), tempInfo);
			if (result && tempInfo.distance < nearestDis)
			{
				hasHit = true;
				nearestDis = tempInfo.distance;
				outInfo.hitActor = collider->GetOwner();
				outInfo.impactPoint = tempInfo.impactPoint;
			}
		}
		else if (std::holds_alternative<PlanePrimitive>(primitive.mPrimitive))
		{
			bool result = RayAgainstPlane(origin, dir, maxDistance, primitive, collider->GetOwnerPosition(), tempInfo);
			if (result && tempInfo.distance < nearestDis)
			{
				hasHit = true;
				nearestDis = tempInfo.distance;
				outInfo.hitActor = collider->GetOwner();
				outInfo.impactPoint = tempInfo.impactPoint;
			}
		}
	}
	outInfo.distance = nearestDis;
	return hasHit;
}



bool PhysicsManager::RayAgainstSphere(const Vector3& origin, const Vector3& dir, float maxDistance, 
	const PhysicsPrimitive& primitive, const struct Vector3& colliderPos, HitInfo& outInfo)
{
	Vec3 adjustedPos = colliderPos + primitive.mPosOffset;
	SpherePrimitive sphere = std::get<SpherePrimitive>(primitive.mPrimitive);
	float radius = sphere.mRadius;
	Vector3 originToCenter = adjustedPos - origin;
	float projectedDis = originToCenter.Dot(dir);

	float originToCenterLenSq = originToCenter.SqrMagnitude();

	if (projectedDis < 0.0f)
	{
		if (originToCenterLenSq < radius * radius)
		{
			//printf("inside\n");
			outInfo.impactPoint = origin;
			outInfo.distance = 0.0f;
			return true;
		}
		else
		{
			//printf("no collision\n");
			return false;
		}
	}

	float centerToProjectedDis = sqrt(originToCenterLenSq - projectedDis * projectedDis);
	float sideDis = sqrt(radius * radius - centerToProjectedDis * centerToProjectedDis);

	if (radius * radius - originToCenterLenSq + projectedDis * projectedDis < 0.0f)
	{
		//printf("no collision\n");
		return false;

	}
	else if (originToCenterLenSq < radius * radius)
	{
		//printf("inside\n");
		outInfo.impactPoint = origin;
		outInfo.distance = 0.0f;
		return true;
	}
	else
	{
		//printf("collided\n");
		outInfo.distance = sqrtf(originToCenterLenSq - radius * radius);
		Vector3 originToCenterDir = originToCenter.normalized();
		outInfo.impactPoint = origin + originToCenterDir * outInfo.distance;
		return true;
	}
}

bool PhysicsManager::RayAgainstAABB(const Vector3& origin, const Vector3& dir, float maxDistance, 
	const PhysicsPrimitive& primitive, const struct Vector3& colliderPos, HitInfo& outInfo)
{
	Vec3 adjustedPos = colliderPos + primitive.mPosOffset;
	AABBPrimitive aabb = std::get<AABBPrimitive>(primitive.mPrimitive);

	Vec3 aabbMin = adjustedPos - aabb.mExtend;
	Vec3 aabbMax = adjustedPos + aabb.mExtend;

	float t1 = (aabbMin.mX - origin.mX) / dir.mX;
	float t2 = (aabbMax.mX - origin.mX) / dir.mX;
	float t3 = (aabbMin.mY - origin.mY) / dir.mY;
	float t4 = (aabbMax.mY - origin.mY) / dir.mY;
	float t5 = (aabbMin.mZ - origin.mZ) / dir.mZ;
	float t6 = (aabbMax.mZ - origin.mZ) / dir.mZ;

	float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	if (tmax < 0.0f || tmin > tmax)
	{
		return false;
	}
	else
	{
		outInfo.impactPoint = origin + dir * tmin;
		outInfo.distance = tmin;
		return true;
	}
}

bool PhysicsManager::RayAgainstPlane(const Vector3& origin, const Vector3& dir, float maxDistance, 
	const PhysicsPrimitive& primitive, const Vector3& colliderPos, HitInfo& outInfo)
{
	Vec3 adjustedPos = colliderPos + primitive.mPosOffset;
	PlanePrimitive plane = std::get<PlanePrimitive>(primitive.mPrimitive);

	float dirDotN = dir.Dot(plane.mNormal);

	// Parallel
	if (dirDotN == 0.0f)
		return false;

	float originDotN = origin.Dot(plane.mNormal);

	float t = (plane.mDistance - originDotN) / dirDotN;

	if(t < 0.0f)
		return false;

	outInfo.impactPoint = origin + dir * t;
	outInfo.distance = dir.Magnitude() * t;
	return true;
}

bool PhysicsManager::RayAgainstCapsule(const Vector3& origin, const Vector3& dir, float maxDistance, 
	const PhysicsPrimitive& primitive, const Vector3& colliderPos, HitInfo& outInfo)
{
	Vec3 adjustedPos = colliderPos + primitive.mPosOffset;
	CapsulePrimitive capsule = std::get<CapsulePrimitive>(primitive.mPrimitive);
	Vec3 capsuleTop = adjustedPos,
		capsuleBottom = adjustedPos;
	capsuleTop.mY += capsule.mHalfHeight;
	capsuleBottom.mY -= capsule.mHalfHeight;

	Vec3 capsuleAxis = capsuleTop - capsuleBottom;
	float capsuleLen = capsuleAxis.Magnitude();
	capsuleAxis.Normalize();

	Vec3 originToCapsuleBottom = capsuleBottom - origin;

	float t = originToCapsuleBottom.Dot(capsuleAxis);
	float d = dir.Dot(capsuleAxis);

	Vec3 closestPointOnAxis = capsuleBottom + capsuleAxis * t;
	Vec3 closestPointToOrigin = closestPointOnAxis - origin;

	float sqrDis = closestPointToOrigin.SqrMagnitude();

	if (sqrDis <= capsule.mRadius * capsule.mRadius)
	{
		float intersectionDis = sqrt(capsule.mRadius * capsule.mRadius - sqrDis);
		float rayDis = t - intersectionDis;

		if (rayDis >= 0.0f && rayDis <= maxDistance)
		{
			outInfo.impactPoint = origin + dir * rayDis;
			outInfo.distance = rayDis;
			return true;
		}
	}

	return false;
}

bool PhysicsManager::SpherecastQuery(const Vector3& origin, const Vector3& dir, float sphereRadius, float maxDistance, HitInfo& outInfo)
{
	//printf("-------------------------------------\n");
	bool hasHit = false;
	float nearestDis = maxDistance;

	for (auto collider : mPhysicsComponents)
	{
		PhysicsPrimitive primitive = collider->mPhyPrimitive;
		HitInfo tempInfo;

		if (std::holds_alternative<SpherePrimitive>(primitive.mPrimitive))
		{
			bool result = SpherecastAgainstSphere(origin, dir, sphereRadius, maxDistance,  primitive, collider->GetOwnerPosition(), tempInfo);
			if (result && tempInfo.distance < nearestDis)
			{
				hasHit = true;
				nearestDis = tempInfo.distance;
				outInfo.hitActor = collider->GetOwner();
				outInfo.impactPoint = tempInfo.impactPoint;
			}
		}
		else if (std::holds_alternative<AABBPrimitive>(primitive.mPrimitive))
		{
			bool result = SpherecastAgainstAABB(origin, dir, sphereRadius, maxDistance,  primitive, collider->GetOwnerPosition(), tempInfo);
			if (result && tempInfo.distance < nearestDis)
			{
				hasHit = true;
				nearestDis = tempInfo.distance;
				outInfo.hitActor = collider->GetOwner();
				outInfo.impactPoint = tempInfo.impactPoint;
			}
		}
		else if (std::holds_alternative<PlanePrimitive>(primitive.mPrimitive))
		{
			bool result = SpherecastAgainstPlane(origin, dir, sphereRadius, maxDistance,  primitive, collider->GetOwnerPosition(), tempInfo);
			if (result && tempInfo.distance < nearestDis)
			{
				hasHit = true;
				nearestDis = tempInfo.distance;
				outInfo.hitActor = collider->GetOwner();
				outInfo.impactPoint = tempInfo.impactPoint;
			}
		}
	}
	outInfo.distance = nearestDis;
	return hasHit;
}

bool PhysicsManager::SpherecastAgainstSphere(const Vector3& origin, const Vector3& dir, float sphereRadius, float maxDistance, const PhysicsPrimitive& primitive, const Vector3& colliderPos, HitInfo& outInfo)
{
	Vec3 adjustedPos = colliderPos + primitive.mPosOffset;
	SpherePrimitive sphere = std::get<SpherePrimitive>(primitive.mPrimitive);
	float targetRadius = sphere.mRadius;
	float summedRadius = targetRadius + sphereRadius;
	Vector3 originToCenter = adjustedPos - origin;
	float projectedDis = originToCenter.Dot(dir);

	float originToCenterLenSq = originToCenter.SqrMagnitude();

	if (projectedDis < 0.0f)
	{
		if (originToCenterLenSq < summedRadius * summedRadius)
		{
			//printf("inside\n");
			outInfo.impactPoint = origin;
			outInfo.distance = 0.0f;
			return true;
		}
		else
		{
			//printf("no collision\n");
			return false;
		}
	}

	float centerToProjectedDis = sqrt(originToCenterLenSq - projectedDis * projectedDis);
	float sideDis = sqrt(summedRadius * summedRadius - centerToProjectedDis * centerToProjectedDis);

	if (summedRadius * summedRadius - originToCenterLenSq + projectedDis * projectedDis < 0.0f)
	{
		//printf("no collision\n");
		return false;

	}
	else if (originToCenterLenSq < summedRadius * summedRadius)
	{
		//printf("inside\n");
		outInfo.impactPoint = origin;
		outInfo.distance = 0.0f;
		return true;
	}
	else
	{
		//printf("collided\n");
		outInfo.distance = sqrtf(originToCenterLenSq - summedRadius * summedRadius);
		Vector3 originToCenterDir = originToCenter.normalized();
		outInfo.impactPoint = origin + originToCenterDir * outInfo.distance;
		return true;
	}
}

bool PhysicsManager::SpherecastAgainstAABB(const Vector3& origin, const Vector3& dir, float sphereRadius, float maxDistance, const PhysicsPrimitive& primitive, const Vector3& colliderPos, HitInfo& outInfo)
{
	Vec3 adjustedPos = colliderPos + primitive.mPosOffset;
	AABBPrimitive aabb = std::get<AABBPrimitive>(primitive.mPrimitive);

	Vec3 aabbMin = adjustedPos - aabb.mExtend;
	Vec3 aabbMax = adjustedPos + aabb.mExtend;

	aabbMin.mX -= sphereRadius;
	aabbMin.mY -= sphereRadius;
	aabbMin.mZ -= sphereRadius;
	aabbMax.mX += sphereRadius;
	aabbMax.mY += sphereRadius;
	aabbMax.mZ += sphereRadius;


	float t1 = (aabbMin.mX - origin.mX) / dir.mX;
	float t2 = (aabbMax.mX - origin.mX) / dir.mX;
	float t3 = (aabbMin.mY - origin.mY) / dir.mY;
	float t4 = (aabbMax.mY - origin.mY) / dir.mY;
	float t5 = (aabbMin.mZ - origin.mZ) / dir.mZ;
	float t6 = (aabbMax.mZ - origin.mZ) / dir.mZ;

	float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	if (tmax < 0.0f || tmin > tmax)
	{
		return false;
	}
	else
	{
		outInfo.impactPoint = origin + dir * tmin;
		outInfo.distance = tmin;
		return true;
	}
}

bool PhysicsManager::SpherecastAgainstPlane(const Vector3& origin, const Vector3& dir, float sphereRadius, float maxDistance, const PhysicsPrimitive& primitive, const Vector3& colliderPos, HitInfo& outInfo)
{
	return false;
}

bool PhysicsManager::SpherecastAgainstCapsule(const Vector3& origin, const Vector3& dir, float sphereRadius, float maxDistance, const PhysicsPrimitive& primitive, const Vector3& colliderPos, HitInfo& outInfo)
{
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

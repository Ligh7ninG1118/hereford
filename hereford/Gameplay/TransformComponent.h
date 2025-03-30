#pragma once

#include "HerefordCommon.h"

class TransformComponent : public Component
{
public:
	TransformComponent(uint32_t ownerID);
	~TransformComponent();

	Vector3 GetPosition() const { return mPosition; };
	Vector3 GetScale() const { return mScale; }
	Vector3 GetRotation() const { return mRotation; }

	void SetPosition(const Vector3& pos) { mPosition = pos; }
	void SetScale(const Vector3& scale) { mScale = scale; }
	void SetRotation(const Vector3& rotation) { mRotation = rotation; }

	Vector3 GetForward() const
	{
		return Vector3(cos(DEG2RAD * mRotation.mY) * cos(DEG2RAD * mRotation.mX),
			sin(DEG2RAD * mRotation.mX),
			sin(DEG2RAD * mRotation.mY) * cos(DEG2RAD * mRotation.mX)).normalized();
	}

	Vector3 GetRight() const
	{
		return GetForward().Cross(Vec3(0.0f, 1.0f, 0.0f)).normalized();
	}

private:
	Vector3 mPosition;
	Vector3 mScale;
	Vector3 mRotation;
};


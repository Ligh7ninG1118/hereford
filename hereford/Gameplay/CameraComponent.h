#pragma once

#include "../Core/Component.h"
#include "../Math/Math.h"

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner, float eyeHeight = 1.8f);
	~CameraComponent();

	void Update(float deltaTime) override;
	void ProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY) override;

	void RotateCamera(Vec2 dir);

	void SetEyeHeight(float eyeHeight) { mPositionOffset.mY = eyeHeight; }
	void SetHorFOV(float hov) { mHorFOV = hov; }

	Mat4 GetViewMatrix() const;
	Mat4 GetPerspMatrix(float screenRatio) const;
	Mat4 GetOrthoMatrix(float left, float right, float bottom, float top) const;
	Vec3 GetFrontVector() const
	{
		return Vec3(cos(DEG2RAD * mRotation.mY) * cos(DEG2RAD * mRotation.mX),
			sin(DEG2RAD * mRotation.mX),
			sin(DEG2RAD * mRotation.mY) * cos(DEG2RAD * mRotation.mX)).normalized();
	}
	Vec3 GetRightVector() const
	{
		return GetFrontVector().Cross(Vec3(0.0f, 1.0f, 0.0f)).normalized();
	}

	Vec3 GetRotation() const { return mRotation; }

	float GetMouseSens() const { return mMouseSens; }
	Vec3 GetPositionOffset() const{ return mPositionOffset; }
	Vec3 GetCameraPosition() const;
private:
	Vec3 mPositionOffset;
	Vec3 mRotation;
	float mHorFOV;
	float mNearPlane;
	float mFarPlane;
	float mMouseSens = 0.15f;
};
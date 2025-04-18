#pragma once

#include "../Core/Component.h"
#include "../Math/Math.h"

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner, float eyeHeight = 1.8f);
	~CameraComponent();

	void Update(float deltaTime) override;

	void RotateCamera(Vec2 dir);

	void SetEyeHeight(float eyeHeight) { mPositionOffset.mY = eyeHeight; }
	void SetVerFOV(float hov) { mVerFOV = hov; }

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

	float mAimingSensMultiplier = 1.0f;

private:
	void UpdateViewFromInput(Vec2 mouseDelta);

	friend class Renderer;

	Vec3 mPositionOffset;
	Vec3 mRotation;
	float mVerticalRecenteringTarget;
	Vec2 mDeferredRecoilDir;

	float mVerFOV;
	float mNearPlane;
	float mFarPlane;
	const float mMouseSens = 0.1f;


	const float mCameraRecenteringRate = 15.0f;
	const float mRecoilClimbingRate = 15.0f;
	const float mMaxVerticalAngle = 89.0f;
	const float mMaxHorizontalAngle = 360.0f;

};
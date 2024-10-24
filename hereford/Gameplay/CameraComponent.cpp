#include "CameraComponent.h"
#include "Core/Actor.h"

#include "imgui/imgui.h"

CameraComponent::CameraComponent(Actor* owner, float eyeHeight)
	: Component(owner),
	mDeferredRecoilDir(Vec2(0.0f,0.0f)),
	mPositionOffset(Vector3(0.0f, eyeHeight, 0.0f)),
	mRotation(Vector3(0.0f, 0.0f, 0.0f)),
	mHorFOV(80.0f),
	mNearPlane(0.03f),
	mFarPlane(10000.0f),
	mVerticalRecenteringTarget(0.0f)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(float deltaTime)
{
	/*ImGui::Begin("Recoil");
	ImGui::Text("Deferred (%.2f, %.2f)", mDeferredRecoilDir.mX, mDeferredRecoilDir.mY);
	ImGui::Text("Recenter Target (%.2f)", mVerticalRecenteringTarget);
	ImGui::End();*/

	if (mDeferredRecoilDir.Magnitude() > EPSILON)
	{
		Vec2 stepDir = mDeferredRecoilDir * deltaTime * mCameraRecenteringRate;
		mDeferredRecoilDir -= stepDir;
		mRotation.mY += stepDir.mX;
		mRotation.mX += stepDir.mY;

		mRotation.mX = Math::Clamp(mRotation.mX, -mMaxVerticalAngle, mMaxVerticalAngle);
		mRotation.mY = (mRotation.mY > mMaxHorizontalAngle || mRotation.mY < -mMaxHorizontalAngle) ? 0.0f : mRotation.mY;

		//Set owner rotation as well
		mOwner->SetRotation(Vec3(0.0f, mRotation.mY, 0.0f));
	}
	else if (abs(mRotation.mX - mVerticalRecenteringTarget) > EPSILON)
	{
		mRotation.mX = Math::Lerp(mRotation.mX, mVerticalRecenteringTarget, deltaTime * mCameraRecenteringRate);

	}
}

// use float delta for smoothing?
void CameraComponent::ProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
	mRotation.mY += mouseDeltaX * mMouseSens * mAimingSensMultiplier;
	mRotation.mX += mouseDeltaY * mMouseSens * mAimingSensMultiplier;
	mVerticalRecenteringTarget += mouseDeltaY * mMouseSens * mAimingSensMultiplier;

	if (mDeferredRecoilDir.Magnitude() > EPSILON)
	{
		// Mouse movement on vertical axis can offset deferred recoil changes
		mDeferredRecoilDir.mY -= mouseDeltaY * mMouseSens * mAimingSensMultiplier;
		mDeferredRecoilDir.mY = mDeferredRecoilDir.mX <= 0.0f ? 0.0f : mDeferredRecoilDir.mX;

		//TODO
		if (mouseDeltaY < 0.0f)
			mVerticalRecenteringTarget -= mouseDeltaY * mMouseSens * mAimingSensMultiplier;
	}


	mRotation.mY = (mRotation.mY > mMaxHorizontalAngle || mRotation.mY < -mMaxHorizontalAngle) ? 0.0f : mRotation.mY;
	mRotation.mX = Math::Clamp(mRotation.mX, -mMaxVerticalAngle, mMaxVerticalAngle);
	mVerticalRecenteringTarget = Math::Clamp(mVerticalRecenteringTarget, -mMaxVerticalAngle, mMaxVerticalAngle);

	//Set owner rotation as well
	mOwner->SetRotation(Vec3(0.0f, mRotation.mY, 0.0f));
}

void CameraComponent::RotateCamera(Vec2 dir)
{
	//mDeferredRecoilDir += dir;
}

Mat4 CameraComponent::GetViewMatrix() const
{
	Vec3 Front = GetFrontVector();
	Vec3 Right = GetRightVector();
	Vec3 Up = Right.Cross(Front).normalized();
	Vec3 pos = GetOwner()->GetPosition() + mPositionOffset;

	Mat4 Mtsl2 = Mat4::Identity;
	Mat4 Mrot2 = Mat4::CalculateLookAtMatrix(Front, Right, Up);

	Mtsl2.Translate(-pos);

	return Mrot2 * Mtsl2;
}

Mat4 CameraComponent::GetPerspMatrix(float screenRatio) const
{
	return Mat4::CalculatePerspMatrix(mHorFOV, screenRatio, mNearPlane, mFarPlane);
}

Mat4 CameraComponent::GetOrthoMatrix(float left, float right, float bottom, float top) const
{
	return Mat4::CalculateOrthoMatrix(left, right, bottom, top);
}

Vec3 CameraComponent::GetCameraPosition() const
{
	return mPositionOffset + mOwner->GetPosition();
}

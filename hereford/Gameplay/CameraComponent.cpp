#include "CameraComponent.h"
#include "Core/Actor.h"
#include "Core/GameContext.h"
#include "Input/InputManager.h"
#include "imgui/imgui.h"

CameraComponent::CameraComponent(Actor* owner, float eyeHeight)
	: Component(owner),
	mDeferredRecoilDir(Vec2(0.0f,0.0f)),
	mPositionOffset(Vector3(0.0f, eyeHeight, 0.0f)),
	mRotation(Vector3(0.0f, 0.0f, 0.0f)),
	mVerFOV(110.0f),
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

	UpdateViewFromInput(GetGameContext()->GetInputManager().ReadValue<Vec2>(EInputAction::PLAYER_VIEW));

	if (mDeferredRecoilDir.Magnitude() > EPISILON)
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
	else if (abs(mRotation.mX - mVerticalRecenteringTarget) > EPISILON)
	{
		mRotation.mX = Math::Lerp(mRotation.mX, mVerticalRecenteringTarget, deltaTime * mCameraRecenteringRate);

	}
}

void CameraComponent::UpdateViewFromInput(Vec2 viewDelta)
{
	mRotation.mY += viewDelta.mX * mMouseSens * mAimingSensMultiplier;
	mRotation.mX += viewDelta.mY * mMouseSens * mAimingSensMultiplier;
	mVerticalRecenteringTarget += viewDelta.mY * mMouseSens * mAimingSensMultiplier;

	if (mDeferredRecoilDir.Magnitude() > EPISILON)
	{
		// Mouse movement on vertical axis can offset deferred recoil changes
		mDeferredRecoilDir.mY -= viewDelta.mY * mMouseSens * mAimingSensMultiplier;
		mDeferredRecoilDir.mY = mDeferredRecoilDir.mX <= 0.0f ? 0.0f : mDeferredRecoilDir.mX;

		//TODO
		if (viewDelta.mY < 0.0f)
			mVerticalRecenteringTarget -= viewDelta.mY * mMouseSens * mAimingSensMultiplier;
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
	return Mat4::CalculatePerspMatrix(mVerFOV, screenRatio, mNearPlane, mFarPlane);
}

Mat4 CameraComponent::GetOrthoMatrix(float left, float right, float bottom, float top) const
{
	return Mat4::CalculateOrthoMatrix(left, right, bottom, top);
}

Vec3 CameraComponent::GetCameraPosition() const
{
	return mPositionOffset + mOwner->GetPosition();
}

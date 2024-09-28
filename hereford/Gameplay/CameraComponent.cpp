#include "CameraComponent.h"
#include "Core/Actor.h"


CameraComponent::CameraComponent(Actor* owner, float eyeHeight)
	: Component(owner),
	mDeferredRecoilDir(Vec2(0.0f,0.0f)),
	mPositionOffset(Vector3(0.0f, eyeHeight, 0.0f)),
	mRotation(Vector3(0.0f, 0.0f, 0.0f)),
	mHorFOV(80.0f),
	mNearPlane(0.03f),
	mFarPlane(10000.0f)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(float deltaTime)
{
	if (mDeferredRecoilDir.SqrMagnitude() > EPSILON)
	{
		Vec2 stepDir = mDeferredRecoilDir * deltaTime * 15.0f;
		mDeferredRecoilDir -= stepDir;
		mRotation.mY += stepDir.mX;
		mRotation.mX += stepDir.mY;

		mRotation.mX = Math::Clamp(mRotation.mX, -89.0f, 89.0f);
		mRotation.mY = (mRotation.mY > 360.0f || mRotation.mY < -360.0f) ? 0.0f : mRotation.mY;

		//Set owner rotation as well
		mOwner->SetRotation(Vec3(0.0f, mRotation.mY, 0.0f));

	}
}

// use float delta for smoothing?
void CameraComponent::ProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
	mRotation.mY += mouseDeltaX * mMouseSens;
	mRotation.mX += mouseDeltaY * mMouseSens;

	if (mDeferredRecoilDir.SqrMagnitude() > EPSILON)
	{
		// Mouse movement on vertical axis can offset deferred recoil changes
		mDeferredRecoilDir.mY -= mouseDeltaX * mMouseSens;
		mDeferredRecoilDir.mY = mDeferredRecoilDir.mY <= 0.0f ? 0.0f : mDeferredRecoilDir.mY;
	}


	mRotation.mY = (mRotation.mY > 360.0f || mRotation.mY < -360.0f) ? 0.0f : mRotation.mY;
	mRotation.mX = Math::Clamp(mRotation.mX, -89.0f, 89.0f);

	//Set owner rotation as well
	mOwner->SetRotation(Vec3(0.0f, mRotation.mY, 0.0f));
}

void CameraComponent::RotateCamera(Vec2 dir)
{
	mDeferredRecoilDir += dir;
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

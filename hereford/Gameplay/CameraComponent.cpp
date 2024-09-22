#include "CameraComponent.h"
#include "Core/Actor.h"

CameraComponent::CameraComponent(Actor* owner, float eyeHeight)
	: Component(owner),
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
}

void CameraComponent::ProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
	mRotation.mY += mouseDeltaX * mMouseSens;
	mRotation.mX += mouseDeltaY * mMouseSens;

	mRotation.mY = (mRotation.mY > 360.0f || mRotation.mY < -360.0f) ? 0.0f : mRotation.mY;
	mRotation.mX = Math::Clamp(mRotation.mX, -89.0f, 89.0f);
}

void CameraComponent::RotateCamera(Vec2 dir)
{
	mRotation.mY += dir.mX;
	mRotation.mX += dir.mY;

	mRotation.mX = Math::Clamp(mRotation.mX, -89.0f, 89.0f);
	mRotation.mY = (mRotation.mY > 360.0f || mRotation.mY < -360.0f) ? 0.0f : mRotation.mY;
}

Mat4 CameraComponent::GetViewMatrix() const
{
	Vec3 Front = GetFrontVector();
	Vec3 Right = GetRightVector();
	Vec3 Up = Right.Cross(Front).normalized();
	Vec3 pos = GetOwner()->GetPosition() + mPositionOffset;

	Mat4 Mtsl2 = Mat4::Identity;
	Mat4 Mrot2 = Mat4::Identity;

	Mtsl2.Translate(-pos);

	Mrot2.m[0][0] = Right.mX;	Mrot2.m[1][0] = Right.mY;	Mrot2.m[2][0] = Right.mZ;	Mrot2.m[3][0] = 0.0f;
	Mrot2.m[0][1] = Up.mX;		Mrot2.m[1][1] = Up.mY;		Mrot2.m[2][1] = Up.mZ;		Mrot2.m[3][1] = 0.0f;
	Mrot2.m[0][2] = -Front.mX;	Mrot2.m[1][2] = -Front.mY;	Mrot2.m[2][2] = -Front.mZ;	Mrot2.m[3][2] = 0.0f;
	Mrot2.m[0][3] = 0.0f;		Mrot2.m[1][3] = 0.0f;		Mrot2.m[2][3] = 0.0f;		Mrot2.m[3][3] = 1.0f;


	return Mrot2 * Mtsl2;
}

Mat4 CameraComponent::GetPerspMatrix(const float& screenRatio) const
{
	Mat4 projection = Mat4::Zero;

	float tanHalfFOVy = tan(DEG2RAD * mHorFOV / 2.0f);
	projection.m[0][0] = 1.0f / (screenRatio * tanHalfFOVy);
	projection.m[1][1] = 1.0f / tanHalfFOVy;
	projection.m[2][2] = -(mFarPlane + mNearPlane) / (mFarPlane - mNearPlane);
	projection.m[2][3] = -1.0f;
	projection.m[3][2] = -(2.0f * mFarPlane * mNearPlane) / (mFarPlane - mNearPlane);

	return projection;
}

Mat4 CameraComponent::GetOrthoMatrix(float left, float right, float bottom, float top) const
{
	Mat4 projection = Mat4::Identity;
	projection.m[0][0] = 2.0f / (right - left);
	projection.m[1][1] = 2.0f / (top - bottom);
	projection.m[2][2] = -1.0f;

	projection.m[3][0] = -(right + left) / (right - left);
	projection.m[3][1] = -(top + bottom) / (top - bottom);
	projection.m[3][2] = 0.0f;

	return projection;
}

Vec3 CameraComponent::GetCameraPosition() const
{
	return mPositionOffset + mOwner->GetPosition();
}

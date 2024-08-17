#include "CameraComponent.h"
#include "../Core/Actor.h"

CameraComponent::CameraComponent(Actor* owner)
	: Component(owner),
	m_PositionOffset(Vector3(0.0f, 1.8f, 0.0f)),
	m_Rotation(Vector3(0.0f, 0.0f, 0.0f)),
	m_HorFOV(80.0f),
	m_NearPlane(0.03f),
	m_FarPlane(10000.0f)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(float deltaTime)
{
}

void CameraComponent::ProcessInput(const Uint8* keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
	m_Rotation.mY += mouseDeltaX * mouseSens;
	m_Rotation.mX += mouseDeltaY * mouseSens;

	m_Rotation.mY = (m_Rotation.mY > 360.0f || m_Rotation.mY < -360.0f) ? 0.0f : m_Rotation.mY;
	m_Rotation.mX = Math::Clamp(m_Rotation.mX, -89.0f, 89.0f);
}

Mat4 CameraComponent::GetViewMatrix() const
{
	Vec3 Front = GetFrontVector();
	Vec3 Right = GetRightVector();
	Vec3 Up = Right.Cross(Front).normalized();
	Vec3 pos = GetOwner()->GetPosition() + m_PositionOffset;

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

	float tanHalfFOVy = tan(DEG2RAD * m_HorFOV / 2.0f);
	projection.m[0][0] = 1.0f / (screenRatio * tanHalfFOVy);
	projection.m[1][1] = 1.0f / tanHalfFOVy;
	projection.m[2][2] = -(m_FarPlane + m_NearPlane) / (m_FarPlane - m_NearPlane);
	projection.m[2][3] = -1.0f;
	projection.m[3][2] = -(2.0f * m_FarPlane * m_NearPlane) / (m_FarPlane - m_NearPlane);

	return projection;
}

Vec3 CameraComponent::GetCameraPosition() const
{
	return m_PositionOffset + mOwner->GetPosition();
}

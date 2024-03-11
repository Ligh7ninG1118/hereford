#include "CameraComponent.h"
#include "../Core/Actor.h"

CameraComponent::CameraComponent(Actor* owner)
	: Component(owner),
	m_PositionOffset(Vector3(0.0f, 1.8f, 0.0f)),
	m_Rotation(Vector3(0.0f, 0.0f, 0.0f)),
	m_ScreenRatio(1.7778f),
	m_HorFOV(80.0f),
	m_NearPlane(0.1f),
	m_FarPlane(100.0f)
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

glm::mat4 CameraComponent::GetViewMatrix() const
{
	Vec3 Front = GetFrontVector();
	Vec3 Right = GetRightVector();
	Vec3 Up = Right.Cross(Front).normalized();

	glm::mat4 Mrot, Mtsl;

	Mrot[0][0] = Right.mX;	Mrot[1][0] = Right.mY;	Mrot[2][0] = Right.mZ;	Mrot[3][0] = 0.0f;
	Mrot[0][1] = Up.mX;		Mrot[1][1] = Up.mY;		Mrot[2][1] = Up.mZ;		Mrot[3][1] = 0.0f;
	Mrot[0][2] = -Front.mX;	Mrot[1][2] = -Front.mY;	Mrot[2][2] = -Front.mZ;	Mrot[3][2] = 0.0f;
	Mrot[0][3] = 0.0f;		Mrot[1][3] = 0.0f;		Mrot[2][3] = 0.0f;		Mrot[3][3] = 1.0f;

	Vec3 pos = GetOwner()->GetPosition() + m_PositionOffset;

	glm::vec3 Position = glm::vec3(pos.mX, pos.mY,pos.mZ);

	Mtsl = glm::mat4(1.0f);
	Mtsl = glm::translate(Mtsl, -Position);

	return Mrot * Mtsl;
}

glm::mat4 CameraComponent::GetPerspMatrix() const
{
	glm::mat4 projection = glm::perspective(glm::radians(m_HorFOV), m_ScreenRatio, m_NearPlane, m_FarPlane);

	return projection;
}

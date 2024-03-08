#include "CameraComponent.h"
#include "../Core/Actor.h"

CameraComponent::CameraComponent(Actor* owner)
	: Component(owner),
	m_PositionOffset(Vector3(0.0f, 1.8f, 0.0f)),
	m_Rotation(Vector3::Zero),
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

void CameraComponent::ProcessInput(const Uint8* keyState)
{
}

glm::mat4 CameraComponent::GetViewMatrix() const
{
	glm::vec3 front;

	float Yaw, Pitch;
	Yaw = m_Rotation.mY;
	Pitch = m_Rotation.mX;

	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	glm::vec3 Front = glm::normalize(front);

	glm::vec3 Right = glm::normalize(glm::cross(Front, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 Up = glm::normalize(glm::cross(Right, Front));

	glm::mat4 Mrot, Mtsl;

	Mrot[0][0] = Right.x;	Mrot[1][0] = Right.y;	Mrot[2][0] = Right.z;	Mrot[3][0] = 0.0f;
	Mrot[0][1] = Up.x;		Mrot[1][1] = Up.y;		Mrot[2][1] = Up.z;		Mrot[3][1] = 0.0f;
	Mrot[0][2] = -Front.x;	Mrot[1][2] = -Front.y;	Mrot[2][2] = -Front.z;	Mrot[3][2] = 0.0f;
	Mrot[0][3] = 0.0f;		Mrot[1][3] = 0.0f;		Mrot[2][3] = 0.0f;		Mrot[3][3] = 1.0f;

	Vector3 pos = GetOwner()->GetPosition() + m_PositionOffset;

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

void CameraComponent::UpdateCameraVectors()
{
}

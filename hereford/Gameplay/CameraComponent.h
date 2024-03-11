#pragma once

#include "../Core/Component.h"
#include "../Math/Math.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner);
	~CameraComponent();

	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY) override;

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetPerspMatrix() const;
	Vector3 GetFrontVector() const;
	Vector3 GetRightVector() const;

	float GetMouseSens() const { return mouseSens; }

private:
	void UpdateCameraVectors();

	Vector3 m_PositionOffset;
	Vector3 m_Rotation;
	float m_ScreenRatio;
	float m_HorFOV;
	float m_NearPlane;
	float m_FarPlane;
	float mouseSens = 0.15f;
};
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 80.0f;


class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		glm::mat4 Mrot, Mtsl;

		Mrot[0][0] = Right.x;	Mrot[1][0] = Right.y;	Mrot[2][0] = Right.z;	Mrot[3][0] = 0.0f;
		Mrot[0][1] = Up.x;		Mrot[1][1] = Up.y;		Mrot[2][1] = Up.z;		Mrot[3][1] = 0.0f;
		Mrot[0][2] = -Front.x;	Mrot[1][2] = -Front.y;	Mrot[2][2] = -Front.z;	Mrot[3][2] = 0.0f;
		Mrot[0][3] = 0.0f;		Mrot[1][3] = 0.0f;		Mrot[2][3] = 0.0f;		Mrot[3][3] = 1.0f;

		Mtsl = glm::mat4(1.0f);
		Mtsl = glm::translate(Mtsl, -Position);

		return Mrot * Mtsl;

		//return glm::lookAt(Position, Position + Front, Up);

		//return CalculateLookAtMatrix(Position, Position + Front, WorldUp);
	}

	void ProcessKeyboard(CameraMovement direction, float deltaTime, bool fpsCamera = false)
	{
		glm::vec3 tempFront = Front;
		if (fpsCamera == true)
			tempFront.y = 0.0f;

		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += tempFront * velocity;
		if (direction == BACKWARD)
			Position -= tempFront * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		UpdateCameraVectors();
	}

	void ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > ZOOM)
			Zoom = ZOOM;
	}


private:
	void UpdateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	glm::mat4 CalculateLookAtMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
	{
		glm::vec3 dir = glm::normalize(position - target);
		glm::vec3 right = glm::normalize(glm::cross(glm::normalize(worldUp), dir));
		glm::vec3 up = glm::cross(dir, right);

		glm::mat4 Mrot, Mtsl;

		Mrot[0][0] = right.x;	Mrot[1][0] = right.y;	Mrot[2][0] = right.z;	Mrot[3][0] = 0.0f;
		Mrot[0][1] = up.x;		Mrot[1][1] = up.y;		Mrot[2][1] = up.z;		Mrot[3][1] = 0.0f;
		Mrot[0][2] = dir.x;		Mrot[1][2] = dir.y;		Mrot[2][2] = dir.z;		Mrot[3][2] = 0.0f;
		Mrot[0][3] = 0.0f;		Mrot[1][3] = 0.0f;		Mrot[2][3] = 0.0f;		Mrot[3][3] = 1.0f;

		Mtsl = glm::mat4(1.0f);
		Mtsl = glm::translate(Mtsl, -position);

		return Mrot * Mtsl;
	}
};
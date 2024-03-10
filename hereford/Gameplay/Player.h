#pragma once
#include "../Core/Actor.h"


class Player : public Actor
{
public:
	Player(class GameContext* gameCtx);
	~Player();

	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const Uint8* keyState) override;

	class CameraComponent* GetMainCamera() const { return m_pCameraComponent; }

private:
	void ProcessMovement(const float& deltaTime);


	class CameraComponent* m_pCameraComponent;

	bool hasMovementInput = false;
	Vector3 inputMoveDir;

	int lastMouseX;
	int lastMouseY;
	Vector3 currentVelocity = Vector3::Zero;


	const float topMovementSpeed = 2.5f;
	const float accelerationSpeed = 15.0f;
	const float decelerationSpeed = 20.0f;
	const float minVelocityOffset = 0.01f;
};


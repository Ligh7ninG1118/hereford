#pragma once
#include "../Core/Actor.h"


enum class MoveDir
{
	None,
	Forward,
	Back,
	Left,
	Right,
};

class Player : public Actor
{
public:
	Player(class GameContext* gameCtx);
	~Player();

	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const Uint8* keyState) override;

	class CameraComponent* GetMainCamera() const { return m_pCameraComponent; }

private:
	class CameraComponent* m_pCameraComponent;

	MoveDir mMoveDir;
	int lastMouseX;
	int lastMouseY;
	float movementSpeed = 2.5f;
};


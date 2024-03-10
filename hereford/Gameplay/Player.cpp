#include "Player.h"
#include "CameraComponent.h"
#include "InputComponent.h"
#include <SDL2/SDL.h>

#include "stdio.h"

Player::Player(GameContext* gameCtx)
	:
	Actor(gameCtx),
	lastMouseX(0),
	lastMouseY(0)
{
	m_pCameraComponent = new CameraComponent(static_cast<Actor*>(this));
	
	//SetPosition(Vector3(0.0f, 0.0f, 3.0f));
}

Player::~Player()
{
}

void Player::OnUpdate(float deltaTime)
{
	ProcessMovement(deltaTime);
}



void Player::OnProcessInput(const Uint8* keyState)
{
	inputMoveDir = Vector3::Zero;
	hasMovementInput = false;

	if (keyState[SDL_SCANCODE_W])
	{
		inputMoveDir += m_pCameraComponent->GetFrontVector();
		hasMovementInput = true;
	}
	if (keyState[SDL_SCANCODE_S])
	{
		inputMoveDir -= m_pCameraComponent->GetFrontVector();
		hasMovementInput = true;
	}
	if (keyState[SDL_SCANCODE_A])
	{
		inputMoveDir -= m_pCameraComponent->GetRightVector();
		hasMovementInput = true;
	}
	if (keyState[SDL_SCANCODE_D])
	{
		inputMoveDir += m_pCameraComponent->GetRightVector();
		hasMovementInput = true;
	}

	inputMoveDir.mY = 0.0f;
	inputMoveDir.Normalize();

	int currentMouseX, currentMouseY, deltaX, deltaY;
	SDL_GetMouseState(&currentMouseX, &currentMouseY);
	deltaX = currentMouseX - lastMouseX;
	deltaY = currentMouseY - lastMouseY;
	//printf("Mouse Input: %d %d\n", deltaX, deltaY);

	lastMouseX = currentMouseX;
	lastMouseY = currentMouseY;

	m_pCameraComponent->ProcessMouseInput(deltaX, -deltaY);
}

void Player::ProcessMovement(float deltaTime)
{
	Vector3 updatedPos = GetPosition();
	updatedPos += currentVelocity * deltaTime;
	SetPosition(updatedPos);

	if (hasMovementInput)
	{
		currentVelocity += inputMoveDir * accelerationSpeed * deltaTime;
	}
	else // In case of no-gravity space combat, comment this part
	{
		currentVelocity -= currentVelocity.normalized() * decelerationSpeed * deltaTime;
	}

	if (currentVelocity.SqrMagnitude() > topMovementSpeed * topMovementSpeed)
	{
		currentVelocity = currentVelocity.normalized() * topMovementSpeed;
	}
}

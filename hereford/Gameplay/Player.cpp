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
	Vector3 pos = GetPosition();
	Vector3 moveDir = Vector3::Zero;
	switch (mMoveDir)
	{
	case MoveDir::None:
		break;
	case MoveDir::Forward:
		moveDir = m_pCameraComponent->GetFrontVector();
		break;
	case MoveDir::Back:
		moveDir = Vector3::Zero - m_pCameraComponent->GetFrontVector();
		break;
	case MoveDir::Left:
		moveDir = Vector3::Zero - m_pCameraComponent->GetRightVector();
		break;
	case MoveDir::Right:
		moveDir = m_pCameraComponent->GetRightVector();
		break;
	default:
		break;
	}

	moveDir.mY = 0.0f;
	moveDir.Normalize();

	pos += moveDir * movementSpeed * deltaTime;

	SetPosition(pos);

}

void Player::OnProcessInput(const Uint8* keyState)
{
	if (keyState[SDL_SCANCODE_W])
		mMoveDir = MoveDir::Forward;
	else if (keyState[SDL_SCANCODE_A])
		mMoveDir = MoveDir::Left;
	else if (keyState[SDL_SCANCODE_S])
		mMoveDir = MoveDir::Back;
	else if (keyState[SDL_SCANCODE_D])
		mMoveDir = MoveDir::Right;
	else
		mMoveDir = MoveDir::None;

	int currentMouseX, currentMouseY, deltaX, deltaY;
	SDL_GetMouseState(&currentMouseX, &currentMouseY);
	deltaX = currentMouseX - lastMouseX;
	deltaY = currentMouseY - lastMouseY;
	//printf("Mouse Input: %d %d\n", deltaX, deltaY);

	lastMouseX = currentMouseX;
	lastMouseY = currentMouseY;

	m_pCameraComponent->ProcessMouseInput(deltaX, -deltaY);
}

#include "Player.h"
#include "CameraComponent.h"
#include "InputComponent.h"
#include <SDL2/SDL.h>

#include "stdio.h"

Player::Player(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	m_pCameraComponent = new CameraComponent(static_cast<Actor*>(this));
}

Player::~Player()
{
}

void Player::OnUpdate(float deltaTime)
{
	Vector3 pos = GetPosition();

	printf("Player pos: %f %f %f\n", pos.mX, pos.mY, pos.mZ);

	printf("moveDir: %d\n", mMoveDir);
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
}

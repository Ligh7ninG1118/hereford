#include "FlyCamera.h"
#include "Gameplay/CameraComponent.h"
#include <SDL2/SDL.h>


FlyCamera::FlyCamera(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	mPtrCameraComp = std::make_unique<CameraComponent>(static_cast<Actor*>(this));

}

FlyCamera::~FlyCamera()
{
}

void FlyCamera::OnUpdate(float deltaTime)
{
	ProcessMovement(deltaTime);
}

void FlyCamera::OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
	inputMoveDir = Vector3::Zero;
	hasMovementInput = false;

	if (keyState[SDL_SCANCODE_W] == EInputState::KEY_HOLD)
	{
		inputMoveDir += GetForward();
		hasMovementInput = true;
	}
	if (keyState[SDL_SCANCODE_S] == EInputState::KEY_HOLD)
	{
		inputMoveDir -= GetForward();
		hasMovementInput = true;
	}
	if (keyState[SDL_SCANCODE_A] == EInputState::KEY_HOLD)
	{
		// Cross with World Up to get Right vector;
		inputMoveDir -= GetForward().Cross(Vector3(0.0f, 1.0f, 0.0f));
		hasMovementInput = true;
	}
	if (keyState[SDL_SCANCODE_D] == EInputState::KEY_HOLD)
	{
		inputMoveDir += GetForward().Cross(Vector3(0.0f, 1.0f, 0.0f));
		hasMovementInput = true;
	}
	if (keyState[SDL_SCANCODE_Q] == EInputState::KEY_HOLD)
	{
		inputMoveDir -= Vector3(0.0f, 1.0f, 0.0f);
		hasMovementInput = true;
	}
	if (keyState[SDL_SCANCODE_E] == EInputState::KEY_HOLD)
	{
		inputMoveDir += Vector3(0.0f, 1.0f, 0.0f);
		hasMovementInput = true;
	}

	inputMoveDir.Normalize();
	
}

void FlyCamera::ProcessMovement(float deltaTime)
{
	Vector3 updatedPos = GetPosition();
	updatedPos += inputMoveDir * 5.0f * deltaTime;
	SetPosition(updatedPos);
}

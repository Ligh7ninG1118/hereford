#include "Gameplay/Player.h"
#include "Gameplay/CameraComponent.h"
#include "Gameplay/InputComponent.h"
#include "Core/GameContext.h"
#include <SDL2/SDL.h>

#include "stdio.h"

Player::Player(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	m_pCameraComponent = std::make_unique<CameraComponent>(static_cast<Actor*>(this));
}

Player::~Player()
{
}

void Player::OnUpdate(float deltaTime)
{
	ProcessMovement(deltaTime);
}



void Player::OnProcessInput(const Uint8* keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
	// Horizontal Movement
	{
		inputMoveDir = Vector3::Zero;
		hasMovementInput = false;

		if (keyState[SDL_SCANCODE_W])
		{
			inputMoveDir += GetForward();
			hasMovementInput = true;
		}
		if (keyState[SDL_SCANCODE_S])
		{
			inputMoveDir -= GetForward();
			hasMovementInput = true;
		}
		if (keyState[SDL_SCANCODE_A])
		{
			// Cross with World Up to get Right vector;
			inputMoveDir -= GetForward().Cross(Vector3(0.0f, 1.0f, 0.0f));
			hasMovementInput = true;
		}
		if (keyState[SDL_SCANCODE_D])
		{
			inputMoveDir += GetForward().Cross(Vector3(0.0f, 1.0f, 0.0f));
			hasMovementInput = true;
		}
		if (keyState[SDL_SCANCODE_Q])
		{
			inputMoveDir -= Vector3(0.0f, 1.0f, 0.0f);
			hasMovementInput = true;
		}
		if (keyState[SDL_SCANCODE_E])
		{
			inputMoveDir += Vector3(0.0f, 1.0f, 0.0f);
			hasMovementInput = true;
		}

		//inputMoveDir.mY = 0.0f;
		inputMoveDir.Normalize();
	}

	{
		if ((mouseState & SDL_BUTTON_LEFT))
		{
			if (!lmbPressed)
			{
				Vec3 origin = GetPosition() + m_pCameraComponent->GetPositionOffset();
				Vec3 dir = m_pCameraComponent->GetFrontVector();
				HitInfo hitInfo;
				if (mGame->GetPhysicsManager().Raycast(origin, dir, 1000.0f, hitInfo))
				{
					if (hitInfo.hitActor != nullptr)
						hitInfo.hitActor->SetState(ActorState::Destroy);
				}
				lmbPressed = true;
			}
		}
		else
		{
			lmbPressed = false;
		}
	}

	// Rotation
	{
		mRotation.mY += mouseDeltaX * m_pCameraComponent->GetMouseSens();
		if (mRotation.mY > 360.0f)
			mRotation.mY = 0.0f;
		if (mRotation.mY < -360.0f)
			mRotation.mY = 0.0f;
	}
}

void Player::ProcessMovement(const float& deltaTime)
{
	// Update Position
	Vector3 updatedPos = GetPosition();
	updatedPos += currentVelocity * deltaTime;
	SetPosition(updatedPos);

	// Update Velocity
	if (hasMovementInput)
	{
		currentVelocity += inputMoveDir * accelerationSpeed * deltaTime;
	}
	else // In case of no-gravity space combat, comment this part
	{
		currentVelocity -= currentVelocity.normalized() * decelerationSpeed * deltaTime;
	}

	// Cap velocity at top speed
	if (currentVelocity.SqrMagnitude() > topMovementSpeed * topMovementSpeed)
	{
		currentVelocity = currentVelocity.normalized() * topMovementSpeed;
	}

	// Prevent drifting
	if (currentVelocity.SqrMagnitude() < minVelocityOffset)
	{
		currentVelocity = Vector3::Zero;
	}
}

#include "FlyCamera.h"
#include "Gameplay/CameraComponent.h"
#include "Core/GameContext.h"
#include "Input/InputManager.h"
#include <SDL2/SDL.h>


FlyCamera::FlyCamera(GameContext* gameCtx)
	:
	Actor(gameCtx)
{
	mPtrCameraComp = std::make_unique<CameraComponent>(static_cast<Actor*>(this));

	hInteractInput = gameCtx->GetInputManager().Subscribe(EInputAction::TEST_INPUT, std::bind(&FlyCamera::OnInteracInput, this, std::placeholders::_1));
}

FlyCamera::~FlyCamera()
{
	GetGameContext()->GetInputManager().Unsubscribe(EInputAction::TEST_INPUT, hInteractInput);
}

void FlyCamera::OnUpdate(float deltaTime)
{
	ProcessMovement(deltaTime);
}

void FlyCamera::ProcessMovement(float deltaTime)
{
	auto& inputMgr = GetGameContext()->GetInputManager();
	Vec3 rawInputDir = inputMgr.ReadValue<Vec3>(EInputAction::FLY_MOVEMENT);
	rawInputDir.Normalize();

	Vec3 moveDir = rawInputDir.mX * GetForward()
		+ rawInputDir.mY * Vec3::Up
		+ rawInputDir.mZ * (GetForward().Cross(Vec3::Up));

	Vector3 updatedPos = GetPosition();
	updatedPos += moveDir * 5.0f * deltaTime;
	SetPosition(updatedPos);
}

void FlyCamera::OnInteracInput(EInputS state)
{
	switch (state)
	{
	case EInputS::IDLE:
		break;
	case EInputS::PRESSED:
		printf("F pressed.\n");
		break;
	case EInputS::RELEASED:
		printf("F released.\n");
		break;
	case EInputS::HOLD:
		printf("F held.\n");
		break;
	default:
		break;
	}

}

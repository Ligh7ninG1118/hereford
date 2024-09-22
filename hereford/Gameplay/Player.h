#pragma once
#include "../Core/Actor.h"
#include "Util/DelayedAction.h"
#include "Util/GameEvent.h"
#include "Util/Reflection.h"
#include <memory>


class Player : public Actor
{
	REFLECT(Player)

public:
	Player(class GameContext* gameCtx);
	~Player();

	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY) override;

	class CameraComponent& GetMainCamera() const { return *mPtrCameraComp.get(); }

	class ActionComponent* GetActionComp() const { return mPtrActionComp.get(); }

private:
	void ProcessMovement(const float& deltaTime);
	void ShowDebugInfo();
	void WeaponFiredEventListener(EventOnPlayerWeaponFired inEvent);

	std::unique_ptr<class CameraComponent> mPtrCameraComp;
	std::unique_ptr<class AnimatedRenderComponent> mPtrAnimRenderComp;
	std::shared_ptr<class AnimationStateMachine> mPtrAnimStateMachine;

	std::unique_ptr<class ActionComponent> mPtrActionComp;

	class WeaponComponent* mPtrActiveWeaponComp;
	class UIAmmoIndicator* mPtrUIAmmo;
	class UICrosshair* mPtrUICrosshair;


	bool hasMovementInput = false;
	Vector3 inputMoveDir;

	Vector3 currentVelocity = Vector3::Zero;

	bool lmbPressed = false;

	const float topWalkingSpeed = 2.5f;
	const float topSprintingSpeed = 5.0f;
	const float accelerationSpeed = 15.0f;
	const float decelerationSpeed = 20.0f;
	const float minVelocityOffset = 0.01f;

	GameEvent::Subscription<EventOnPlayerWeaponFired>* mPtrWeaponFiredEvent;
};


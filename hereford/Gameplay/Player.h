#pragma once
#include "../Core/Actor.h"
#include "Util/DelayedAction.h"
#include "Util/GameEvent.h"
#include <memory>


class Player : public Actor
{
public:
	Player(class GameContext* gameCtx);
	~Player();

	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY) override;

	class CameraComponent& GetMainCamera() const { return *m_pCameraComponent.get(); }

private:
	void ProcessMovement(const float& deltaTime);

	void WeaponFiredEventListener(EventOnPlayerWeaponFired inEvent);

	std::unique_ptr<class CameraComponent> m_pCameraComponent;
	std::unique_ptr<class AnimatedRenderComponent> mPtrAnimRenderComp;
	std::shared_ptr<class AnimationStateMachine> mPtrAnimStateMachine;
	class WeaponComponent* mPtrActiveWeaponComp;

	std::shared_ptr<Model> gunModel;
	class Animator* gunAnimator;
	class AnimationStateMachine* gunASM;

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


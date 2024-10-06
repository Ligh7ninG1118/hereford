#pragma once
#include "../Core/Actor.h"
#include "Util/DelayedAction.h"
#include "Util/GameEvent.h"
#include "Util/Reflection.h"
#include "Util/TimelineAction.h"
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

	void SetArmRotateOffset(Vec3 offset);
	void SetArmTranslateOffset(Vec3 offset);

private:
	void ProcessMovement(const float& deltaTime);
	void ShowDebugInfo();
	void WeaponFiredEventListener(EventOnPlayerWeaponFired inEvent);

	void CrouchTimeline(float alpha);

	std::unique_ptr<class CameraComponent> mPtrCameraComp;
	std::unique_ptr<class AnimatedRenderComponent> mPtrAnimRenderComp;
	std::shared_ptr<class AnimationStateMachine> mPtrAnimStateMachine;

	std::unique_ptr<class ActionComponent> mPtrActionComp;

	class WeaponComponent* mPtrActiveWeaponComp;
	class UIAmmoIndicator* mPtrUIAmmo;
	class UICrosshair* mPtrUICrosshair;

	TAHandle mHCrouchTimeline;


	bool hasMovementInput = false;
	Vector3 inputMoveDir;
	Vector3 currentVelocity = Vector3::Zero;

	bool lmbPressed = false;
	float totalRuntime;

	float currentTopSpeed;
	const float topWalkingSpeed = 3.5f;
	const float topCrouchSpeed = 1.5f;
	const float topSprintingSpeed = 6.0f;
	const float maxSpeedChangingRate = 25.0f;
	const float speedOffset = 0.01f;


	const Vec3 hipArmTranslationOffset = Vec3(-0.2f, -0.4f, 0.0f);
	const Vec3 hipArmRotationOffset = Vec3(0.0f, 0.0f, 90.0f);

	Vec3 currentArmTranslationOffset;
	Vec3 currentArmRotationOffset;


	GameEvent::Subscription<EventOnPlayerWeaponFired>* mPtrWeaponFiredEvent;
};


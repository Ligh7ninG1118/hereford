#pragma once
#include "../Core/Actor.h"
#include "Input/InputCommons.h"
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

	class CameraComponent& GetMainCamera() const { return *mPtrCameraComp.get(); }

	class ActionComponent* GetActionComp() const { return mPtrActionComp.get(); }

private:
	void ProcessMovement(float deltaTime);
	void OnJumpInput(EInputState state);
	void OnCrouchInput(EInputState state);
	void OnSprintInput(EInputState state);
	void OnWeaponSwapInput(EInputState state);
	void OnInteractInput(EInputState state);

	void ProcessInteractionPrompt();
	void ShowDebugInfo();


	void WeaponSwitchCallback();
	void CrouchTimeline(float alpha);

	std::unique_ptr<class CameraComponent> mPtrCameraComp;
	std::unique_ptr<class AudioComponent> mPtrAudioComponent;
	std::unique_ptr<class ActionComponent> mPtrActionComp;

	class InputManager* mInputMgr;
	hInputSub hJumpSub;
	hInputSub hCrouchSub;
	hInputSub hSprintSub;
	hInputSub hInteractSub;
	hInputSub hWeaponSwapUpSub;
	hInputSub hWeaponSwapDownSub;


	class Weapon* mPtrActiveWeapon;
	std::vector<Weapon*> mWeaponList;
	int mCurrentWeaponIndex = 0;

	class UIAmmoIndicator* mPtrUIAmmo;
	class UICrosshair* mPtrUICrosshair;

	TAHandle mHCrouchTimeline;
	DAHandle mHWeaponSwitch;

	class IInteractable* mInteractCandidate;

	Vector3 currentVelocity = Vector3::Zero;

	bool lmbPressed = false;
	float totalRuntime;

	bool mIsGrounded = true;

	float currentTopSpeed;
	const float topWalkingSpeed = 3.5f;
	const float topCrouchSpeed = 1.5f;
	const float topSprintingSpeed = 6.0f;
	const float maxSpeedChangingRate = 25.0f;
	const float speedOffset = 0.01f;

	const float OVERRIDE_GRAVITY_CONSTANT = -20.0f;
	const float JUMP_HEIGHT_CONSTANT = 1.5f;

};


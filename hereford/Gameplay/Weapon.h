#pragma once
#include "Core/Actor.h"
#include <memory>
#include "Util/TimelineAction.h"
#include "Util/DelayedAction.h"
#include "Util/Reflection.h"

class Weapon : public Actor
{
	REFLECT(Weapon)

public:
	Weapon(class GameContext* gameCtx);
	virtual ~Weapon();

	virtual void Init(class Player* playerPtr);

	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY) override;

	void SetArmOffset(Vec3 translationOffset);

protected:
	virtual void Fire();
	virtual void Reload();
	
	virtual void AimingTimeline(float alpha);
	virtual void FinishedReload();

	class Player* mPtrPlayer;

	DAHandle mHReloadCallback;
	TAHandle mHAimingTimeline;

	Vec3 mHipArmTranslationOffset;
	Vec3 mHipArmRotationOffset;
	Vec3 mADSArmTranslationOffset;

	Vec3 mCurrentArmTranslationOffset;
	Vec3 mCurrentArmRotationOffset;

	int mFireAnimIndex;
	int mReloadAnimIndex;

	std::unique_ptr<class AnimatedRenderComponent> mPtrAnimRenderComp;
	std::shared_ptr<class AnimationStateMachine> mPtrAnimStateMachine;
	std::unique_ptr<class WeaponComponent> mPtrWeaponComp;
	std::unique_ptr<class AudioComponent> mPtrAudioComponent;
};


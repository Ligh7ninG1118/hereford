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

	void SetArmOffset(Vec3 translationOffset);

	void Draw();
	void Holster();

	class WeaponComponent* GetWeaponComponent() const { return mPtrWeaponComp.get(); }

	float mReloadTime;
	float mDrawTime;
	float mHolsterTime;

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
	Vec3 mADSArmRotationOffset;

	Vec3 mCurrentArmTranslationOffset;
	Vec3 mCurrentArmRotationOffset;

	//TODO: Change to state/flag later
	bool mIsADSing;

	int mFireAnimIndex;
	int mReloadAnimIndex;
	int mDrawAnimIndex;
	int mHolsterAnimIndex;

	std::unique_ptr<class AnimatedRenderComponent> mPtrAnimRenderComp;
	std::unique_ptr<class AnimationStateMachine> mPtrAnimStateMachine;
	std::unique_ptr<class WeaponComponent> mPtrWeaponComp;
	std::unique_ptr<class AudioComponent> mPtrAudioComponent;
};


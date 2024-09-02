#pragma once

#include "Core/Actor.h"
#include "Util/DelayedAction.h"

enum class EWeaponState
{
	READY = 0,
	RELOADING,
	SWAPPING
};

class Weapon : public Actor
{
public:
	Weapon(class GameContext* gameCtx, class Animator* animator);
	~Weapon();
	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const Uint8* keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY) override;

	void Fire();
	void Reload();
	void FinishedReload();

	//class RenderComponent* GetRenderComponent() const { return m_pRenderComponent; }
	class WeaponComponent* GetWeaponComponent() const { return mWeaponComponent; }
private:
	//class RenderComponent* m_pRenderComponent;
	class WeaponComponent* mWeaponComponent;
	class Animator* mAnimator;

	EWeaponState mCurrentState;
	DAHandle mReloadAction;
	bool mHasPressedLMB;
};


#pragma once
#include "Core/Actor.h"
#include <memory>


class Weapon : public Actor
{
public:
	Weapon(class GameContext* gameCtx);
	~Weapon();

	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY) override;

protected:
	std::unique_ptr<class AnimatedRenderComponent> mPtrAnimRenderComp;
	std::shared_ptr<class AnimationStateMachine> mPtrAnimStateMachine;
	std::unique_ptr<class WeaponComponent> mPtrWeaponComp;
};


#pragma once

#include "Core/Actor.h"


class Weapon : public Actor
{
public:
	Weapon(class GameContext* gameCtx, class Animator* mAnimator);
	~Weapon();
	void OnUpdate(float deltaTime) override;

	void FireWeapon();
	void ReloadWeapon();

	//class RenderComponent* GetRenderComponent() const { return m_pRenderComponent; }
	class WeaponComponent* GetWeaponComponent() const { return mWeaponComponent; }
private:
	//class RenderComponent* m_pRenderComponent;
	class WeaponComponent* mWeaponComponent;
	class Animator* mAnimator;

};


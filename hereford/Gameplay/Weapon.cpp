#include "Weapon.h"
#include "Animation/Animator.h"
#include "WeaponComponent.h"

Weapon::Weapon(GameContext* gameCtx, Animator* animator)
	:Actor(gameCtx),
	mAnimator(animator)
{
	mWeaponComponent = new WeaponComponent(this);
}

Weapon::~Weapon()
{
}

void Weapon::OnUpdate(float deltaTime)
{
}

void Weapon::FireWeapon()
{
	mAnimator->PlayAnimation(4, false);
}

void Weapon::ReloadWeapon()
{
	mAnimator->PlayAnimation(3, false);
}

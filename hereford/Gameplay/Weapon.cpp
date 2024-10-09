#include "Weapon.h"
#include "Gameplay/WeaponComponent.h"



Weapon::Weapon(GameContext* gameCtx)
	: Actor(gameCtx)
{
	
}

Weapon::~Weapon()
{
}

void Weapon::OnUpdate(float deltaTime)
{
}

void Weapon::OnProcessInput(const std::vector<EInputState>& keyState, Uint32 mouseState, int mouseDeltaX, int mouseDeltaY)
{
}

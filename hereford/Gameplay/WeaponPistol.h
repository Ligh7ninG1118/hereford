#pragma once
#include "Weapon.h"

class WeaponPistol : public Weapon
{
	REFLECT(WeaponPistol)

public:
	WeaponPistol(class GameContext* gameCtx);
	~WeaponPistol();
	virtual void Init(class Player* playerPtr) override;

};


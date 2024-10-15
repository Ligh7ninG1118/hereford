#pragma once
#include "Weapon.h"

class WeaponSMG : public Weapon
{
	REFLECT(WeaponSMG)

public:
	WeaponSMG(class GameContext* gameCtx);
	~WeaponSMG();
	virtual void Init(class Player* playerPtr) override;
};


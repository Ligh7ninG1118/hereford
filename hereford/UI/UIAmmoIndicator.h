#pragma once
#include "UIImage.h"
class UIAmmoIndicator : public UIImage
{
public:
	UIAmmoIndicator(std::weak_ptr<class Renderer> inPtrRenderer, std::weak_ptr<class Weapon> inPtrWeapon);
	~UIAmmoIndicator();

	void UpdateContent() override;

protected:
	uint16 mCurrentMax;
	
	std::weak_ptr<class Weapon> mPtrWeapon;

};


#pragma once
#include "UIImage.h"
#include "Util/GameEvent.h"

class UIAmmoIndicator : public UIImage
{
public:
	UIAmmoIndicator(class Renderer* inPtrRenderer, class Shader* inPtrShader, std::shared_ptr<class Texture> inPtrUITex);
	~UIAmmoIndicator();

	void Initialize(class WeaponComponent* inPtrWeaponComp);
	void UpdateAmmoIndicatorEventHandler(EventOnWeaponAmmoChanged inEvent);
	void UpdateAmmoIndicator(int ammoLeft);
	void UpdateChamberIndicator(int ammoLeft);

protected:
	uint16 mCurrentMax;
	
	class WeaponComponent* mPtrWeaponComp;
	class GameEvent::Subscription<EventOnWeaponAmmoChanged>* mWeaponAmmoChangedEvent;
	class UIOneInTheChamber* mChamberIndicator;

};


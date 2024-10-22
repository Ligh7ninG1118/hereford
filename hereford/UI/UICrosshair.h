#pragma once
#include "UIImage.h"
#include "Util/GameEvent.h"

class UICrosshair : public UIElement
{
public:
	UICrosshair(class Renderer* inPtrRenderer, class Shader* inPtrShader);
	~UICrosshair();

	void Initialize(class WeaponComponent* inPtrWeaponComp);
	void UpdateCrosshair(EventOnWeaponAiming inEvent);

protected:
	class WeaponComponent* mPtrWeaponComp;

	GameEvent::Subscription<EventOnWeaponAiming>* mWeaponAimingEvent;
};


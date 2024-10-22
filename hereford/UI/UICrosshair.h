#pragma once
#include "UIImage.h"
#include "Util/GameEvent.h"

class UICrosshair : public UIElement
{
public:
	UICrosshair(class Renderer* inPtrRenderer, class Shader* inPtrShader);
	~UICrosshair();

	void Initialize(class WeaponComponent* inPtrWeaponComp);
	void UpdateCrosshairEventHandler(EventOnWeaponAiming inEvent);
	void UpdateCrosshair(float progress);

protected:
	Vec3 mCrosshairColor;
	Vec3 mOutlineColor;
	float mCrosshairTransparency;

	class WeaponComponent* mPtrWeaponComp;

	class GameEvent::Subscription<EventOnWeaponAiming>* mWeaponAimingEvent;
};


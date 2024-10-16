#pragma once
#include "UIImage.h"
class UICrosshair : public UIElement
{
public:
	UICrosshair(class Renderer* inPtrRenderer, class Shader* inPtrShader);
	~UICrosshair();

	void Initialize(class WeaponComponent* inPtrWeaponComp);
	void UpdateContent() override;

protected:
	class WeaponComponent* mPtrWeaponComp;
};


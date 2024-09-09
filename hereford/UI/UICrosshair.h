#pragma once
#include "UIImage.h"
class UICrosshair : public UIElement
{
public:
	UICrosshair(class Renderer* inPtrRenderer, class Shader* inPtrShader, class WeaponComponent* inPtrWeaponComp);
	~UICrosshair();

	void Initialize() override;
	void UpdateContent() override;

protected:
	class WeaponComponent* mPtrWeaponComp;
};


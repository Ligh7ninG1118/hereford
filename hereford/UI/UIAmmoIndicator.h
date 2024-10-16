#pragma once
#include "UIImage.h"
class UIAmmoIndicator : public UIImage
{
public:
	UIAmmoIndicator(class Renderer* inPtrRenderer, class Shader* inPtrShader, std::shared_ptr<class Texture> inPtrUITex);
	~UIAmmoIndicator();

	void Initialize(class WeaponComponent* inPtrWeaponComp);
	void UpdateContent() override;

protected:
	uint16 mCurrentMax;
	
	class WeaponComponent* mPtrWeaponComp;

};


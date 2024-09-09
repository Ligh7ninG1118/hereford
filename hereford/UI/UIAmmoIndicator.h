#pragma once
#include "UIImage.h"
class UIAmmoIndicator : public UIImage
{
public:
	UIAmmoIndicator(class Renderer* inPtrRenderer, class Shader* inPtrShader, std::shared_ptr<class Texture> inPtrUITex, class WeaponComponent* inPtrWeaponComp);
	~UIAmmoIndicator();

	void Initialize() override;
	void UpdateContent() override;

protected:
	uint16 mCurrentMax;
	
	class WeaponComponent* mPtrWeaponComp;

};

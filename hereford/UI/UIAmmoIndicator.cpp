#include "UIAmmoIndicator.h"
#include "Asset/Shader.h"
#include "Gameplay/Weapon.h"
#include "Gameplay/WeaponComponent.h"

UIAmmoIndicator::UIAmmoIndicator(std::weak_ptr<Renderer> inPtrRenderer, std::weak_ptr<Weapon> inPtrWeapon)
	: UIImage(inPtrRenderer),
	mPtrWeapon(inPtrWeapon),
	mCurrentMax(0)
{
}

UIAmmoIndicator::~UIAmmoIndicator()
{
}

void UIAmmoIndicator::UpdateContent()
{
	auto lock = mPtrWeapon.lock();
	if (!lock)
	{
		printf("UIAmmoIndicator::UpdateContent(): Referenced Weapon obj destroyed\n");
		return;
	}


	uint16 maxMag = lock->GetWeaponComponent()->GetMaxMagazineCapacity();
		
	//TODO: Wrong! Since we r holding ptr to a single weapon instance, its magazine capacity wont change
	//		If it indeed was changed, then it means a diff weapon instance, should do this in SetPtrWeapon or somewhere
	if (maxMag != mCurrentMax)
	{
		mCurrentMax = maxMag;
		UpdateGLAsset();
	}

	uint16 currentMag = lock->GetWeaponComponent()->GetCurrentMagazineAmmo();

	mPtrShader->Use();
	float leftX = mPosition.mX - mDimension.mX * mAlignment.mX * mScale.mX;

	//TODO: Calculate width
	float threshold = leftX + (mCurrentMax - currentMag) * 16.0f;	

	mPtrShader->SetFloat("threshold", threshold);
}

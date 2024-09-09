#include "UIAmmoIndicator.h"
#include "Asset/Shader.h"
#include "Asset/Texture.h"
#include "Graphics/Renderer.h"
#include "Gameplay/WeaponComponent.h"

UIAmmoIndicator::UIAmmoIndicator(Renderer* inPtrRenderer, Shader* inPtrShader, std::shared_ptr<Texture> inPtrUITex, WeaponComponent* inPtrWeaponComp)
	: UIImage(inPtrRenderer, inPtrShader, inPtrUITex),
	mPtrWeaponComp(inPtrWeaponComp),
	mCurrentMax(0)
{
}

UIAmmoIndicator::~UIAmmoIndicator()
{
}

void UIAmmoIndicator::Initialize()
{
	mCurrentMax = mPtrWeaponComp->GetMaxMagazineCapacity();

	SetAnchor(EUIAnchorPreset::BOTTOM_RIGHT);
	SetPosition(Vec2(-300.0f, 100.0f));
	SetAlignment(Vec2(1.0f, 0.5f));
	SetDimension(Vec2(66.0f, 32.0f));
	SetTiling(Vec2(static_cast<float>(mCurrentMax), 1.0f));

	mPtrShader->Use();
	mPtrShader->SetVec4("uiColor2", 1.0f, 1.0f, 1.0f, 0.9f);
	mPtrShader->SetVec4("uiColor1", 0.2f, 0.2f, 0.2f, 0.4f);
	mPtrShader->SetInt("uiTex", 0);

	UIImage::Initialize();
}

void UIAmmoIndicator::UpdateContent()
{	
	//TODO: Wrong! Since we r holding ptr to a single weapon instance, its magazine capacity wont change
	//		If it indeed was changed, then it means a diff weapon instance, should do this in SetPtrWeapon or somewhere
	
	uint16 currentMag = mPtrWeaponComp->GetCurrentMagazineAmmo();

	Vec2 screenDimension;
	Vec2 actualPos = mPosition;
	screenDimension = mPtrRenderer->GetScreenDimension();

	switch (mAnchor)
	{
	case EUIAnchorPreset::BOTTOM_MID:
	case EUIAnchorPreset::CENTER_MID:
	case EUIAnchorPreset::TOP_MID:
		actualPos.mX += screenDimension.mX / 2.0f;
		break;
	case EUIAnchorPreset::BOTTOM_RIGHT:
	case EUIAnchorPreset::CENTER_RIGHT:
	case EUIAnchorPreset::TOP_RIGHT:
		actualPos.mX += screenDimension.mX;
		break;
	default:
		break;
	}


	float leftX = actualPos.mX - mDimension.mX * mAlignment.mX * mScale.mX;

	//Okay...But how do we deal with one in the chamber sceneario?
	float threshold = leftX + (mCurrentMax - currentMag) * GetDimension().mX / static_cast<float>(mCurrentMax);	

	mPtrShader->Use();
	mPtrShader->SetFloat("threshold", threshold);

	UIElement::UpdateContent();
}

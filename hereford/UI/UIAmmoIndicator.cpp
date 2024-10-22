#include "UIAmmoIndicator.h"
#include "UIOneInTheChamber.h"
#include "Asset/Shader.h"
#include "Asset/Texture.h"
#include "Asset/AssetManager.h"
#include "Graphics/Renderer.h"
#include "Gameplay/WeaponComponent.h"

UIAmmoIndicator::UIAmmoIndicator(Renderer* inPtrRenderer, Shader* inPtrShader, std::shared_ptr<Texture> inPtrUITex)
	: UIImage(inPtrRenderer, inPtrShader, inPtrUITex),
	mPtrWeaponComp(nullptr),
	mWeaponAmmoChangedEvent(nullptr),
	mCurrentMax(0)
{
	std::shared_ptr<Shader> chamberShader = AssetManager::LoadAsset<Shader>(std::string("Shaders/ui_image_ammo_count_vert.glsl*Shaders/ui_image_frag.glsl"));

	mChamberIndicator = new UIOneInTheChamber(inPtrRenderer, chamberShader.get(), inPtrUITex);
}

UIAmmoIndicator::~UIAmmoIndicator()
{
	GameEvent::Unsubscribe(mWeaponAmmoChangedEvent);
	delete mWeaponAmmoChangedEvent;
}

void UIAmmoIndicator::Initialize(WeaponComponent* inPtrWeaponComp)
{
	mPtrWeaponComp = inPtrWeaponComp;

	mCurrentMax = mPtrWeaponComp->GetMaxMagazineCapacity();

	SetAnchor(EUIAnchorPreset::BOTTOM_RIGHT);
	SetPosition(Vec2(-300.0f, 100.0f));
	SetAlignment(Vec2(1.0f, 0.5f));
	SetDimension(Vec2(5.5f * mCurrentMax, 32.0f));
	SetTiling(Vec2(static_cast<float>(mCurrentMax), 1.0f));

	mPtrShader->Use();
	mPtrShader->SetVec4("uiColor2", 1.0f, 1.0f, 1.0f, 0.9f);
	mPtrShader->SetVec4("uiColor1", 0.2f, 0.2f, 0.2f, 0.4f);
	mPtrShader->SetInt("uiTex", 0);

	mWeaponAmmoChangedEvent = GameEvent::Subscribe<EventOnWeaponAmmoChanged>(std::bind(&UIAmmoIndicator::UpdateAmmoIndicatorEventHandler, this, std::placeholders::_1));

	UIImage::Initialize();

	float xPosChamberUI = -300.0f - 5.5f * mCurrentMax - 5.0f;
	mChamberIndicator->Initialize(xPosChamberUI);

	UpdateAmmoIndicator(mPtrWeaponComp->GetCurrentMagazineAmmo());
}

void UIAmmoIndicator::UpdateAmmoIndicatorEventHandler(EventOnWeaponAmmoChanged inEvent)
{	
	UpdateAmmoIndicator(inEvent.ammoLeft);
}

void UIAmmoIndicator::UpdateAmmoIndicator(int ammoLeft)
{
	uint16 currentMag = ammoLeft;

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

	float threshold = leftX + (mCurrentMax - currentMag) * GetDimension().mX / static_cast<float>(mCurrentMax);

	mPtrShader->Use();
	mPtrShader->SetVec4("uiColor2", 1.0f, 1.0f, 1.0f, 0.9f);
	mPtrShader->SetVec4("uiColor1", 0.2f, 0.2f, 0.2f, 0.4f);
	mPtrShader->SetFloat("threshold", threshold);

	
	UIElement::UpdateContent();

	UpdateChamberIndicator(ammoLeft);
}

void UIAmmoIndicator::UpdateChamberIndicator(int ammoLeft)
{
	if (ammoLeft <= mCurrentMax)
	{
		mChamberIndicator->SetIsActive(false);
	}
	else
	{
		mChamberIndicator->SetIsActive(true);
	}
}



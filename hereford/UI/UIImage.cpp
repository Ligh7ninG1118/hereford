#include "UIImage.h"
#include "Graphics/Renderer.h"

UIImage::UIImage(Renderer* inPtrRenderer, Shader* inPtrShader, std::shared_ptr<Texture> inPtrUITex)
	:
	UIElement(inPtrRenderer, inPtrShader),
	mPtrUITexture(inPtrUITex)
{

}

UIImage::~UIImage()
{
}

std::vector<float> UIImage::GenerateQuad()
{
	Vec2 screenDimension;
	Vec2 actualPos = mPosition;
	screenDimension = mPtrRenderer->GetScreenDimension();

	/*auto lock = mPtrRenderer.lock();
	if (lock)
	{
		screenDimension = lock->GetScreenDimension();
	}
	else
	{
		printf("UIElement::GenerateQuad(): Referenced Renderer obj destroyed\n");
		return nullptr;
	}*/

	switch (mAnchor)
	{
	case EUIAnchorPreset::BOTTOM_LEFT:
		//Do nothing
		break;
	case EUIAnchorPreset::BOTTOM_MID:
		actualPos.mX += screenDimension.mX / 2.0f;
		break;
	case EUIAnchorPreset::BOTTOM_RIGHT:
		actualPos.mX += screenDimension.mX;
		break;
	case EUIAnchorPreset::CENTER_LEFT:
		actualPos.mY += screenDimension.mY / 2.0f;
		break;
	case EUIAnchorPreset::CENTER_MID:
		actualPos.mX += screenDimension.mX / 2.0f;
		actualPos.mY += screenDimension.mY / 2.0f;
		break;
	case EUIAnchorPreset::CENTER_RIGHT:
		actualPos.mX += screenDimension.mX;
		actualPos.mY += screenDimension.mY / 2.0f;
		break;
	case EUIAnchorPreset::TOP_LEFT:
		actualPos.mY += screenDimension.mY;
		break;
	case EUIAnchorPreset::TOP_MID:
		actualPos.mX += screenDimension.mX / 2.0f;
		actualPos.mY += screenDimension.mY;
		break;
	case EUIAnchorPreset::TOP_RIGHT:
		actualPos.mX += screenDimension.mX;
		actualPos.mY += screenDimension.mY;
		break;
	}


	float leftX = actualPos.mX - mDimension.mX * mAlignment.mX * mScale.mX;
	float rightX = actualPos.mX + mDimension.mX * (1.0f - mAlignment.mX) * mScale.mX;

	float bottomY = actualPos.mY - mDimension.mY * mAlignment.mY * mScale.mY;
	float topY = actualPos.mY + mDimension.mY * (1.0f - mAlignment.mY) * mScale.mY;

	std::vector<float> quad{
		{	leftX,		topY,		0.0f,			0.0f ,
			leftX,		bottomY,    0.0f,			mTiling.mY ,
			rightX,		bottomY,    mTiling.mX,		mTiling.mY ,

			leftX,		topY,		0.0f,			0.0f ,
			rightX,		bottomY,    mTiling.mX,		mTiling.mY ,
			rightX,		topY,		mTiling.mX,		0.0f }
	};

	return quad;
}

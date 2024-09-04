#include "UIImage.h"
#include "Graphics/Renderer.h"

UIImage::UIImage(std::weak_ptr<class Renderer> inPtrRenderer)
	:
	UIElement(inPtrRenderer)
{

}

UIImage::~UIImage()
{
}

float* UIImage::GenerateQuad()
{
	float leftX = mPosition.mX - mDimension.mX * mAlignment.mX * mScale.mX;
	float rightX = mPosition.mX + mDimension.mX * (1.0f - mAlignment.mX) * mScale.mX;

	float bottomY = mPosition.mY - mDimension.mY * mAlignment.mY * mScale.mY;
	float topY = mPosition.mY + mDimension.mY * (1.0f - mAlignment.mY) * mScale.mY;


	float quad[6][4] =
	{
		{ leftX,	topY,		0.0f,		0.0f},
		{ leftX,    bottomY,    0.0f,		mTiling.mY },
		{ rightX,	bottomY,    mTiling.mX, mTiling.mY },

		{ leftX,    topY,		0.0f,		0.0f },
		{ rightX,	bottomY,    mTiling.mX, mTiling.mY },
		{ rightX,	topY,		mTiling.mX, 0.0f }
	};
	return *quad;
}

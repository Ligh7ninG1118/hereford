#include "UIElement.h"
#include "Asset/Shader.h"
#include "Graphics/Renderer.h"


UIElement::UIElement(Renderer* inPtrRenderer, Shader* inPtrShader)
	:
	mPosition(Vec2(0.0f, 0.0f)),
	mScale(Vec2(1.0f, 1.0f)),
	mAlignment(Vec2(0.5f, 0.5f)),
	mDimension(Vec2(0.0f, 0.0f)),
	mAnchor(EUIAnchorPreset::BOTTOM_LEFT),
	mPtrShader(inPtrShader),
	mPtrRenderer(inPtrRenderer),
	mVAO(0),
	mVBO(0)
{
	/*auto lock = mPtrRenderer.lock();
	if (lock)
	{
		lock->AddUIElement(shared_from_this());
	}*/

	mPtrRenderer->AddUIElement(this);
}

UIElement::~UIElement()
{
	//auto lock = mPtrRenderer.lock();
	//if (lock)
	//{
	//	// TODO: Doesn't seem right? Is this going to create an additional copy?
	//	lock->RemoveUIElement(shared_from_this());
	//}
	mPtrRenderer->RemoveUIElement(this);

}

void UIElement::Initialize()
{
	UpdateContent();
}

void UIElement::UpdateContent()
{
	if (mVAO == 0)
		GenerateGLAsset();
	else
		UpdateGLAsset();
}

void UIElement::GenerateGLAsset()
{
	std::vector<float> uiQuad = GenerateQuad();

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uiQuad.size(), uiQuad.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void UIElement::UpdateGLAsset()
{
	std::vector<float> uiQuad = GenerateQuad();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	//size of need to be 4 * 6 * sizeof(float), but this is only a float pointer
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * uiQuad.size(), uiQuad.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


std::vector<float> UIElement::GenerateQuad()
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
		{	leftX,		topY,		0.0f, 0.0f ,
			leftX,		bottomY,    0.0f, 1.0f ,
			rightX,		bottomY,    1.0f, 1.0f ,

			leftX,		topY,		0.0f, 0.0f ,
			rightX,		bottomY,    1.0f, 1.0f ,
			rightX,		topY,		1.0f, 0.0f }
	};

	return quad;
}

void UIElement::SetUIProjection(const Mat4& uiProj)
{
	mPtrShader->Use();
	mPtrShader->SetMat4("projection", uiProj);
}


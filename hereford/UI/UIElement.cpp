#include "UIElement.h"
#include "Asset/Shader.h"
#include "Graphics/Renderer.h"


UIElement::UIElement(std::weak_ptr<Renderer> inPtrRenderer)
	:
	mPosition(Vec2(0.0f, 0.0f)),
	mScale(Vec2(1.0f, 1.0f)),
	mAlignment(Vec2(0.5f, 0.5f)),
	mDimension(Vec2(0.0f, 0.0f)),
	mAnchor(EUIAnchorPreset::BOTTOM_LEFT),
	mPtrShader(nullptr),
	mPtrRenderer(inPtrRenderer)
{
	auto lock = mPtrRenderer.lock();
	if (lock)
	{
		lock->AddUIElement(shared_from_this());
	}

	Initialize();
}

UIElement::~UIElement()
{
	auto lock = mPtrRenderer.lock();
	if (lock)
	{
		// TODO: Doesn't seem right? Is this going to create an additional copy?
		lock->RemoveUIElement(shared_from_this());
	}
}

void UIElement::Initialize()
{
	UpdateContent();
	GenerateGLAsset();
}

void UIElement::GenerateGLAsset()
{
	//TODO: Not sure if this is gonna work. should be, but worth checking
	float* uiQuad = GenerateQuad();

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uiQuad), &uiQuad, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void UIElement::UpdateGLAsset()
{
	float* uiQuad = GenerateQuad();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uiQuad), &uiQuad);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


float* UIElement::GenerateQuad()
{
	float leftX = mPosition.mX - mDimension.mX * mAlignment.mX * mScale.mX;
	float rightX = mPosition.mX + mDimension.mX * (1.0f - mAlignment.mX) * mScale.mX;

	float bottomY = mPosition.mY - mDimension.mY * mAlignment.mY * mScale.mY;
	float topY = mPosition.mY + mDimension.mY * (1.0f - mAlignment.mY) * mScale.mY;


	float quad[6][4] =
	{
		{ leftX,	topY,		0.0f, 0.0f},
		{ leftX,    bottomY,    0.0f, 1.0f },
		{ rightX,	bottomY,    1.0f, 1.0f },

		{ leftX,    topY,		0.0f, 0.0f },
		{ rightX,	bottomY,    1.0f, 1.0f },
		{ rightX,	topY,		1.0f, 0.0f }
	};
	//TODO: Returning local var!
	return *quad;
}

void UIElement::SetUIProjection(const Mat4& uiProj)
{
	mPtrShader->SetMat4("projection", uiProj);
}


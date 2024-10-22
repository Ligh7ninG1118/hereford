#include "UIOneInTheChamber.h"
#include "Asset/Shader.h"


UIOneInTheChamber::UIOneInTheChamber(Renderer* inPtrRenderer, Shader* inPtrShader, std::shared_ptr<class Texture> inPtrUITex)
	: UIImage(inPtrRenderer, inPtrShader, inPtrUITex)
{
}

UIOneInTheChamber::~UIOneInTheChamber()
{
}

void UIOneInTheChamber::Initialize(float xPos)
{
	SetAnchor(EUIAnchorPreset::BOTTOM_RIGHT);
	SetPosition(Vec2(xPos, 100.0f));
	SetAlignment(Vec2(1.0f, 0.5f));
	SetDimension(Vec2(5.5f, 32.0f));
	SetTiling(Vec2(1.0f, 1.0f));

	mPtrShader->Use();
	mPtrShader->SetVec4("uiColor", 1.0f, 0.93f, 0.5f, 0.9f);
	mPtrShader->SetInt("uiTex", 0);

	UIImage::Initialize();
}

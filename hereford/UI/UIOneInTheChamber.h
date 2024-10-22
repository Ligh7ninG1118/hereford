#pragma once
#include "UIImage.h"

class UIOneInTheChamber : public UIImage
{
public:
	UIOneInTheChamber(class Renderer* inPtrRenderer, class Shader* inPtrShader, std::shared_ptr<class Texture> inPtrUITex);
	~UIOneInTheChamber();

	void Initialize(float xPos);
};


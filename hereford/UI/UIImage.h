#pragma once
#include "UIElement.h"

class UIImage : public UIElement
{
public:
    UIImage(class Renderer* inPtrRenderer, class Shader* inPtrShader, std::shared_ptr<class Texture> inPtrUITex);
    ~UIImage();

    std::vector<float> GenerateQuad() override;

    inline Vec2 GetTiling() const { return mTiling; }
    inline void SetTiling(Vec2 inTiling) { mTiling = inTiling; }

    inline std::shared_ptr<class Texture> GetTexture() const { return mPtrUITexture; }
    inline void SetTexture(std::shared_ptr<class Texture> inPtrTexture) { mPtrUITexture = inPtrTexture; }


protected:
    std::shared_ptr<class Texture> mPtrUITexture;

    Vec2 mTiling;
};


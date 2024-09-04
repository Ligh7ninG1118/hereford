#pragma once
#include "UIElement.h"

class UIImage : public UIElement
{
public:
    UIImage(std::weak_ptr<class Renderer> inPtrRenderer);
    ~UIImage();

    float* GenerateQuad() override;

    inline Vec2 GetTiling() const { return mTiling; }
    inline void SetTiling(Vec2 inTiling) { mTiling.mX = Math::Clamp(inTiling.mX, 0.0f, 1.0f); mTiling.mY = Math::Clamp(inTiling.mY, 0.0f, 1.0f); }

    inline std::shared_ptr<class Texture> GetTexture() const { return mPtrUITexture; }
    inline void SetTexture(std::shared_ptr<class Texture> inPtrTexture) { mPtrUITexture = inPtrTexture; }


protected:
    std::shared_ptr<class Texture> mPtrUITexture;

    Vec2 mTiling;
};


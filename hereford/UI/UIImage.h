#pragma once
#include "UIElement.h"

class UIImage :
    public UIElement
{
public:
    UIImage(std::weak_ptr<class Renderer> inPtrRenderer);
    ~UIImage();

    inline std::shared_ptr<class Texture> GetTexture() const { return mPtrUITexture; }
    inline void SetTexture(std::shared_ptr<class Texture> inPtrTexture) { mPtrUITexture = inPtrTexture; }


protected:
    std::shared_ptr<class Texture> mPtrUITexture;

};


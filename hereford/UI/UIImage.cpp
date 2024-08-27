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

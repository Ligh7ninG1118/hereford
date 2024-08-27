#include "UIElement.h"

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
}

UIElement::~UIElement()
{
	auto lock = mPtrRenderer.lock();
	if (lock)
	{
		// TODO: Doesn't seem right? Shouldn't create additional shared pointer
		lock->RemoveUIElement(shared_from_this());
	}
}


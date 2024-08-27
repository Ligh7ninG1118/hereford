#include "UIElement.h"

UIElement::UIElement(Vec2 inPos, Vec2 inScale, Vec2 inAlignment, EUIAnchorPreset inAnchor)
	:
	mPosition(inPos),
	mScale(inScale),
	mAlignment(inAlignment),
	mAnchor(inAnchor)
{
}

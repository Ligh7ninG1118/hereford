#pragma once
#include "Math//Math.h"

enum class EUIAnchorPreset
{
	BOTTOM_LEFT = 0,		// Default case
	BOTTOM_MID,
	BOTTOM_RIGHT,
	CENTER_LEFT,
	CENTER_MID,
	CENTER_RIGHT,
	TOP_LEFT,
	TOP_MID,
	TOP_RIGHT,
};

class UIElement
{
public:
	UIElement(Vec2 inPos, Vec2 inScale = Vec2(0.5f, 0.5f), Vec2 inAlignment = Vec2(0.5f, 0.5f), EUIAnchorPreset inAnchor = EUIAnchorPreset::BOTTOM_LEFT);
	~UIElement() = default;

	inline Vec2 GetPosition() const { return mPosition; }
	inline Vec2 GetScale() const { return mScale; }
	inline Vec2 GetAlignment() const { return mAlignment; }
	inline EUIAnchorPreset GetAnchor() const { return mAnchor; }

	inline void SetPosition(Vec2 inPos) { mPosition = inPos; }
	inline void SetScale(Vec2 inScale) { mScale = inScale; }
	inline void SetAlignment(Vec2 inAlignment) { mAlignment = inAlignment; }
	inline void SetAnchor(EUIAnchorPreset inAnchor) { mAnchor = inAnchor; }

private:
	Vec2 mPosition;
	Vec2 mScale;
	Vec2 mAlignment;	// Need to know width/height first? How should I do that for text of arbitary width
	EUIAnchorPreset mAnchor;
};


#pragma once
#include "Math/Math.h"

#include <memory>

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
	: public std::enable_shared_from_this<UIElement>
{
public:
	UIElement(std::weak_ptr<class Renderer> inPtrRenderer);
	~UIElement();

	inline Vec2 GetPosition() const { return mPosition; }
	inline Vec2 GetScale() const { return mScale; }
	inline Vec2 GetAlignment() const { return mAlignment; }
	inline Vec2 GetDimension() const { return mDimension; }
	inline EUIAnchorPreset GetAnchor() const { return mAnchor; }

	inline std::shared_ptr<class Shader> GetShader() const { return mPtrShader; }

	inline void SetPosition(Vec2 inPos) { mPosition = inPos; }
	inline void SetScale(Vec2 inScale) { mScale = inScale; }
	inline void SetAlignment(Vec2 inAlignment) { mAlignment = inAlignment; }
	inline void SetDimension(Vec2 inDimension) { mDimension = inDimension; }
	inline void SetAnchor(EUIAnchorPreset inAnchor) { mAnchor = inAnchor; }

	inline void SetShader(std::shared_ptr<class Shader> inPtrShader) { mPtrShader = inPtrShader; }

protected:
	Vec2 mPosition;
	Vec2 mScale;
	Vec2 mAlignment;	// Need to know width/height first? How should I do that for text of arbitary width
	Vec2 mDimension;
	EUIAnchorPreset mAnchor;

	std::shared_ptr<class Shader> mPtrShader;
	std::weak_ptr<class Renderer> mPtrRenderer;
};


#pragma once
#include "Math/Math.h"
#include <vector>
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

class UIElement : public std::enable_shared_from_this<UIElement>
{
public:
	UIElement(class Renderer* inPtrRenderer, class Shader* inPtrShader);
	~UIElement();

	virtual void Initialize();
	virtual void UpdateContent();
	

	void SetUIProjection(const Mat4& uiProj);


	inline Vec2 GetPosition() const { return mPosition; }
	inline Vec2 GetScale() const { return mScale; }
	inline Vec2 GetAlignment() const { return mAlignment; }
	inline Vec2 GetDimension() const { return mDimension; }
	inline EUIAnchorPreset GetAnchor() const { return mAnchor; }

	inline uint32 GetVAO() const { return mVAO; }
	inline uint32 GetVBO() const { return mVBO; }


	inline class Shader* GetShader() const { return mPtrShader; }

	inline void SetPosition(Vec2 inPos) { mPosition = inPos; }
	inline void SetScale(Vec2 inScale) { mScale = inScale; }
	inline void SetAlignment(Vec2 inAlignment) { mAlignment.mX = Math::Clamp(inAlignment.mX, 0.0f, 1.0f); mAlignment.mY = Math::Clamp(inAlignment.mY, 0.0f, 1.0f);}
	inline void SetDimension(Vec2 inDimension) { mDimension = inDimension; }
	inline void SetAnchor(EUIAnchorPreset inAnchor) { mAnchor = inAnchor; }

	inline void SetShader(class Shader* inPtrShader) { mPtrShader = inPtrShader; }

protected:
	virtual void GenerateGLAsset();
	virtual void UpdateGLAsset();
	virtual std::vector<float> GenerateQuad();

	Vec2 mPosition;
	Vec2 mScale;
	Vec2 mAlignment;
	Vec2 mDimension;
	EUIAnchorPreset mAnchor;

	class Shader* mPtrShader;
	Renderer* mPtrRenderer;

	uint32 mVAO;
	uint32 mVBO;
};


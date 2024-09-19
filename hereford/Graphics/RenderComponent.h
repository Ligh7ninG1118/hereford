#pragma once

#include "Core/Component.h"
#include "Math/Math.h"

enum class ERenderLayer : uint8
{
	Default = 1,
	Weapon = 1 << 1
};


class RenderComponent : public Component
{
public:
	RenderComponent(class Actor* owner, class Renderer& rendererRef, ERenderLayer renderLayer = ERenderLayer::Default);
	~RenderComponent();

	void LoadTextures();

	inline std::shared_ptr<class Shader> GetShader() const { return mPtrShader; }
	inline void SetShader(std::shared_ptr<class Shader> inPtrShader) { mPtrShader = inPtrShader; }

	inline ERenderLayer GetRenderLayer() const { return mRenderLayer; }
	inline void SetRenderLayer(ERenderLayer inOrder) { mRenderLayer = inOrder; }

	inline uint32 GetVAOID() const { return mVAO; }
	inline void SetVAOID(uint32 id) { mVAO = id; }

	inline uint32 GetVBOID() const { return mVBO; }
	inline void SetVBOID(uint32 id) { mVBO = id; }

	inline Vec3 GetColor() const { return mAlbedoColor; }
	inline void SetColor(Vec3 inColor) { mAlbedoColor = inColor; }

	inline Vec3 GetTranslateOffset() const { return mTranslateOffset; }
	inline void SetTranslateOffset(Vec3 inOffset) { mTranslateOffset = inOffset; }

	inline Vec3 GetRotateOffset() const { return mRotateOffset; }
	inline void SetRotateOffset(Vec3 inOffset) { mRotateOffset = inOffset; }

	inline Vec3 GetScaleOffset() const { return mScaleOffset; }
	inline void SetScaleOffset(Vec3 inOffset) { mScaleOffset = inOffset; }

	virtual Mat4 GetModelMatrix() const;
	std::vector<std::shared_ptr<class Texture>> mTextures;

protected:
	class Renderer& m_Renderer;

	ERenderLayer mRenderLayer;

	std::shared_ptr<class Shader> mPtrShader;

	uint32 mVAO;
	uint32 mVBO;

	Vec3 mAlbedoColor;

	Vec3 mTranslateOffset;
	Vec3 mRotateOffset;
	Vec3 mScaleOffset;
};


#pragma once

#include "Core/Component.h"
#include "Math/Math.h"
#include <string>
#include <vector>
#include "Asset/Texture.h"
#include "Asset/Model.h"

const uint16 RM_PURECOLOR		= 0x0001u;
const uint16 RM_EMBEDDEDTEX		= 0x0002u;
const uint16 RM_EXPLICITTEX		= 0x0004u;

const uint16 RM_ANIMATED		= 0x0010u;
const uint16 RM_STATIC			= 0x0020u;

const uint16 RM_FLAT			= 0x0100u;
const uint16 RM_LIGHTING		= 0x0200u;
const uint16 RM_IBL				= 0x0400u;
const uint16 RM_LIGHTINGANDIBL	= 0x0600u;

const uint16 RM_MODELMESH		= 0x1000u;
const uint16 RM_SIMPLEMESH		= 0x2000u;


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


	inline std::shared_ptr<class Shader> GetShader() const { return mPtrShader; }
	inline void SetShader(std::shared_ptr<class Shader> inPtrShader) { mPtrShader = inPtrShader; }

	inline ERenderLayer GetRenderLayer() const { return mRenderLayer; }
	inline void SetRenderLayer(ERenderLayer inOrder) { mRenderLayer = inOrder; }

	inline uint32 GetVAOID() const { return mVAO == 0 ? mPtrModel->mMeshes[0].mVAOID : mVAO; }
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

	inline uint16 GetRenderModeFlag() const { return mRenderModeFlag; }
	inline void SetRenderModeFlag(uint16 flag) { mRenderModeFlag = flag; }

	inline std::weak_ptr<Model> GetModel() const { return mPtrModel; }
	inline const std::vector<Mesh>& GetMeshes() const { return mPtrModel->mMeshes; }
	inline void SetModel(std::shared_ptr<Model> inModel) { mPtrModel = inModel; }
	inline const std::vector<std::shared_ptr<Texture>>& GetTextures() const { return mTextures; }
	void LoadTexture(const std::string& texPath, ETextureType texType);

	virtual Mat4 GetModelMatrix() const;

	inline bool GetIsCombinedMRTex() const { return mCombinedMRTex; }
	inline void SetIsCombinedMRTex(bool value) { mCombinedMRTex = value; }

protected:
	class Renderer& m_Renderer;

	ERenderLayer mRenderLayer;
	uint16 mRenderModeFlag;

	std::shared_ptr<Model> mPtrModel;
	std::vector<std::shared_ptr<class Texture>> mTextures;
	std::shared_ptr<class Shader> mPtrShader;

	uint32 mVAO;
	uint32 mVBO;

	Vec3 mAlbedoColor;
	bool mCombinedMRTex;

	Vec3 mTranslateOffset;
	Vec3 mRotateOffset;
	Vec3 mScaleOffset;
};


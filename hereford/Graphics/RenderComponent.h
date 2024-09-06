#pragma once

#include "Core/Component.h"

enum class ERenderMode
{

};


class RenderComponent : public Component
{
public:
	RenderComponent(class Actor* owner, class Renderer& rendererRef);
	~RenderComponent();

	inline std::shared_ptr<class Shader> GetShader() const { return mPtrShader; }
	inline void SetShader(std::shared_ptr<class Shader> inPtrShader) { mPtrShader = inPtrShader; }

	inline uint32 GetVAOID() const { return mVAO; }
	inline void SetVAOID(uint32 id) { mVAO = id; }

	inline uint32 GetVBOID() const { return mVBO; }
	inline void SetVBOID(uint32 id) { mVBO = id; }

	inline Vec3 GetColor() const { return mAlbedoColor; }
	inline void SetColor(Vec3 inColor) { mAlbedoColor = inColor; }

	Mat4 GetModelMatrix() const;

private:
	class Renderer& m_Renderer;

	std::shared_ptr<class Shader> mPtrShader;
	uint32 mVAO;
	uint32 mVBO;

	Vec3 mAlbedoColor;

	//need to add custom parameters for T/R/S offset
};


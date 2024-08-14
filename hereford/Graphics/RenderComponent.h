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

	bool Initialize();

	unsigned int GetShaderID();
	void SetShaderID(unsigned int id);

	unsigned int GetVAOID();
	void SetVAOID(unsigned int id);

	unsigned int GetVBOID();
	void SetVBOID(unsigned int id);

	Mat4 GetModelMatrix() const;
	Vec3 color;


private:
	class Renderer& m_Renderer;

	float m_Vertices;

	unsigned int m_ShaderID;
	unsigned int m_VertexAttributeObjectID;
	unsigned int m_VertexBufferObjectID;

	//need to add custom parameters for T/R/S offset
};


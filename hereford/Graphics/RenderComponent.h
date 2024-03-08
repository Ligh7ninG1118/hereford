#pragma once

#include "../Core/Component.h"
#include "Renderer.h"


class RenderComponent : public Component
{
public:
	RenderComponent(class Actor* owner, Renderer* pRenderer);
	~RenderComponent();

	bool Initialize();

	unsigned int GetShaderID();
	void SetShaderID(unsigned int id);

	unsigned int GetVAOID();
	void SetVAOID(unsigned int id);

	unsigned int GetVBOID();
	void SetVBOID(unsigned int id);


private:
	Renderer* pRenderer;

	float m_Vertices;

	unsigned int m_ShaderID;
	unsigned int m_VertexAttributeObjectID;
	unsigned int m_VertexBufferObjectID;

};


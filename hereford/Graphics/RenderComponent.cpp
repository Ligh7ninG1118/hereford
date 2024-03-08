#include "RenderComponent.h"
#include "../Core/Actor.h"

RenderComponent::RenderComponent(Actor* owner, Renderer* renderer)
	: Component(owner),
	pRenderer(renderer)
{
}

RenderComponent::~RenderComponent()
{
}

bool RenderComponent::Initialize()
{
	
	
	return false;

}

unsigned int RenderComponent::GetShaderID()
{
	return m_ShaderID;
}

void RenderComponent::SetShaderID(unsigned int id)
{
	m_ShaderID = id;
}

unsigned int RenderComponent::GetVAOID()
{
	return m_VertexAttributeObjectID;
}

void RenderComponent::SetVAOID(unsigned int id)
{
	m_VertexAttributeObjectID = id;
}

unsigned int RenderComponent::GetVBOID()
{
	return m_VertexBufferObjectID;
}

void RenderComponent::SetVBOID(unsigned int id)
{
	m_VertexBufferObjectID = id;
}

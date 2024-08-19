#include "Core/Actor.h"
#include "Core/GameContext.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderComponent.h"

#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

RenderComponent::RenderComponent(Actor* owner, Renderer& rendererRef)
	: 
	Component(owner),
	m_Renderer(rendererRef)
{
	m_Renderer.AddRenderComponent(this);
	Initialize();
}

RenderComponent::~RenderComponent()
{
	m_Renderer.RemoveRenderComponent(this);
}

bool RenderComponent::Initialize()
{
	//SetShaderID(mOwner->GetGame()->GetShader("Graphics/Shaders/standard_vert.glsl", "Graphics/Shaders/standard_frag.glsl"));
	//SetShaderID(mOwner->GetGame()->GetShader("Graphics/Shaders/model_tex_vert.glsl", "Graphics/Shaders/model_tex_frag.glsl"));

	//SetVAOID(mOwner->GetGame()->GetMesh("block"));
	
	return true;

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

Mat4 RenderComponent::GetModelMatrix() const
{
	//TODO: Quaternion rotation

	Mat4 mat = Mat4::Identity;
	Vec3 rot = mOwner->GetRotation();

	mat.Translate(mOwner->GetPosition());
	mat.Scale(mOwner->GetScale());
	mat.Rotate(DEG2RAD * rot.mX, Vec3(1.0f, 0.0f, 0.0f));
	mat.Rotate(DEG2RAD * rot.mY, Vec3(0.0f, 1.0f, 0.0f));
	mat.Rotate(DEG2RAD * rot.mZ, Vec3(0.0f, 0.0f, 1.0f));
	return mat;
}

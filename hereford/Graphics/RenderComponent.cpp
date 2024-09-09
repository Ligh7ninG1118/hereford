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
	m_Renderer(rendererRef),
	mTranslateOffset(Vec3::Zero),
	mRotateOffset(Vec3::Zero),
	mScaleOffset(Vec3::One)
{
	m_Renderer.AddRenderComponent(this);
}

RenderComponent::~RenderComponent()
{
	m_Renderer.RemoveRenderComponent(this);
}

Mat4 RenderComponent::GetModelMatrix() const
{
	Mat4 mat = Mat4::Identity;
	Vec3 rot = mOwner->GetRotation();

	mat.Scale(mOwner->GetScale());
	mat.Scale(mScaleOffset);
	mat.Rotate(rot);
	mat.Rotate(mRotateOffset);
	mat.Translate(mOwner->GetPosition());
	mat.Translate(mTranslateOffset);
	
	

	/*mat.Rotate(DEG2RAD * rot.mX, Vec3(1.0f, 0.0f, 0.0f));
	mat.Rotate(DEG2RAD * rot.mY, Vec3(0.0f, 1.0f, 0.0f));
	mat.Rotate(DEG2RAD * rot.mZ, Vec3(0.0f, 0.0f, 1.0f));*/
	return mat;
}

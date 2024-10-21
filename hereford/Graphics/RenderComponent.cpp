#include "Core/Actor.h"
#include "Core/GameContext.h"

#include "Asset/AssetManager.h"
#include "Asset/Texture.h"

#include "Props/TestCube.h"
#include "Props/Ground.h"
#include "Props/RangeTarget.h"

#include "Graphics/Renderer.h"
#include "Graphics/RenderComponent.h"

#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

RenderComponent::RenderComponent(Actor* owner, Renderer& rendererRef, ERenderLayer renderLayer)
	:
	Component(owner),
	m_Renderer(rendererRef),
	mRenderLayer(renderLayer),
	mRenderModeFlag(0u),
	mVAO(0),
	mVBO(0),
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

void RenderComponent::LoadTexture(const std::string& texPath, ETextureType texType)
{
	mTextures.push_back(AssetManager::LoadAsset<Texture>(texPath));
	mTextures[mTextures.size()-1]->SetType(texType);
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
	
	return mat;
}

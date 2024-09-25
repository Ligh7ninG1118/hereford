#include "Core/Actor.h"
#include "Core/GameContext.h"

#include "Asset/AssetManager.h"
#include "Asset/Texture.h"

#include "Props/TestCube.h"
#include "Props/Ground.h"


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

void RenderComponent::LoadTextures()
{
	Actor* owner = GetOwner();
	if (dynamic_cast<TestCube*>(owner))
	{
		mTextures.push_back(AssetManager::LoadAsset<Texture>(std::string("LocalResources/used-stainless-steel2/used-stainless-steel2_albedo.png")));
		mTextures.push_back(AssetManager::LoadAsset<Texture>(std::string("LocalResources/used-stainless-steel2/used-stainless-steel2_ao.png")));
		mTextures.push_back(AssetManager::LoadAsset<Texture>(std::string("LocalResources/used-stainless-steel2/used-stainless-steel2_height.png")));
		mTextures.push_back(AssetManager::LoadAsset<Texture>(std::string("LocalResources/used-stainless-steel2/used-stainless-steel2_metallic.png")));
		mTextures.push_back(AssetManager::LoadAsset<Texture>(std::string("LocalResources/used-stainless-steel2/used-stainless-steel2_normal-ogl.png")));
		mTextures.push_back(AssetManager::LoadAsset<Texture>(std::string("LocalResources/used-stainless-steel2/used-stainless-steel2_roughness.png")));

		mTextures[0]->SetType(ETextureType::DIFFUSE);
		mTextures[1]->SetType(ETextureType::AMBIENT);
		mTextures[2]->SetType(ETextureType::HEIGHT);
		mTextures[3]->SetType(ETextureType::METALLIC);
		mTextures[4]->SetType(ETextureType::NORMALS);
		mTextures[5]->SetType(ETextureType::ROUGHNESS);
	}
	else if (dynamic_cast<Ground*>(owner))
	{
		mTextures.push_back(AssetManager::LoadAsset<Texture>(std::string("LocalResources/gravel/gravel_albedo.png")));
		mTextures.push_back(AssetManager::LoadAsset<Texture>(std::string("LocalResources/gravel/gravel_ao.png")));
		mTextures.push_back(AssetManager::LoadAsset<Texture>(std::string("LocalResources/gravel/gravel_height.png")));
		mTextures.push_back(AssetManager::LoadAsset<Texture>(std::string("LocalResources/gravel/gravel_metallic.png")));
		mTextures.push_back(AssetManager::LoadAsset<Texture>(std::string("LocalResources/gravel/gravel_normal-ogl.png")));
		mTextures.push_back(AssetManager::LoadAsset<Texture>(std::string("LocalResources/gravel/gravel_roughness.png")));

		mTextures[0]->SetType(ETextureType::DIFFUSE);
		mTextures[1]->SetType(ETextureType::AMBIENT);
		mTextures[2]->SetType(ETextureType::HEIGHT);
		mTextures[3]->SetType(ETextureType::METALLIC);
		mTextures[4]->SetType(ETextureType::NORMALS);
		mTextures[5]->SetType(ETextureType::ROUGHNESS);
	}

	

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

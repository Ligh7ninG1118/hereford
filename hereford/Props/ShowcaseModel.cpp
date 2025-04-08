#include "ShowcaseModel.h"
#include "Core/GameContext.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderComponent.h"
#include "Asset/AssetManager.h"


ShowcaseModel::ShowcaseModel(GameContext* gameCtx) 
	:
	Actor(gameCtx)
{
	mPtrRenderComp = new RenderComponent(static_cast<Actor*>(this), gameCtx->GetRenderer());
	mPtrRenderComp->SetShader(AssetManager::LoadAsset<Shader>(std::string("Shaders/model_tex_pbr_vert.glsl*Shaders/model_tex_pbr_frag.glsl")));
	mPtrRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/red button/redbutton.dae")));
	mPtrRenderComp->SetRenderModeFlag(RM_EMBEDDEDTEX | RM_STATIC | RM_LIGHTINGANDIBL | RM_MODELMESH);
	mPtrRenderComp->SetRotateOffset(Vec3(90.0f, 0.0f, 0.0f));
	mPtrRenderComp->SetScaleOffset(Vec3(0.2f, 0.2f, 0.2f));
	mPtrRenderComp->SetIsCombinedMRTex(false);

	auto model = mPtrRenderComp->GetModel();

	Texture diffuseTex("LocalResources/red button/Material_albedo.jpg");
	diffuseTex.SetType(ETextureType::DIFFUSE);
	model.lock()->ManualLoadTexture(diffuseTex);

	Texture aoTex("LocalResources/red button/Material_albedo.jpg");
	aoTex.SetType(ETextureType::AMBIENT_OCCLUSION);
	model.lock()->ManualLoadTexture(aoTex);

	Texture normalTex("LocalResources/red button/Material_normal.jpg");
	normalTex.SetType(ETextureType::NORMALS);
	model.lock()->ManualLoadTexture(normalTex);

	Texture metallicTex("LocalResources/red button/Material_metallic.jpg");
	metallicTex.SetType(ETextureType::METALLIC);
	model.lock()->ManualLoadTexture(metallicTex);

	Texture roughnessTex("LocalResources/red button/Material_roughness.jpg");
	roughnessTex.SetType(ETextureType::ROUGHNESS);
	model.lock()->ManualLoadTexture(roughnessTex);
}

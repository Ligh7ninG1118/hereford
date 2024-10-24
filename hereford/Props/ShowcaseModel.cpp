#include "ShowcaseModel.h"
#include "Core/GameContext.h"
#include "Graphics/RenderComponent.h"
#include "Asset/AssetManager.h"


ShowcaseModel::ShowcaseModel(GameContext* gameCtx) 
	:
	Actor(gameCtx)
{
	mPtrRenderComp = new RenderComponent(static_cast<Actor*>(this), gameCtx->GetRenderer());
	mPtrRenderComp->SetShader(AssetManager::LoadAsset<Shader>(std::string("Shaders/model_tex_pbr_vert.glsl*Shaders/model_tex_pbr_frag.glsl")));
	mPtrRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/damagedhelmet/DamagedHelmet.gltf")));
	mPtrRenderComp->SetRenderModeFlag(RM_EMBEDDEDTEX | RM_STATIC | RM_LIGHTINGANDIBL | RM_MODELMESH);
	mPtrRenderComp->SetRotateOffset(Vec3(90.0f, 0.0f, 0.0f));
	mPtrRenderComp->SetIsCombinedMRTex(true);
}

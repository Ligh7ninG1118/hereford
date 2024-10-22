#include "PlywoodWall.h"
#include "Graphics/RenderComponent.h"
#include "Core/GameContext.h"
#include "Asset/AssetManager.h"
#include "Asset/Texture.h"


PlywoodWall::PlywoodWall(GameContext* gameCtx, int type)
	:
	Actor(gameCtx),
	mType(type)
{
	mPtrRenderComp = new RenderComponent(static_cast<Actor*>(this), gameCtx->GetRenderer());
	switch (mType)
	{
	case 1:
		mPtrRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/Plywood/plywood-corner-m.fbx")));
		break;
	case 2:
		mPtrRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/Plywood/plywood-doorway-m.fbx")));
		break;
	case 3:
		mPtrRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/Plywood/plywood-wall-m.fbx")));
		break;
	case 4:
		mPtrRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/Plywood/plywood-wall-wide-m.fbx")));
		break;
	case 5:
		mPtrRenderComp->SetModel(AssetManager::LoadAsset<Model>(std::string("LocalResources/Plywood/plywood-window-m.fbx")));
		break;
	}


	std::shared_ptr<Shader> shader = AssetManager::LoadAsset<Shader>(std::string("Shaders/model_tex_phong_vert.glsl*Shaders/model_simple_frag.glsl"));
	mPtrRenderComp->SetShader(shader);
	mPtrRenderComp->SetRenderModeFlag(RM_DIFFUSETEX | RM_STATIC | RM_PBR | RM_MODELMESH);


	auto model = mPtrRenderComp->GetModel();

	Texture diffuseTex("LocalResources/Plywood/Wal_D.jpg");
	diffuseTex.SetType(ETextureType::DIFFUSE);
	model.lock()->ManualLoadTexture(diffuseTex);

	Texture specTex("LocalResources/Plywood/Wal_S.jpg");
	specTex.SetType(ETextureType::SPECULAR);
	model.lock()->ManualLoadTexture(specTex);
}

PlywoodWall::~PlywoodWall()
{
}

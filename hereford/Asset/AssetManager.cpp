#include "AssetManager.h"


AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
	mAssetsMap.clear();
}

void AssetManager::Destroy()
{
	mAssetsMap.clear();
}

std::unordered_map<std::string, std::shared_ptr<Asset>> AssetManager::mAssetsMap;


#include "AssetManager.h"


AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
	printf("AssetManager Destructor\n");

	mAssetsMap.clear();
}

void AssetManager::Destroy()
{
	printf("AssetManager Destroy\n");
	//TODO: need to manually clear all pointers?
	mAssetsMap.clear();
}

std::unordered_map<std::string, std::shared_ptr<Asset>> AssetManager::mAssetsMap;


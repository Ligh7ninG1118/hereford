#include "AssetManager.h"


AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
	//TODO: need to manually clear all pointers?
	mAssetsMap.clear();
}

std::unordered_map<std::string, std::shared_ptr<Asset>> AssetManager::mAssetsMap;

